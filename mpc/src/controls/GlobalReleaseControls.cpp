#include <controls/GlobalReleaseControls.hpp>

#include <Mpc.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <audiomidi/EventHandler.hpp>
#include <controls/KbMapping.hpp>
//#include <controls/StepEditorKbRelease.hpp>
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
	: AbstractControls(mpc)
{
}

/*
void GlobalReleaseControls::keyEvent(unsigned char kc)
{
	init();
	auto lSequencer = sequencer.lock();
	auto lMainFrame = mainFrame.lock();
	auto ledPanel = lMainFrame->getLedPanel().lock();
	if (kc == KbMapping::goTo())
		KbMouseController::goToIsPressed() = false;

	if (kc == KbMapping::rec()) {
		KbMouseController::recIsPressed() = false;
		if (lSequencer->isRecording()) return;
		ledPanel->setRec(false);
	}
	if (kc == KbMapping::overdub()) {
		KbMouseController::overdubIsPressed() = false;
		if (lSequencer->isOverDubbing()) return;
		ledPanel->setOverDub(false);
	}
	if (csn.compare("trackmute") == 0 && kc == KbMapping::f6() && !lSequencer->isSoloEnabled()) {
		auto ls = lMainFrame->getLayeredScreen().lock();
		ls->removeCurrentBackground();
		ls->setCurrentBackground("trackmute");
		//KbMouseController::f6IsPressed() = false;
		return;
	}
	if (KbMouseController::getPressedPad(kc) != -1) {
		auto pressedPad = KbMouseController::getPressedPad(kc);
		pad(pressedPad);
	}
	if (csn.compare("step_tc") == 0 || csn.compare("sequencer_step") == 0) {
		StepEditorKbRelease(lMainFrame.get()).stepEditorKbRelease(kc);
	}

	if ((kc == KbMapping::f6() || kc == KbMapping::f5()) && !lSequencer->isPlaying() && csn.compare("sequencer") != 0)
		sampler.lock()->stopAllVoices();
}
*/

void GlobalReleaseControls::pad(int i, int velo, bool repeat, int tick)
{
	init();
	/*
	if (KbMouseController::pressedPads().find(i) == KbMouseController::pressedPads().end()) return;
	KbMouseController::pressedPads().erase(KbMouseController::pressedPads().find(i));
	*/
	if (csn.compare("loadasound") == 0) return;

	auto lTrk = track.lock();
	auto note = lTrk->getBusNumber() > 0 ? program.lock()->getPad(i + (bank_ * 16))->getNote() : i + (bank_ * 16) + 35;
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
	overDubPressed = false;
    init();
   //getLedPanel().lock()->setOverDub(sequencer.lock()->isOverDubbing());
}

void GlobalReleaseControls::rec()
{
    recPressed = false;
    init();
    //mainFrame.lock()->getLedPanel().lock()->setRec(sequencer.lock()->isRecording());
}

void GlobalReleaseControls::tap()
{
    tapPressed = false;
	auto lSequencer = sequencer.lock();
    if (lSequencer->isRecordingOrOverdubbing())
        lSequencer->flushTrackNoteCache();
}

void GlobalReleaseControls::shift()
{
    shiftPressed = false;
}

void GlobalReleaseControls::erase()
{
    erasePressed = false;
}

GlobalReleaseControls::~GlobalReleaseControls() {
}
