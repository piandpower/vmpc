#include "MpcSoundPlayerChannel.hpp"

// ctootextensions
#include "MpcSoundPlayerControls.hpp"
#include "MpcMultiMidiSynth.hpp"
#include "MpcFaderControl.hpp"
#include "MpcMixerInterconnection.hpp"
#include "Voice.hpp"
#include "MuteInfo.hpp"

// mpc
#include <sampler/NoteParameters.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/StereoMixerChannel.hpp>
#include <sampler/IndivFxMixerChannel.hpp>
#include <ui/sampler/MixerSetupGui.hpp>

// ctoot
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
	for (int i = 0; i < 64; i++) {
		stereoMixerChannels.push_back(make_shared<mpc::sampler::StereoMixerChannel>());
		indivFxMixerChannels.push_back(make_shared<mpc::sampler::IndivFxMixerChannel>());
	}
}

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
	auto program = lSampler->getProgram(programNumber);
	auto lProgram = program.lock();
	auto padNumber = lProgram->getPadNumberFromNote(note);
	auto np = lProgram->getNoteParameters(note);
	checkForMutes(np);
	auto soundNumber = np->getSndNumber();

	Voice* voice = nullptr;
	auto lControls = controls.lock();
	for (auto& v : *lControls->getMms().lock()->getVoices()) {
		if (v->isFinished()) {
			voice = v;
			break;
		}
	}

	if (soundNumber == -1 || voice == nullptr) {
		return;
	}

	auto vars = lSampler->getSound(soundNumber);
	auto smc = lProgram->getStereoMixerChannel(padNumber).lock();
	auto ifmc = lProgram->getIndivFxMixerChannel(padNumber).lock();

	bool sSrcDrum = controls.lock()->getMixerSetupGui()->isStereoMixSourceDrum();
	bool iSrcDrum = controls.lock()->getMixerSetupGui()->isIndivFxSourceDrum();
	if (sSrcDrum) smc = stereoMixerChannels[padNumber];
	if (iSrcDrum) ifmc = indivFxMixerChannels[padNumber];

	shared_ptr<ctoot::audio::mixer::AudioMixer> lMixer = mixer.lock();
	auto audioControlsChain = lMixer->getMixerControls()->getStripControls(to_string(voice->getStripNumber()));
	auto mainMixControls = dynamic_pointer_cast<ctoot::audio::mixer::MainMixControls>(audioControlsChain->getControls()[4]).get();
	dynamic_pointer_cast<ctoot::audio::mixer::PanControl>(mainMixControls->getControls()[0])->setValue((smc->getPanning()) / 100.0);
	dynamic_pointer_cast<ctoot::audio::fader::FaderControl>(mainMixControls->getControls()[2])->setValue(smc->getLevel());
	audioControlsChain = lMixer->getMixerControls()->getStripControls(to_string(voice->getStripNumber() + 32));
	mainMixControls = dynamic_pointer_cast<ctoot::audio::mixer::MainMixControls>(audioControlsChain->getControls()[4]).get();
	if (ifmc->getOutput() > 0) {
		if (vars.lock()->isMono()) {
			if (ifmc->getOutput() % 2 == 1) {
				mixerConnections[voice->getStripNumber() - 1]->setLeftEnabled(true);
				mixerConnections[voice->getStripNumber() - 1]->setRightEnabled(false);
			}
			else {
				mixerConnections[voice->getStripNumber() - 1]->setLeftEnabled(false);
				mixerConnections[voice->getStripNumber() - 1]->setRightEnabled(true);
			}
		}
		else {
			mixerConnections[voice->getStripNumber() - 1]->setLeftEnabled(true);
			mixerConnections[voice->getStripNumber() - 1]->setRightEnabled(true);
		}
	}

	auto faderControl = dynamic_pointer_cast<MpcFaderControl>(mainMixControls->getControls()[2]).get();

	if (faderControl->getValue() != 0) faderControl->setValue(0);

	auto auxBus = (int)(ceil((ifmc->getOutput() - 2) / 2.0));
	for (int i = 0; i < 4; i++) {
		auto compoundControl = dynamic_pointer_cast<ctoot::control::CompoundControl>(audioControlsChain->getControls()[i]).get();
		if (i == auxBus) {
			dynamic_pointer_cast<MpcFaderControl>(compoundControl->getControls()[2])->setValue((float)(ifmc->getVolumeIndividualOut()));
		}
		else {
			dynamic_pointer_cast<MpcFaderControl>(compoundControl->getControls()[2])->setValue(0);
		}
	}
	stopPad(padNumber, 1);
	voice->init(track, velo, padNumber, vars, np, varType, varValue, note, index, frameOffset, true);
}

void MpcSoundPlayerChannel::checkForMutes(mpc::sampler::NoteParameters* np)
{
	if (np->getMuteAssignA() != 34 || np->getMuteAssignB() != 34) {
		for (auto& v : *controls.lock()->getMms().lock()->getVoices()) {
			if (v->isFinished()) continue;
			if (v->getMuteInfo() == nullptr) {
				continue;
			}

			if (v->getMuteInfo()->muteMe(np->getMuteAssignA(), index)
				|| v->getMuteInfo()->muteMe(np->getMuteAssignB(), index)) {
				v->startDecay();
			}
		}
	}
}

void MpcSoundPlayerChannel::stopPad(int p, int o)
{
	for (auto& v : *controls.lock()->getMms().lock()->getVoices()) {
		if (v->getPadNumber() == p
			&& v->getVoiceOverlap() == o
			&& !v->isDecaying()
			&& index == v->getMuteInfo()->getDrum()) {
			v->startDecay();
			break;
		}
	}
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
	for (auto& voice : *controls.lock()->getMms().lock()->getVoices()) {
		if (voice->isFinished()) continue;
		voice->startDecay();
	}
}

void MpcSoundPlayerChannel::allSoundOff(int frameOffset)
{
	for (auto& voice : *controls.lock()->getMms().lock()->getVoices()) {
		if (voice->isFinished()) continue;
		voice->startDecay(frameOffset);
	}
}

void MpcSoundPlayerChannel::connectVoices()
{
	auto lMixer = mixer.lock();
	for (auto j = 0; j < 32; j++) {
		auto ams1 = lMixer->getStrip(to_string(j + 1)).lock();
		auto voice = controls.lock()->getMms().lock()->getVoices()->at(j);
		ams1->setInputProcess(voice);
		auto mi = new MpcMixerInterconnection("con" + to_string(j), server);
		ams1->setDirectOutputProcess(mi->getInputProcess());
		auto ams2 = lMixer->getStrip(to_string(j + 1 + 32)).lock();
		ams2->setInputProcess(mi->getOutputProcess());
		mixerConnections.push_back(mi);
	}
}

weak_ptr<ctoot::audio::core::MetaInfo> MpcSoundPlayerChannel::getInfo()
{
	return weak_ptr<ctoot::audio::core::MetaInfo>();
}

vector<weak_ptr<mpc::sampler::StereoMixerChannel>> MpcSoundPlayerChannel::getStereoMixerChannels()
{
	vector<weak_ptr<mpc::sampler::StereoMixerChannel>> res;
	for (auto& m : stereoMixerChannels)
		res.push_back(m);
	return res;
}

vector<weak_ptr<mpc::sampler::IndivFxMixerChannel>> MpcSoundPlayerChannel::getIndivFxMixerChannels()
{
	vector<weak_ptr<mpc::sampler::IndivFxMixerChannel>> res;
	for (auto& m : indivFxMixerChannels)
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
	for (auto& v : *controls.lock()->getMms().lock()->getVoices()) {
		if (v->getPadNumber() == pad
			&& v->getVoiceOverlap() == overlap
			&& !v->isDecaying()
			&& index == v->getMuteInfo()->getDrum()) {
			v->startDecay(offset);
			break;
		}
	}
}

MpcSoundPlayerChannel::~MpcSoundPlayerChannel() {
	for (auto& m : mixerConnections) {
		if (m != nullptr) {
			delete m;
		}
	}
}
