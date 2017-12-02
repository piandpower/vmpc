#include <file/all/AllLoader.hpp>

#include <Mpc.hpp>
#include <file/all/AllParser.hpp>
#include <file/all/Bar.hpp>
#include <file/all/BarList.hpp>
#include <file/all/Count.hpp>
#include <file/all/Defaults.hpp>
#include <file/all/MidiInput.hpp>
#include <file/all/MidiSyncMisc.hpp>
#include <file/all/Misc.hpp>
#include <file/all/AllSequence.hpp>
#include <file/all/SequenceNames.hpp>
#include <file/all/AllSequencer.hpp>
#include <file/all/AllSong.hpp>
#include <file/all/Tracks.hpp>
#include <StartUp.hpp>
#include <ui/UserDefaults.hpp>
#include <ui/midisync/MidiSyncGui.hpp>
#include <ui/sequencer/SongGui.hpp>
#include <ui/sequencer/StepEditorGui.hpp>
#include <ui/sequencer/window/MultiRecordingSetupLine.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <sequencer/Event.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/Sequencer.hpp>
#include <sequencer/Song.hpp>
#include <sequencer/TempoChangeEvent.hpp>

#include <thirdp/bcmath/bcmath_stl.h>

using namespace mpc::file::all;
using namespace std;

AllLoader::AllLoader(mpc::disk::MpcFile* file) 
{
	allParser = new AllParser(file);
	sequencesALL = allParser->getAllSequences();
	auto allSeqNames = allParser->getSeqNames()->getNames();
	vector<Sequence*> temp;
	int counter = 0;
	for (int i = 0; i < 99; i++) {
		if (allSeqNames.at(i).find("(Unused)") != string::npos) {
			temp.push_back(nullptr);
		}
		else {
			temp.push_back(sequencesALL[counter++]);
		}
	}
	sequencesALL = temp;
	convertSequences(true);
}

AllLoader::AllLoader(mpc::Mpc* mpc, mpc::disk::MpcFile* file)
{
	this->mpc = mpc;
	auto lSequencer = mpc->getSequencer().lock();
	allParser = new AllParser(file);
	sequencesALL = allParser->getAllSequences();
	auto defaults = allParser->getDefaults();
	auto ud = mpc::StartUp::getUserDefaults().lock();
	ud->setLastBar(defaults->getBarCount() - 1);
	ud->setBus(defaults->getBusses()[0]);
	for (int i = 0; i < 33; i++)
		ud->setDeviceName(i, defaults->getDefaultDevNames()[i]);

	ud->setSequenceName(defaults->getDefaultSeqName());
	auto defTrackNames = defaults->getDefaultTrackNames();
	for (int i = 0; i < 64; i++)
		ud->setTrackName(i, defTrackNames[i]);

	ud->setDeviceNumber(defaults->getDevices()[0]);
	ud->setTimeSig(defaults->getTimeSigNum(), defaults->getTimeSigDen());
	ud->setPgm(defaults->getPgms()[0]);
	ud->setTempo(BCMath(defaults->getTempo() / 10.0));
	ud->setVelo(defaults->getTrVelos()[0]);
	convertSequences(false);
	auto allSeqNames = allParser->getSeqNames()->getNames();
	auto sequencer = allParser->getSequencer();
	lSequencer->setActiveSequenceIndex(sequencer->sequence);
	lSequencer->setSelectedTrackIndex(sequencer->track);
	auto swGui = mpc->getUis().lock()->getSequencerWindowGui();
	swGui->setNoteValue(sequencer->tc);
	auto count = allParser->getCount();
	swGui->setCountIn(count->getCountInMode());
	swGui->setAccentVelo(count->getAccentVelo());
	swGui->setNormalVelo(count->getNormalVelo());
	swGui->setClickOutput(count->getClickOutput());
	swGui->setClickVolume(count->getClickVolume());
	swGui->setRate(count->getRate());
	swGui->setMetronomeSound(count->getSound());
	swGui->setInPlay(count->isEnabledInPlay());
	swGui->setInRec(count->isEnabledInRec());
	swGui->setWaitForKey(count->isWaitForKeyEnabled());
	lSequencer->setCountEnabled(count->isEnabled());
	auto midiInput = allParser->getMidiInput();
	swGui->setReceiveCh(midiInput->getReceiveCh());
	swGui->setFilterType(midiInput->getFilterType());
	auto trackDests = midiInput->getMultiRecTrackDests();
	for (int i = 0; i < trackDests.size(); i++)
		swGui->getMrsLines()[i]->setTrack(trackDests[i]);

	swGui->setChPressurePassEnabled(midiInput->isChPressurePassEnabled());
	swGui->setExclusivePassEnabled(midiInput->isExclusivePassEnabled());
	swGui->setMidiFilterEnabled(midiInput->isFilterEnabled());
	lSequencer->setRecordingModeMulti(midiInput->isMultiRecEnabled());
	swGui->setNotePassEnabled(midiInput->isNotePassEnabled());
	swGui->setPgmChangePassEnabled(midiInput->isPgmChangePassEnabled());
	swGui->setPitchBendPassEnabled(midiInput->isPitchBendPassEnabled());
	swGui->setPolyPressurePassEnabled(midiInput->isPolyPressurePassEnabled());
	swGui->setSustainPedalToDuration(midiInput->isSustainPedalToDurationEnabled());
	auto misc = allParser->getMisc();
	auto midiSyncMisc = allParser->getMidiSync();
	auto seGui = mpc->getUis().lock()->getStepEditorGui();
	seGui->setAutoStepIncrementEnabled(misc->isAutoStepIncEnabled());
	seGui->setTcValueRecordedNotes(misc->getDurationTcPercentage());
	seGui->setDurationOfRecordedNotes(misc->isDurationOfRecNotesTc());
	auto msGui = mpc->getUis().lock()->getMidiSyncGui();
	msGui->setReceiveMMCEnabled(misc->isInReceiveMMCEnabled());
	msGui->setSendMMCEnabled(midiSyncMisc->isSendMMCEnabled());
	msGui->setModeIn(midiSyncMisc->getInMode());
	msGui->setModeOut(midiSyncMisc->getOutMode());
	msGui->setShiftEarly(midiSyncMisc->getShiftEarly());
	msGui->setFrameRate(midiSyncMisc->getFrameRate());
	msGui->setIn(midiSyncMisc->getInput());
	msGui->setOut(midiSyncMisc->getOutput());
	lSequencer->setSecondSequenceEnabled(sequencer->secondSeqEnabled);
	lSequencer->setSecondSequenceIndex(sequencer->secondSeqIndex);
	mpc->getUis().lock()->getSongGui()->setDefaultSongName(midiSyncMisc->getDefSongName());
	swGui->setTapAvg(misc->getTapAvg());
	auto songs = allParser->getSongs();
	for (int i = 0; i < 20; i++)
		lSequencer->getSong(i).lock()->setName(songs[i]->name);

}

void AllLoader::convertSequences(const bool indiv)
{
	auto counter = 0;
	int index = -1;
	if (!indiv) mpc->getSequencer().lock()->purgeAllSequences();
	for (auto& as : sequencesALL) {
		index++;
		if (as == nullptr) {
			if (indiv) mpcSequences.push_back(nullptr);
			continue;
		}
		shared_ptr<mpc::sequencer::Sequence> mpcSeq;
		if (indiv) {
			mpcSeq = make_shared<mpc::sequencer::Sequence>(mpc, mpc->getSequencer().lock()->getDefaultTrackNames());
		}
		else {
			mpcSeq = mpc->getSequencer().lock()->getSequence(index).lock();
		}
		mpcSeq->init(as->barCount - 1);
		for (int i = 0; i < as->barCount; i++) {
			auto num = dynamic_cast<Bar*>(as->barList->getBars()[i])->getNumerator();
			auto den = dynamic_cast<Bar*>(as->barList->getBars()[i])->getDenominator();
			mpcSeq->setTimeSignature(i, num, den);
		}
		mpcSeq->setName(as->name);
		mpcSeq->setInitialTempo(as->tempo);
		auto at = as->tracks;
		for (int i = 0; i < 64; i++) {
			auto t = mpcSeq->getTrack(i).lock();
			t->setUsed(at->getStatus(i) != 6);
			t->setName(at->getName(i));
			t->setBusNumber(at->getBus(i));
			t->setProgramChange(at->getPgm(i));
			t->setOn(at->getStatus(i) != 5);
			t->setVelocityRatio(at->getVelo(i));
		}
		for (int j = 0; j < as->getEventAmount(); j++) {
			auto e = as->allEvents[j];
			if (e == nullptr) continue;
			int track = e->getTrack();
			if (track > 128) track -= 128;
			if (track < 0) track += 128;
			if (track > 63) track -= 64;
			mpcSeq->getTrack(track).lock()->cloneEvent(shared_ptr<mpc::sequencer::Event>(e));
		}
		for (int i = 0; i < 32; i++)
			mpcSeq->setDeviceName(i, as->devNames[i]);

		mpcSeq->initMetaTracks();
		mpcSeq->setFirstLoopBar(as->loopFirst);
		mpcSeq->setLastLoopBar(as->loopLast);
		mpcSeq->setLastLoopBar(as->loopLast);
		if (as->loopLastEnd)
			mpcSeq->setLastLoopBar(INT_MAX);

		mpcSeq->setLoopEnabled(as->loop);
		if (indiv) {
			mpcSequences.push_back(mpcSeq);
		}
	}
}

vector<shared_ptr<mpc::sequencer::Sequence>>* AllLoader::getSequences()
{
    return &mpcSequences;
}
