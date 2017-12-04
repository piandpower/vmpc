#include "Sequencer.hpp"

#include <Mpc.hpp>
#include <ui/Uis.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <StartUp.hpp>
#include <ui/UserDefaults.hpp>
#include <lcdgui/LayeredScreen.hpp>

#include <audiomidi/AudioMidiServices.hpp>

#include <sequencer/Event.hpp>
#include <sequencer/TempoChangeEvent.hpp>
#include <sequencer/FrameSeq.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/TimeSignature.hpp>
#include <sequencer/Song.hpp>
#include <sequencer/Step.hpp>

#include <sampler/Sampler.hpp>

// ctoot
#include <audio/server/NonRealTimeAudioServer.hpp>

// moduru
#include <System.hpp>
#include <lang/StrUtil.hpp>

#include <Logger.hpp>

using namespace mpc::sequencer;
using namespace std;

Sequencer::Sequencer(mpc::Mpc* mpc) 
{
	this->mpc = mpc;
}

void Sequencer::init()
{
	TICK_VALUES = vector<int>{ 0, 48, 32, 24, 16, 12, 8 };
	repeats = 0;
	sequences = vector<shared_ptr<Sequence>>(99);
	taps = make_unique<moduru::io::CircularIntBuffer>(4, true, true);
	reposition = -1;
	nextsq = -1;
	previousTempo = BCMath("0.0");
	auto userDefaults = mpc::StartUp::getUserDefaults().lock();
	defaultSequenceName = moduru::lang::StrUtil::trim(userDefaults->getSequenceName());
	for (int i = 0; i < 64; i++) {
		string name = "Track-";
		name = name.append(moduru::lang::StrUtil::padLeft(to_string(i + 1), "0", 2));
		defaultTrackNames.push_back(name);
	}

	activeTrackIndex = 0;
	songs = vector<shared_ptr<Song>>(20);
	defaultDeviceNames = vector<string>(33);
	for (int i = 0; i < 33; i++)
		defaultDeviceNames[i] = userDefaults->getDeviceName(i);

	recordingModeMulti = userDefaults->isRecordingModeMulti();
	soloEnabled = false;
	tempoSourceSequence = true;
	countEnabled = true;
	recording = false;
	tempo = userDefaults->getTempo();
	tempo = BCMath("120.0");

	metronomeOnly = false;
	metronomeSeq = nullptr;
	activeSequenceIndex = 0;
	currentlyPlayingSequenceIndex = 0;
	songMode = false;


	//mpc->getAudioMidiServices().lock()->getFrameSequencer().lock()->setGui(gui);
	purgeAllSequences();
	for (int i = 0; i < 20; i++) {
		songs[i] = make_shared<Song>(this);
		songs[i]->setName("Song" + moduru::lang::StrUtil::padLeft(to_string(i + 1), "0", 2));
	}
}

int Sequencer::getActiveSequenceIndex()
{
	return activeSequenceIndex;
}

void Sequencer::playToTick(int targetTick)
{
	auto seqIndex = songMode ? getSongSequenceIndex() : currentlyPlayingSequenceIndex;
	auto tc = metronomeOnly ? metronomeSeq.get() : sequences[seqIndex].get();
	for (auto& trk : tc->getTracks()) {
		auto lTrk = trk.lock();
		while (lTrk->getNextTick() <= targetTick) {
			lTrk->playNext();
		}
	}
	for (auto& trk : tc->getMetaTracks()) {
		auto lTrk = trk.lock();
		while (lTrk->getNextTick() <= targetTick) {
			lTrk->playNext();
		}
	}
}

vector<int> Sequencer::getTickValues() {
	return TICK_VALUES;
}

int Sequencer::repeats;
bool Sequencer::endOfSong = false;

void Sequencer::setTempo(BCMath i)
{
	if (i.toDouble() < 30.0 || i.toDouble() > 300.0) return;
	auto s = getActiveSequence().lock();
	auto tce = getCurrentTempoChangeEvent().lock();
	if (tempoSourceSequence) {
		if (tce->getTick() == 0) {
			s->setInitialTempo(i);
		}
		else {
			auto initialTempo = s->getInitialTempo();
			auto ratio = i.toDouble() / initialTempo.toDouble();
			tce->setRatio((int)(ratio * 1000.0));
		}
	}
	else {
		tempo = i;
	}
	setChanged();
	notifyObservers(string("tempo"));
}

BCMath Sequencer::getTempo()
{
	if (!isPlaying() && !getActiveSequence().lock()->isUsed()) {
		return tempo;
	}
	if (tempoSourceSequence) {
		auto tce = getCurrentTempoChangeEvent().lock();
		if (tce) {
			return tce->getTempo();
		}
		return getActiveSequence().lock()->getInitialTempo();
	}
	return tempo;
}

weak_ptr<TempoChangeEvent> Sequencer::getCurrentTempoChangeEvent()
{
	auto index = -1;
	auto s = getActiveSequence().lock();
	for (auto& tce : s->getTempoChangeEvents()) {
		auto lTce = tce.lock();
		if (getTickPosition() >= lTce->getTick()) {
			index++;
		}
		else {
			break;
		}
	}
	if (index == -1) index++;
	return s->getTempoChangeEvents()[index];
}

bool Sequencer::isTempoSourceSequence()
{
    return tempoSourceSequence;
}

void Sequencer::setTempoSourceSequence(bool b)
{
	tempoSourceSequence = b;
	setChanged();
	notifyObservers(string("temposource"));
	setChanged();
	notifyObservers(string("tempo"));
}

bool Sequencer::isRecordingOrOverdubbing()
{
    return recording || overdubbing;
}

bool Sequencer::isRecording()
{
    return recording;
}

bool Sequencer::isSoloEnabled()
{
    return soloEnabled;
}

void Sequencer::setSoloEnabled(bool b)
{
    soloEnabled = b;
    setChanged();
    notifyObservers(string("soloenabled"));
}

weak_ptr<Sequence> Sequencer::getSequence(int i)
{
    return sequences[i];
}

string Sequencer::getDefaultSongName()
{
    return defaultSongName;
}

void Sequencer::setDefaultSongName(string s)
{
    defaultSongName = s;
}

string Sequencer::getDefaultSequenceName()
{
	return defaultSequenceName;
}

void Sequencer::setDefaultSequenceName(string s)
{
    defaultSequenceName = s;
}

void Sequencer::setActiveSequenceIndex(int i)
{
	if (i < 0 || i > 98) return;

	activeSequenceIndex = i;
	if (!isPlaying()) {
		position = 0;
		setChanged();
		notifyObservers(string("now"));
	}
	setChanged();
	notifyObservers(string("seqnumbername"));
	setChanged();
	notifyObservers(string("timesignature"));
	setChanged();
	notifyObservers(string("numberofbars"));
	setChanged();
	notifyObservers(string("tempo"));
	notifyTrack();
}

string Sequencer::getDefaultDeviceName(int i)
{
    return defaultDeviceNames[i];
}

void Sequencer::setDefaultDeviceName(int i, string s)
{
	defaultDeviceNames[i] = s;
}

bool Sequencer::isCountEnabled()
{
    return countEnabled;
}

void Sequencer::setCountEnabled(bool b)
{
    countEnabled = b;
    setChanged();
    notifyObservers(string("count"));
}

void Sequencer::setTimeDisplayStyle(int i)
{
    timeDisplayStyle = i;
}

int Sequencer::getTimeDisplayStyle()
{
    return timeDisplayStyle;
}

void Sequencer::setRecordingModeMulti(bool b)
{
    recordingModeMulti = b;
    setChanged();
    notifyObservers(string("recordingmode"));
}

void Sequencer::setFrameRate(int i)
{
    frameRate = i;
}

int Sequencer::getFrameRate()
{
    return frameRate;
}

bool Sequencer::isRecordingModeMulti()
{
    return recordingModeMulti;
}

int Sequencer::getActiveTrackIndex()
{
    return activeTrackIndex;
}

void Sequencer::trackUp()
{
    if(activeTrackIndex == 63) return;
    activeTrackIndex++;
    notifyTrack();
}

void Sequencer::trackDown()
{
    if(activeTrackIndex == 0) return;
    activeTrackIndex--;
    notifyTrack();
}

bool Sequencer::isPlaying()
{
	auto lAms = mpc->getAudioMidiServices().lock();
	if (lAms->isDisabled() || !lAms->getFrameSequencer().lock())
		return false;

	return lAms->getFrameSequencer().lock()->isRunning();
}

void Sequencer::play(bool fromStart)
{
    if(isPlaying()) return;
	//    endOfSong = false;
    repeats = 0;
	//auto currentSong = songs[lGui->getSongGui()->getSelectedSongIndex()];
    Step* currentStep = nullptr;
	/*
	if (songMode) {
		if (!currentSong->isUsed()) return;

		if (fromStart)
			lGui->getSongGui()->setOffset(-1);

		if (lGui->getSongGui()->getOffset() + 1 > currentSong->getStepAmount() - 1)
			return;

		int step = lGui->getSongGui()->getOffset() + 1;
		if (step > currentSong->getStepAmount())
			step = currentSong->getStepAmount() - 1;

		currentStep = currentSong->getStep(step);
	}
	*/
    move(position);
    currentlyPlayingSequenceIndex = activeSequenceIndex;

	//auto swGui = lGui->getSequencerWindowGui();


    //if(!countEnabled || swGui->getCountInMode() == 0 || (swGui->getCountInMode() == 1 && recording == false)) {
        if(fromStart)
            move(0);

    //}
	auto s = getActiveSequence().lock();
	/*
	if (countEnabled && !songMode) {
        if(swGui->getCountInMode() == 2 || (swGui->getCountInMode() == 1 && recording == true)) {
            move(s->getLoopStart());
            startCountingIn();
        }
    }
	*/

	//auto lMainFrame = lGui->getMainFrame().lock();
	//auto ledPanel = lMainFrame->getLedPanel().lock();
	if (!songMode) {
		if (!s->isUsed())
			return;

		s->initLoop();
		if (recording || overdubbing) {
			undoPlaceHolder = copySequence(s);
			lastRecordingActive = true;
			recordStartTick = getTickPosition();
			//ledPanel->setUndoSeq(lastRecordingActive);
		}
	}
	//ledPanel->setPlay(true);
	auto lAms = mpc->getAudioMidiServices().lock();
	lAms->getFrameSequencer().lock()->start();
	if (lAms->isBouncePrepared()) {
		lAms->startBouncing();
	}
	else {
		lAms->getOfflineServer()->setRealTime(true);
	}
    setChanged();
    notifyObservers(string("play"));
}

void Sequencer::undoSeq()
{
	if (isPlaying()) return;
	if (undoPlaceHolder == nullptr) return;

	auto s = copySequence(undoPlaceHolder);
	undoPlaceHolder = copySequence(sequences[activeSequenceIndex]);
	sequences[activeSequenceIndex] = s;
	lastRecordingActive = !lastRecordingActive;
	//	//lGui->getMainFrame().lock()->getLedPanel().lock()->setUndoSeq(lastRecordingActive);
}

void Sequencer::clearUndoSeq()
{
    if(isPlaying()) return;
    undoPlaceHolder = nullptr;
    lastRecordingActive = false;
	//	//lGui->getMainFrame().lock()->getLedPanel().lock()->setUndoSeq(false);
}

void Sequencer::playFromStart()
{
	if (isPlaying()) return;
	play(true);
}

void Sequencer::play()
{
	if (isPlaying()) return;
	play(false);
}

void Sequencer::rec()
{
	if (isPlaying()) return;
	recording = true;
	play(false);
}

void Sequencer::recFromStart()
{
	if (isPlaying()) return;
	recording = true;
	play(true);
}

void Sequencer::overdub()
{
	if (isPlaying()) return;
	overdubbing = true;
	play(false);
}

void Sequencer::switchRecordToOverDub()
{
	if (!isRecording()) return;
	recording = false;
	overdubbing = true;
	//	//auto lMainFrame = lGui->getMainFrame().lock();
	//auto ledPanel = lMainFrame->getLedPanel().lock();
	//ledPanel->setOverDub(true);
	//ledPanel->setRec(false);
}

void Sequencer::overdubFromStart()
{
	if (isPlaying()) return;
	overdubbing = true;
	play(true);
}

void Sequencer::stop(){
	stop(-1);
}

void Sequencer::stop(int tick)
{
    if(!isPlaying()) {
        if(position != 0)
            setBar(0);
        return;
    }

    //mpc->getEventHandler()->handle(MidiClockEvent(ctoot::midi::core::ShortMessage::STOP), Track(mpc, 999));
	auto s1 = getActiveSequence().lock();
	auto s2 = getCurrentlyPlayingSequence().lock();
	auto pos = getTickPosition();
	if (pos > s1->getLastTick())
		pos = s1->getLastTick();
	auto lAms = mpc->getAudioMidiServices().lock();
	int frameOffset = tick == -1 ? 0 : lAms->getFrameSequencer().lock()->getEventFrameOffset(tick);
	lAms->getFrameSequencer().lock()->stop();
    if (recording || overdubbing)
        s2->getTrack(activeTrackIndex).lock()->correctTimeRange(0, s2->getLastTick(), TICK_VALUES[mpc->getUis().lock()->getSequencerWindowGui()->getNoteValue()]);

    auto notifynextsq = false;
	if (nextsq != -1) {
		notifynextsq = true;
		nextsq = -1;
		mpc->getLayeredScreen().lock()->setFocus("sq");
	}
    recording = false;
    overdubbing = false;
    
    move(pos);
	if (!lAms->isBouncing()) mpc->getSampler().lock()->stopAllVoices(frameOffset);
    if(notifynextsq) {
        setChanged();
        notifyObservers(string("nextsqoff"));
    }
    notifyTimeDisplay();
    if(endOfSong) {
        //invokeLater(Sequencer_stop_1(this));
    }
	if (lAms->isBouncing()) {
		if (stopBounceThread.joinable()) stopBounceThread.join();
		stopBounceThread = thread(&Sequencer::static_stopBounce, this);
	}
	/*
	auto ledPanel = lMainFrame->getLedPanel().lock();
	ledPanel->setOverDub(false);
	ledPanel->setPlay(false);
	ledPanel->setRec(false);
	*/
    setChanged();
    notifyObservers(string("stop"));
}

void Sequencer::static_stopBounce(void * args)
{
	static_cast<Sequencer*>(args)->runStopBounceThread();
}

void Sequencer::runStopBounceThread() {
	this_thread::sleep_for(chrono::milliseconds(100));
	auto lAms = mpc->getAudioMidiServices().lock();
	//lAms->stopBouncing();
	//lAms->getOfflineServer()->setRealTime(true);
}

bool Sequencer::isCountingIn()
{
    return countingIn;
}

void Sequencer::setCountingIn(bool b)
{
    countingIn = b;
}

void Sequencer::startCountingIn()
{
    countingIn = true;
}

void Sequencer::notifyTrack()
{
    setChanged();
    notifyObservers(string("tracknumbername"));
    setChanged();
    notifyObservers(string("trackon"));
    setChanged();
    notifyObservers(string("programchange"));
    setChanged();
    notifyObservers(string("velocityratio"));
    setChanged();
    notifyObservers(string("tracktype"));
    setChanged();
    notifyObservers(string("device"));
    setChanged();
    notifyObservers(string("devicename"));
}

void Sequencer::setSequence(int i, shared_ptr<Sequence> s)
{
	sequences[i].swap(s);
}

void Sequencer::purgeAllSequences()
{
	for (int i = 0; i < 99; i++) {
		purgeSequence(i);
	}
    activeSequenceIndex = 0;
}

void Sequencer::purgeSequence(int i) {
	sequences[i].reset();
	sequences[i] = make_shared<Sequence>(mpc, defaultTrackNames);
	string res = defaultSequenceName;
	res.append(moduru::lang::StrUtil::padLeft(to_string(i + 1), "0", 2));
	sequences[i]->setName(res);
}

void Sequencer::copySequence(int sq0, int sq1)
{
	sequences[sq1] = copySequence(sequences[sq0]);
    sequences[sq1]->initLoop();
}

shared_ptr<Sequence> Sequencer::copySequence(weak_ptr<Sequence> src)
{
	auto source = src.lock();
	auto copy = make_shared<Sequence>(mpc, defaultTrackNames);
	copy->init(source->getLastBar());
	copySequenceParameters(source, copy);
	for (int i = 0; i < 64; i++) {
		copyTrack(source->getTrack(i), copy->getTrack(i));
	}
	return copy;
}

void Sequencer::copySequenceParameters(int sq0, int sq1)
{
    copySequenceParameters(sequences[sq0], sequences[sq1]);
}


void Sequencer::copySequenceParameters(weak_ptr<Sequence> src, weak_ptr<Sequence> dst)
{
	auto source = src.lock();
	auto dest = dst.lock();
    dest->setName(source->getName());
    //dest->setTempoChangeEvents(source->getTempoChangeEvents());
	copyTempoChangeEvents(src, dst);
    dest->setLoopEnabled(source->isLoopEnabled());
    dest->setUsed(source->isUsed());
    dest->setDeviceNames(source->getDeviceNames());
}

void Sequencer::copyTempoChangeEvents(weak_ptr<Sequence> src, weak_ptr<Sequence> dst) {
	auto srcList = src.lock()->getTempoChangeEvents();
	
}

void Sequencer::copyTrack(int tr0, int tr1, int sq0, int sq1)
{
	auto src = sequences[sq0]->getTrack(tr0).lock();
	auto dest = sequences[sq1]->purgeTrack(tr1).lock();
	copyTrack(src, dest);
}

void Sequencer::copyTrack(weak_ptr<Track> src, weak_ptr<Track> dest)
{
	auto lSrc = src.lock();
	auto lDest = dest.lock();
	lDest->setTrackIndex(lSrc->getTrackIndex());
	for (auto& e : lSrc->getEvents()) {
		lDest->cloneEvent(e);
	}
	copyTrackParameters(src, dest);
}

void Sequencer::copyTrackParameters(int tr0, int tr1, int sq0, int sq1)
{
	copyTrackParameters(sequences[sq0]->getTrack(tr0), sequences[sq1]->getTrack(tr1));
}

void Sequencer::copyTrackParameters(weak_ptr<Track> source, weak_ptr<Track> dest)
{
	auto lSrc = source.lock();
	auto lDest = dest.lock();
	lDest->setUsed(lSrc->isUsed());
    lDest->setOn(lSrc->isOn());
    lDest->setDeviceNumber(lSrc->getDevice());
    lDest->setBusNumber(lSrc->getBusNumber());
    lDest->setVelocityRatio(lSrc->getVelocityRatio());
    lDest->setProgramChange(lSrc->getProgramChange());
    lDest->setName(lSrc->getName());
}

string Sequencer::getDefaultTrackName(int i)
{
	return defaultTrackNames[i];
}

vector<string> Sequencer::getDefaultTrackNames() {
	return defaultTrackNames;
}

void Sequencer::setDefaultTrackName(string s, int i)
{
	defaultTrackNames[i] = s;
}

int Sequencer::getCurrentBarNumber()
{
    auto s = isPlaying() ? getCurrentlyPlayingSequence().lock() : getActiveSequence().lock();
    auto pos = getTickPosition();
	if(pos == s->getLastTick()) return s->getLastBar() + 1;
	auto index = pos;
	if(isPlaying() && !countingIn) index = getTickPosition();
    if(index == 0) return 0;

    auto barLengths = s->getBarLengths();
    auto barCounter = 0;

    int tickCounter = 0;
	for (int i = 0; i < 999; i++) {
		if (i > s->getLastBar()) i = 0;
		tickCounter += (*barLengths)[i];
		if (tickCounter > index) {
			barCounter = i;
			break;
		}
	}
    return barCounter;
}

int Sequencer::getCurrentBeatNumber()
{
    auto s = isPlaying() ? getCurrentlyPlayingSequence().lock() : getActiveSequence().lock();
    auto pos = getTickPosition();
    if(pos == s->getLastTick()) return 0;
    auto index = pos;
	if (isPlaying() && !countingIn) {
		index = getTickPosition();
		if (index > s->getLastTick()) {
			index %= s->getLastTick();
		}
	}
    
	auto ts = s->getTimeSignature();
	auto den = ts.getDenominator();
	auto denTicks = 96 * (4.0 / den);
    
	if(index == 0) return 0;

    int barStartPos = 0;
    auto barCounter = 0;
	for (auto l : *s->getBarLengths()) {
		if (barCounter == getCurrentBarNumber()) break;
		barStartPos += l;
		barCounter++;
	}
	auto beatCounter = (int)(floor((index - barStartPos) / denTicks));
	return beatCounter;
}

int Sequencer::getCurrentClockNumber()
{
	auto s = isPlaying() ? getCurrentlyPlayingSequence().lock() : getActiveSequence().lock();

	int pos = getTickPosition();
	if (pos == s->getLastTick()) {
		return 0;
	}
	int index = pos;
	if (isPlaying() && !countingIn) {
		if (index > s->getLastTick()) {
			index %= s->getLastTick();
		}
	}
	auto ts = s->getTimeSignature();
	auto den = ts.getDenominator();
	auto denTicks = 96 * (4.0 / den);

	if (index == 0) {
		return index;
	}

	auto barCounter = 0;
	int clock = index;

	for (auto l : *s->getBarLengths()) {
		if (barCounter == getCurrentBarNumber()) break;
		clock -= l;
		barCounter++;
	}
	for (int i = 0; i < getCurrentBeatNumber(); i++) {
		clock -= denTicks;
	}
	return clock;
}

void Sequencer::setBar(int i)
{
	if (isPlaying()) return;

	if (i < 0) return;

	auto s = getActiveSequence().lock();
	if (i > s->getLastBar() + 1) return;

	if (s->getLastBar() == 998 && i > 998) return;
	auto ts = s->getTimeSignature();
	auto den = ts.getDenominator();
	auto denTicks = (int)(96 * (4.0 / den));
	if (i != s->getLastBar() + 1) {
		ts.setNumerator(s->getNumerator(i));
		ts.setDenominator(s->getDenominator(i));
	}
	auto barLengths = s->getBarLengths();
	auto currentClock = getCurrentClockNumber();
	auto currentBeat = getCurrentBeatNumber();
	int pos = 0;
	auto barCounter = 0;
	for (auto l : *barLengths) {
		if (barCounter == i) break;
		pos += l;
		barCounter++;
	}
	pos += currentBeat * denTicks;
	pos += currentClock;
	if (pos > s->getLastTick()) pos = s->getLastTick();

	move(pos);
	setChanged();
	notifyObservers(string("timesignature"));
	setBeat(0);
	setClock(0);
}

void Sequencer::setBeat(int i)
{
	if (i < 0 || isPlaying()) {
		return;
	}

	auto s = getActiveSequence().lock();
	auto pos = getTickPosition();
	if (pos == s->getLastTick()) {
		return;
	}
	auto ts = s->getTimeSignature();
	auto difference = i - getCurrentBeatNumber();
	auto num = ts.getNumerator();
	if (i >= num) {
		return;
	}

	auto den = ts.getDenominator();
	auto denTicks = 96 * (4.0 / den);
	pos += difference * denTicks;
	move(pos);
}

void Sequencer::setClock(int i)
{
	if (i < 0 || isPlaying()) {
		return;
	}

	auto s = getActiveSequence().lock();
	int pos = getTickPosition();
	if (pos == s->getLastTick()) {
		return;
	}
	getCurrentClockNumber();
	int difference = i - getCurrentClockNumber();
	auto den = s->getTimeSignature().getDenominator();
	auto denTicks = 96 * (4.0 / den);
	if (i > denTicks - 1) {
		return;
	}

	if (pos + difference > s->getLastTick()) {
		return;
	}

	pos += difference;
	move(pos);
}

int Sequencer::getLoopEnd()
{
    return getActiveSequence().lock()->getLoopEnd();
}


weak_ptr<Sequence> Sequencer::getActiveSequence()
{
//	//	if (songMode && songs[lGui->getSongGui()->getSelectedSongIndex()]->getStepAmount() != 0)
//		return sequences[getSongSequenceIndex() >= 0 ? getSongSequenceIndex() : activeSequenceIndex];
	return sequences[activeSequenceIndex];
}

int Sequencer::getUsedSequenceCount()
{
    return getUsedSequences().size();
}

vector<weak_ptr<Sequence>> Sequencer::getUsedSequences()
{
	vector<weak_ptr<Sequence>> usedSeqs;
	for (auto s : sequences)
		if (s->isUsed())
			usedSeqs.push_back(s);
    return usedSeqs;
}

vector<int> Sequencer::getUsedSequenceIndexes()
{
	vector<int> usedSeqs;
	for (int i = 0; i < 99; i++) {
        auto s = sequences[i];
		if (s->isUsed())
			usedSeqs.push_back(i);
    }
	return usedSeqs;
}

void Sequencer::goToPreviousEvent()
{
	auto t = getActiveSequence().lock()->getTrack(getActiveTrackIndex()).lock();
	if(t->getEventIndex() == 0) {
		setBar(0);
		return;
	}
	if(t->getEventIndex() + 1 >= t->getEvents().size() && t->getEvent((int)(t->getEvents().size()) - 1).lock()->getTick() < position) {
		t->setEventIndex((int)(t->getEvents().size()) - 1);
		move(t->getEvent(t->getEventIndex()).lock()->getTick());
		return;
	}
	shared_ptr<Event> event;
	shared_ptr<Event> prev;
	while (t->getEventIndex() > 0) {
		event = t->getEvent(t->getEventIndex()).lock();
		prev = t->getEvent(t->getEventIndex() - 1).lock();
		if(prev->getTick() == event->getTick()) {
			t->setEventIndex(t->getEventIndex() - 1);
		} else {
			break;
		}
	}
	t->setEventIndex(t->getEventIndex() - 1);
	while (t->getEventIndex() > 0) {
		event = t->getEvent(t->getEventIndex()).lock();
		prev = t->getEvent(t->getEventIndex() - 1).lock();
		if(prev->getTick() == event->getTick()) {
			t->setEventIndex(t->getEventIndex() - 1);
		} else {
			break;
		}
	}
	move(t->getEvents()[t->getEventIndex()].lock()->getTick());
}

void Sequencer::goToNextEvent()
{
	auto s = getActiveSequence().lock();
	auto t = s->getTrack(getActiveTrackIndex()).lock();
	if (t->getEvents().size() == 0) {
		if (position != s->getLastTick()) {
			move(s->getLastTick());
		}
		return;
	}
	const int eventCount = t->getEvents().size();
	if (position == s->getLastTick())
		return;

	if (t->getEventIndex() >= eventCount - 1 && position >= t->getEvent(eventCount - 1).lock()->getTick()) {
		move(s->getLastTick());
		return;
	}
	if (t->getEvent(t->getEventIndex()).lock()->getTick() > position) {
		move(t->getEvent(t->getEventIndex()).lock()->getTick());
		return;
	}
	shared_ptr<Event> event;
	shared_ptr<Event> next;
	if (t->getEvent(t->getEventIndex()).lock()->getTick() == position) {
		while (t->getEventIndex() < eventCount - 2) {
			event = t->getEvent(t->getEventIndex()).lock();
			next = t->getEvent(t->getEventIndex() + 1).lock();
			if (next->getTick() == event->getTick()) {
				t->setEventIndex(t->getEventIndex() + 1);
			}
			else {
				break;
			}
		}
	}
	t->setEventIndex(t->getEventIndex() + 1);
	while (t->getEventIndex() < eventCount - 2) {
		event = t->getEvent(t->getEventIndex()).lock();
		next = t->getEvent(t->getEventIndex() + 1).lock();
		if (next->getTick() == event->getTick()) {
			t->setEventIndex(t->getEventIndex() + 1);
		}
		else {
			break;
		}
	}
	move(t->getEvent(t->getEventIndex()).lock()->getTick());
}

void Sequencer::notifyTimeDisplay()
{
    setChanged();
	notifyObservers(string("bar"));
    setChanged();
    notifyObservers(string("beat"));
    setChanged();
	notifyObservers(string("clock"));
}

void Sequencer::goToPreviousStep()
{
	auto swGui = mpc->getUis().lock()->getSequencerWindowGui();
	auto stepSize = TICK_VALUES[swGui->getNoteValue()];
	auto pos = getTickPosition();
	auto stepAmt = static_cast<int>(ceil(getActiveSequence().lock()->getLastTick() / stepSize)) + 1;
	auto stepGrid = vector<int>(stepAmt);
	for (int i = 0; i < stepGrid.size(); i++)
		stepGrid[i] = i * stepSize;

	auto currentStep = 0;
	for (auto l : stepGrid) {
		if (pos <= l) break;
		currentStep++;
	}
	if (currentStep == 0)
		return;

	currentStep--;
	move(currentStep * stepSize);
}

void Sequencer::goToNextStep()
{
	auto swGui = mpc->getUis().lock()->getSequencerWindowGui();
	auto stepSize = TICK_VALUES[swGui->getNoteValue()];
	auto pos = getTickPosition();
	auto stepGrid = vector<int>(ceil(getActiveSequence().lock()->getLastTick() / stepSize));
	for (int i = 0; i < stepGrid.size(); i++)
		stepGrid[i] = i * stepSize;

	auto currentStep = -1;
	for (auto l : stepGrid) {
		if (pos < l) break;
		currentStep++;
	}
	if (currentStep == stepGrid.size())
		return;

	currentStep++;
	move(currentStep * stepSize);
}

void Sequencer::tap()
{
	if (isPlaying()) return;
	auto nanoLong = moduru::System::nanoTime();

	if (nanoLong - lastTap > (2000 * 1000000)) {
		//taps = make_unique<moduru::io::CircularIntBuffer>(4, true, true);
	}


	lastTap = nanoLong;
	//taps->write(vector<int>{ (int)nanoLong });
	int accum = 0;
	vector<long> tapsLong;
	//while (taps->availableRead() > 0)
	//	tapsLong.push_back(taps->read());

	for (int i = 0; i < (int)(tapsLong.size()) - 1; i++) {
		int l0 = tapsLong[i];
		int l1 = tapsLong[i + 1];
		accum += l1 - l0;
	}

	if (accum == 0) return;

	auto tempo = (60000.0 * 1000000.0) / (accum / ((int)(tapsLong.size()) - 1));
	tempo = floor(tempo * 10) / 10;
	setTempo(BCMath(tempo));
}

int Sequencer::getResolution()
{
    return 96;
}

void Sequencer::move(int tick)
{
	auto oldTick = getTickPosition();
	reposition = tick;
	position = tick;
	playStartTick = tick;
	auto s = isPlaying() ? getCurrentlyPlayingSequence().lock() : getActiveSequence().lock();

	if (!isPlaying() && songMode) {
		s = sequences[getSongSequenceIndex()];
	}

	if (s->isUsed()) {
		for (auto& t : s->getTracks()) {
			auto lTrk = t.lock();
			if (!lTrk->isUsed()) continue;
			lTrk->move(tick, oldTick);
		}
		for (auto& t : s->getMetaTracks()) {
			auto lTrk = t.lock();
			if (!lTrk->isUsed()) continue;
			lTrk->move(tick, oldTick);
		}
	}

	notifyTimeDisplay();
	if (getTempo().toDouble() != previousTempo.toDouble()) {
		previousTempo = getTempo();
		setChanged();
		notifyObservers(string("tempo"));
	}
}

int Sequencer::getTickPosition()
{
    if (isPlaying()) {
        return mpc->getAudioMidiServices().lock()->getFrameSequencer().lock()->getTickPosition();
    }
    return position;
}

weak_ptr<Sequence> Sequencer::getCurrentlyPlayingSequence()
{
	return sequences[getCurrentlyPlayingSequenceIndex()];
}

void Sequencer::setSelectedTrackIndex(int i)
{
    activeTrackIndex = i;
	setChanged();
	notifyObservers(string("selectedtrackindex"));
}

int Sequencer::getCurrentlyPlayingSequenceIndex()
{
	//	//auto songseq = songMode ? songs[lGui->getSongGui()->getSelectedSongIndex()]->getStep(lGui->getSongGui()->getOffset() + 1)->getSequence() : -1;
	//return songMode ? songseq : currentlyPlayingSequenceIndex;
	return getActiveSequenceIndex();
}

void Sequencer::setCurrentlyPlayingSequenceIndex(int i) {
	currentlyPlayingSequenceIndex = i;
	activeSequenceIndex = i;
}

int Sequencer::getNextSq()
{
    return nextsq;
}

int Sequencer::getFirstUsedSeqDown(int from)
{
	auto result = -1;
	for (int i = from; i >= 0; i--) {
		if (sequences[i]->isUsed()) {
			result = i;
			break;
		}
	}
	return result;
}

int Sequencer::getFirstUsedSeqUp(int from)
{
	auto result = -1;
	for (int i = from; i < 99; i++) {
		if (sequences[i]->isUsed()) {
			result = i;
			break;
		}
	}
	return result;
}

void Sequencer::resetNextSq() {
	nextsq = -1;
}

void Sequencer::setNextSq(int i)
{
	auto firstnotify = nextsq == -1;
	auto up = i > nextsq;
	if (firstnotify) up = i > currentlyPlayingSequenceIndex;

	auto result = up ? getFirstUsedSeqUp(i) : getFirstUsedSeqDown(i);

	if (result == -1)
		return;

	nextsq = result;
	setChanged();
	if (firstnotify) {
		notifyObservers(string("nextsq"));
	}
	else {
		notifyObservers(string("nextsqvalue"));
	}
}

void Sequencer::setNextSqPad(int i)
{
	if (!sequences[i]->isUsed()) {
		nextsq = -1;
		setChanged();
		notifyObservers(string("nextsqoff"));
		return;
	}
	auto firstnotify = nextsq == -1;
	nextsq = i;
	if (firstnotify) {
		setChanged();
		notifyObservers(string("nextsq"));
	}
	else {
		setChanged();
		notifyObservers(string("nextsqvalue"));
	}
}

weak_ptr<Song> Sequencer::getSong(int i)
{
    return songs[i];
}

bool Sequencer::isSongModeEnabled()
{
   return songMode;
}

void Sequencer::setSongModeEnabled(bool b)
{
    songMode = b;
}

int Sequencer::getSongSequenceIndex()
{
	//	//auto song = songs[lGui->getSongGui()->getSelectedSongIndex()];
	//auto step = lGui->getSongGui()->getOffset() + 1;
	//if (step > song->getStepAmount() - 1)
	//	step = song->getStepAmount() - 1;

	//return song->getStep(step)->getSequence();
	return 0;
}

bool Sequencer::isSecondSequenceEnabled()
{
    return secondSequenceEnabled;
}

void Sequencer::setSecondSequenceEnabled(bool b)
{
    secondSequenceEnabled = b;
}

int Sequencer::getSecondSequenceIndex()
{
    return secondSequenceIndex;
}

void Sequencer::setSecondSequenceIndex(int i)
{
    secondSequenceIndex = i;
}

void Sequencer::flushTrackNoteCache()
{
	for (auto& t : getCurrentlyPlayingSequence().lock()->getTracks()) {
		t.lock()->flushNoteCache();
	}
}

void Sequencer::storeActiveSequenceInPlaceHolder()
{
	undoPlaceHolder = copySequence(sequences[activeSequenceIndex]);
	lastRecordingActive = true;
	//	//lGui->getMainFrame().lock()->getLedPanel().lock()->setUndoSeq(lastRecordingActive);
}

bool Sequencer::isOverDubbing()
{
    return overdubbing;
}

int Sequencer::getPlayStartTick()
{
    return playStartTick;
}

void Sequencer::notify(string s)
{
	setChanged();
	notifyObservers(s);
}

void Sequencer::setRecording(bool b)
{
    recording = b;
}

void Sequencer::setOverdubbing(bool b)
{
    overdubbing = b;
}

void Sequencer::playMetronomeTrack()
{
	if (isPlaying() || metronomeOnly) {
		return;
	}

	metronomeOnly = true;
	metronomeSeq = make_unique<Sequence>(mpc, defaultTrackNames);
	auto s = getActiveSequence().lock();
	metronomeSeq->init(8);
	metronomeSeq->setTimeSignature(0, 3, s->getNumerator(getCurrentBarNumber()), s->getDenominator(getCurrentBarNumber()));
	metronomeSeq->setInitialTempo(getTempo());
	metronomeSeq->removeFirstMetronomeClick();
	//mpc->getAudioMidiServices().lock()->getFrameSequencer().lock()->startMetronome();
}

void Sequencer::stopMetronomeTrack()
{
	if (!metronomeOnly) return;
	metronomeOnly = false;
//	mpc->getAudioMidiServices().lock()->getFrameSequencer().lock()->stop();
}

weak_ptr<Sequence> Sequencer::createSeqInPlaceHolder() {
	placeHolder = make_shared<Sequence>(mpc, defaultTrackNames);
	return placeHolder;
}

void Sequencer::clearPlaceHolder() {
	placeHolder.reset();
}

void Sequencer::movePlaceHolderTo(int destIndex) {
	sequences[destIndex].swap(placeHolder);
	clearPlaceHolder();
}

weak_ptr<Sequence> Sequencer::getPlaceHolder() {
	return placeHolder;
}

Sequencer::~Sequencer() {
	try {
		if (stopBounceThread.joinable())
			stopBounceThread.join();
	}
	catch (exception e) {
		// discard any exceptions
	}
}
