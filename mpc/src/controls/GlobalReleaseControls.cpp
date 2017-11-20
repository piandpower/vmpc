#include "GlobalReleaseControls.hpp"

#include <Mpc.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <audiomidi/EventHandler.hpp>
#include <controls/KbMapping.hpp>
#include <controls/KbMouseController.hpp>
//#include <controls/StepEditorKbRelease.hpp>
#include <ui/Uis.hpp>
#include <lcdgui/LayeredScreen.hpp>
//#include <hardware/LedPanel.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sequencer/FrameSeq.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/NoteEvent.hpp>
#include <sequencer/Sequencer.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>

using namespace mpc::controls;
using namespace std;

GlobalReleaseControls::GlobalReleaseControls(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	samplerGui = mpc->getUis().lock()->getSamplerGui();
	sampler = mpc->getSampler();
	sequencer = mpc->getSequencer();
}

void GlobalReleaseControls::init()
{
    csn = mpc->getLayeredScreen().lock()->getCurrentScreenName();
	auto lSequencer = sequencer.lock();
    track = lSequencer->getActiveSequence().lock()->getTrack(lSequencer->getActiveTrackIndex());
	auto lTrk = track.lock();
	auto drum = lTrk->getBusNumber() - 1;
	if (drum >= 0 && !mpc->getAudioMidiServices().lock()->isDisabled()) {
		auto lSampler = sampler.lock();
		mpcSoundPlayerChannel = lSampler->getDrum(drum);
		program = lSampler->getProgram(mpcSoundPlayerChannel->getProgram());
	}
    bank = samplerGui->getBank();
}

void GlobalReleaseControls::keyEvent(unsigned char kc)
{
	init();
	auto lSequencer = sequencer.lock();
	//auto ledPanel = lMainFrame->getLedPanel().lock();
	if (kc == KbMapping::goTo())
		mpc->getKbmc().lock()->setGoToPressed(false);

	if (kc == KbMapping::rec()) {
		mpc->getKbmc().lock()->setRecPressed(false);
		if (lSequencer->isRecording()) return;
		//ledPanel->setRec(false);
	}
	if (kc == KbMapping::overdub()) {
		mpc->getKbmc().lock()->setOverdubPressed(false);
		if (lSequencer->isOverDubbing()) return;
		//ledPanel->setOverDub(false);
	}
	if (csn.compare("trackmute") == 0 && kc == KbMapping::f6() && !lSequencer->isSoloEnabled()) {
		auto ls = mpc->getLayeredScreen().lock();
		ls->removeCurrentBackground();
		ls->setCurrentBackground("trackmute");
		//KbMouseController::f6IsPressed = false;
		return;
	}

	auto pressedPad = mpc->getKbmc().lock()->getPressedPad(kc);

	if (pressedPad != -1) {
		pad(pressedPad);
	}
	
	if (csn.compare("step_tc") == 0 || csn.compare("sequencer_step") == 0) {
		//StepEditorKbRelease(lMainFrame.get()).stepEditorKbRelease(kc);
	}

	if ((kc == KbMapping::f6() || kc == KbMapping::f5()) && !lSequencer->isPlaying() && csn.compare("sequencer") != 0)
		sampler.lock()->stopAllVoices();
}

void GlobalReleaseControls::pad(int i)
{
	init();
	auto pressedPads = mpc->getKbmc().lock()->getPressedPads();
	if (pressedPads->find(i) == pressedPads->end()) return;
	pressedPads->erase(pressedPads->find(i));
	
	if (csn.compare("loadasound") == 0) return;

	auto lTrk = track.lock();
	auto note = lTrk->getBusNumber() > 0 ? program.lock()->getPad(i + (bank * 16))->getNote() : i + (bank * 16) + 35;
	generateNoteOff(note);
	
	if (csn.compare("sequencer_step") == 0) {
		auto newDur = static_cast<int>(mpc->getAudioMidiServices().lock()->getFrameSequencer().lock()->getTickPosition());
		sequencer.lock()->stopMetronomeTrack();
		lTrk->adjustDurLastEvent(newDur);
	}
}

void GlobalReleaseControls::generateNoteOff(int nn)
{
    init();
	auto lSequencer = sequencer.lock();
	auto lTrk = track.lock();
    if(lSequencer->isRecordingOrOverdubbing()) {
        auto n = new mpc::sequencer::NoteEvent();
        n->setNote(nn);
        n->setVelocity(0);
        n->setTick(lSequencer->getTickPosition());
        lTrk->recordNoteOff(n);
    }
    auto noteEvent = make_shared<mpc::sequencer::NoteEvent>(nn);
    noteEvent->setVelocity(0);
    noteEvent->setDuration(0);
    noteEvent->setTick(-1);
    mpc->getEventHandler().lock()->handle(noteEvent, lTrk.get());
}

void GlobalReleaseControls::overDub()
{
	mpc->getKbmc().lock()->setOverdubPressed(false);
    init();
    //mainFrame.lock()->getLedPanel().lock()->setOverDub(sequencer.lock()->isOverDubbing());
}

void GlobalReleaseControls::rec()
{
	mpc->getKbmc().lock()->setRecPressed(false);
    init();
    //mainFrame.lock()->getLedPanel().lock()->setRec(sequencer.lock()->isRecording());
}

void GlobalReleaseControls::tap()
{
	mpc->getKbmc().lock()->setTapPressed(false);
	auto lSequencer = sequencer.lock();
    if (lSequencer->isRecordingOrOverdubbing())
        lSequencer->flushTrackNoteCache();
}

void GlobalReleaseControls::shift()
{
	mpc->getKbmc().lock()->releaseShift();
}

void GlobalReleaseControls::erase()
{
	mpc->getKbmc().lock()->setErasePressed(false);
}

GlobalReleaseControls::~GlobalReleaseControls() {
}
