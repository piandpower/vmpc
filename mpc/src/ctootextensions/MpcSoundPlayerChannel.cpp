#include <ctootextensions/MpcSoundPlayerChannel.hpp>

#include <ctootextensions/MpcFaderControl.hpp>
#include <ctootextensions/MpcMixerInterconnection.hpp>
#include <sampler/NoteParameters.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/MixerChannel.hpp>
#include <ctootextensions/MpcSoundPlayerControls.hpp>
#include <ctootextensions/Voice.hpp>
#include <ctootextensions/MuteInfo.hpp>
#include <audio/core/MetaInfo.hpp>
#include <audio/core/AudioControlsChain.hpp>
#include <audio/mixer/AudioMixer.hpp>
#include <audio/mixer/AudioMixerStrip.hpp>
#include <audio/mixer/MainMixControls.hpp>
#include <audio/mixer/PanControl.hpp>
#include <audio/mixer/MixerControls.hpp>
#include <audio/server/AudioServer.hpp>
#include <control/CompoundControl.hpp>
#include <control/Control.hpp>
#include <ctootextensions/MpcSoundPlayerControls.hpp>

using namespace mpc::ctootextensions;
using namespace std;

MpcSoundPlayerChannel::MpcSoundPlayerChannel(weak_ptr<MpcSoundPlayerControls> controls)
{
	this->controls = controls;
	receivePgmChange = true;
	receiveMidiVolume = true;
	auto lControls = controls.lock();
	index = lControls->getDrumNumber();
	sampler = lControls->getSampler();
	mixer = lControls->getMixer();
	server = lControls->getServer();
	auto tmpVoices = lControls->getVoices();
	if (index == 0) {
		for (auto& v : tmpVoices) {
			unusedVoices.try_enqueue(v);
		}
	}
	for (int i = 0; i < 64; i++) {
		mixerChannels.push_back(make_shared<mpc::sampler::MixerChannel>());
	}
}

moodycamel::ConcurrentQueue<weak_ptr<mpc::ctootextensions::Voice>> MpcSoundPlayerChannel::unusedVoices;
moodycamel::ConcurrentQueue<weak_ptr<mpc::ctootextensions::Voice>> MpcSoundPlayerChannel::voices;

void MpcSoundPlayerChannel::setProgram(int i)
{
	if (i < 0) return;
	if (!sampler.lock()->getProgram(i).lock()) return;
	programNumber = i;
	setChanged();
	notifyObservers(string("pgm"));
}

int MpcSoundPlayerChannel::getProgram()
{
	return programNumber;
}

bool MpcSoundPlayerChannel::receivesPgmChange()
{
	return receivePgmChange;
}

void MpcSoundPlayerChannel::setReceivePgmChange(bool b)
{
	receivePgmChange = b;
	setChanged();
	notifyObservers(string("receivepgmchange"));
}

bool MpcSoundPlayerChannel::receivesMidiVolume()
{
	return receiveMidiVolume;
}

void MpcSoundPlayerChannel::setReceiveMidiVolume(bool b)
{
	receiveMidiVolume = b;
	setChanged();
	notifyObservers(string("receivemidivolume"));
}

void MpcSoundPlayerChannel::setLocation(string location)
{
	//if (info) info.reset();
	//info = make_shared<ctoot::audio::core::MetaInfo>("soundplayerchannel", location);
}

void MpcSoundPlayerChannel::noteOn(int note, int velo)
{
	mpcNoteOn(-1, note, velo, 0, 64, 0);
}

void MpcSoundPlayerChannel::mpcNoteOn(int track, int note, int velo, int varType, int varValue, int frameOffset)
{
	if (note < 35 || note > 98) return;
	if (velo == 0) return;
	auto lSampler = sampler.lock();
	program = lSampler->getProgram(programNumber);
	auto lProgram = program.lock();
	padNumber = lProgram->getPadNumberFromNote(note);
	np = lProgram->getNoteParameters(note);
	checkForMutes(np);
	soundNumber = np->getSndNumber();
	if (soundNumber == -1 || unusedVoices.size_approx() == 0 || !unusedVoices.try_dequeue(voice)) {
		return;
	}

	voices.try_enqueue(voice);
	auto lVoice = voice.lock();
	vars = lSampler->getSound(soundNumber);
	pgmMixerChannel = lProgram->getMixerChannel(padNumber);
	auto lPmc = pgmMixerChannel.lock();
	shared_ptr<ctoot::audio::mixer::AudioMixer> lMixer = mixer.lock();
	auto audioControlsChain = lMixer->getMixerControls()->getStripControls(to_string(lVoice->getStripNumber()));
	auto mainMixControls = dynamic_pointer_cast<ctoot::audio::mixer::MainMixControls>(audioControlsChain->getControls()[4]).get();
	dynamic_pointer_cast<ctoot::audio::mixer::PanControl>(mainMixControls->getControls()[0])->setValue((lPmc->getPanning()) / 100.0);
	dynamic_pointer_cast<ctoot::audio::fader::FaderControl>(mainMixControls->getControls()[2])->setValue(lPmc->getLevel());
	audioControlsChain = lMixer->getMixerControls()->getStripControls(to_string(lVoice->getStripNumber() + 32));
	mainMixControls = dynamic_pointer_cast<ctoot::audio::mixer::MainMixControls>(audioControlsChain->getControls()[4]).get();
	if (lPmc->getOutput() > 0) {
		if (vars.lock()->isMono()) {
			if (lPmc->getOutput() % 2 == 1) {

				mixerConnections[lVoice->getStripNumber() - 1]->setLeftEnabled(true);
				mixerConnections[lVoice->getStripNumber() - 1]->setRightEnabled(false);
			}
			else {
				mixerConnections[lVoice->getStripNumber() - 1]->setLeftEnabled(false);
				mixerConnections[lVoice->getStripNumber() - 1]->setRightEnabled(true);
			}
		}
		else {
			mixerConnections[lVoice->getStripNumber() - 1]->setLeftEnabled(true);
			mixerConnections[lVoice->getStripNumber() - 1]->setRightEnabled(true);
		}
	}

	faderControl = dynamic_pointer_cast<MpcFaderControl>(mainMixControls->getControls()[2]).get();

	if (faderControl->getValue() != 0) faderControl->setValue(0);

	auxBus = (int)(ceil((lPmc->getOutput() - 2) / 2.0));
	for (int i = 0; i < 4; i++) {
		compoundControl = dynamic_pointer_cast<ctoot::control::CompoundControl>(audioControlsChain->getControls()[i]).get();
		if (i == auxBus) {
			dynamic_pointer_cast<MpcFaderControl>(compoundControl->getControls()[2])->setValue((float)(lPmc->getVolumeIndividualOut()));
		}
		else {
			dynamic_pointer_cast<MpcFaderControl>(compoundControl->getControls()[2])->setValue(0);
		}
	}
	stopPad(padNumber, 1);
	lVoice->init(track, velo, padNumber, vars, np, varType, varValue, note, index, frameOffset, true);
}

void MpcSoundPlayerChannel::checkForMutes(mpc::sampler::NoteParameters* np)
{
	//auto voicesVec = vector<weak_ptr<Voice>>(voices.unsafe_begin(), voices.unsafe_end());
	vector<weak_ptr<Voice>> voicesVec;
	while (voices.size_approx() != 0) {
		weak_ptr<Voice> v;
		voices.try_dequeue(v);
		voicesVec.push_back(v);
	}
	if (np->getMuteAssignA() != 34 || np->getMuteAssignB() != 34) {
		for (auto& voice : voicesVec) {
			auto v = voice.lock();
			if (v->getMuteInfo() == nullptr) {
				continue;
			}

			if (v->getMuteInfo()->muteMe(np->getMuteAssignA(), index)
				|| v->getMuteInfo()->muteMe(np->getMuteAssignB(), index)) {
				v->startDecay();
			}
		}
	}
	for (auto& v : voicesVec)
		voices.try_enqueue(v);
}

void MpcSoundPlayerChannel::stopPad(int p, int o)
{
	vector<weak_ptr<Voice>> voicesVec;
	while (voices.size_approx() != 0) {
		weak_ptr<Voice> v;
		voices.try_dequeue(v);
		voicesVec.push_back(v);
	}
	for (auto& voice : voicesVec) {
		auto v = voice.lock();
		if (v->getPadNumber() == p
			&& v->getVoiceOverlap() == o
			&& !v->isDecaying()
			&& index == v->getMuteInfo()->getDrum()) {
			v->startDecay();
			break;
		}
	}
	for (auto& v : voicesVec)
		voices.try_enqueue(v);
}

void MpcSoundPlayerChannel::noteOff(int note)
{
	if (note < 35 || note > 98) {
		return;
	}
	stopPad(sampler.lock()->getProgram(programNumber).lock()->getPadNumberFromNote(note), 2);
}

void MpcSoundPlayerChannel::allNotesOff()
{
}

void MpcSoundPlayerChannel::allSoundOff()
{
	vector<weak_ptr<Voice>> voicesVec;
	while (voices.size_approx() != 0) {
		weak_ptr<Voice> v;
		voices.try_dequeue(v);
		voicesVec.push_back(v);
	}
	for (auto& voice : voicesVec) {
		voice.lock()->startDecay();
	}
	for (auto& v : voicesVec)
		voices.try_enqueue(v);
}

void MpcSoundPlayerChannel::allSoundOff(int frameOffset)
{
	vector<weak_ptr<Voice>> voicesVec;
	while (voices.size_approx() != 0) {
		weak_ptr<Voice> v;
		voices.try_dequeue(v);
		voicesVec.push_back(v);
	}
	for (auto& v : voicesVec) {
		v.lock()->startDecay(frameOffset);
	}
	for (auto& v : voicesVec)
		voices.try_enqueue(v);
}

void MpcSoundPlayerChannel::connectVoices()
{
	vector<weak_ptr<Voice>> voiceArray;
	while (unusedVoices.size_approx() != 0) {
		weak_ptr<Voice> v;
		unusedVoices.try_dequeue(v);
		if (v.lock()) {
			voiceArray.push_back(v);
		}
	}
	auto lMixer = mixer.lock();
	for (auto j = 0; j < 32; j++) {
		auto ams1 = lMixer->getStrip(to_string(j + 1)).lock();
		auto voice = voiceArray[j].lock();
		ams1->setInputProcess(voice.get());
		auto mi = new MpcMixerInterconnection("con" + to_string(j), server);
		ams1->setDirectOutputProcess(mi->getInputProcess());
		auto ams2 = lMixer->getStrip(to_string(j + 1 + 32)).lock();
		ams2->setInputProcess(mi->getOutputProcess());
		mixerConnections.push_back(mi);
		voice->setParent(this);
	}
	for (auto& v : voiceArray) {
		unusedVoices.try_enqueue(v);
	}
}

void MpcSoundPlayerChannel::kill(weak_ptr<Voice> mpcVoice)
{
	unusedVoices.try_enqueue(mpcVoice);
	vector<weak_ptr<Voice>> newvoices;
	weak_ptr<Voice> placeHolder;
	while (voices.try_dequeue(placeHolder)) {
		newvoices.push_back(placeHolder);
	}
	for (auto& v : newvoices) {
		if (v.lock() != mpcVoice.lock()) voices.try_enqueue(v);
	}
}

weak_ptr<ctoot::audio::core::MetaInfo> MpcSoundPlayerChannel::getInfo()
{
	return info;
}

vector<weak_ptr<mpc::sampler::MixerChannel>> MpcSoundPlayerChannel::getMixerChannels()
{
	vector<weak_ptr<mpc::sampler::MixerChannel>> res;
	for (auto& m : mixerChannels)
		res.push_back(m);
	return res;
}

int MpcSoundPlayerChannel::getDrumNumber()
{
	return index;
}

void MpcSoundPlayerChannel::mpcNoteOff(int note, int frameOffset)
{
	if (note < 35 || note > 98) {
		return;
	}
	stopPad(sampler.lock()->getProgram(programNumber).lock()->getPadNumberFromNote(note), 2, frameOffset);
}

void MpcSoundPlayerChannel::stopPad(int pad, int overlap, int offset)
{
	vector<weak_ptr<Voice>> voicesVec;
	while (voices.size_approx() != 0) {
		weak_ptr<Voice> v;
		voices.try_dequeue(v);
		voicesVec.push_back(v);
	}
	for (auto& voice : voicesVec) {
		auto v = voice.lock();
		if (v->getPadNumber() == pad
			&& v->getVoiceOverlap() == overlap
			&& !v->isDecaying()
			&& index == v->getMuteInfo()->getDrum()) {
			v->startDecay(offset);
			break;
		}
	}
	for (auto& v : voicesVec)
		voices.try_enqueue(v);
}

MpcSoundPlayerChannel::~MpcSoundPlayerChannel() {
	for (auto& m : mixerConnections) {
		if (m != nullptr) {
			delete m;
		}
	}
}
