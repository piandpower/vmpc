#include <synth/SynthRack.hpp>

#include <audio/system/AudioDevice.hpp>
#include <audio/system/AudioSystem.hpp>
#include <midi/core/MidiDevice.hpp>
#include <midi/core/MidiInput.hpp>
#include <midi/core/MidiPort.hpp>
#include <midi/core/MidiSystem.hpp>
#include <midi/core/MidiTransport.hpp>
#include <synth/MidiSynth.hpp>
#include <synth/SynthRackObserver.hpp>
#include <synth/SynthRackControls.hpp>

using namespace ctoot::synth;

SynthRack::SynthRack(std::weak_ptr<SynthRackControls> controls, std::weak_ptr<ctoot::midi::core::MidiSystem> midiSystem, std::weak_ptr<ctoot::audio::system::AudioSystem> audioSystem)
{
	this->midiSystem = midiSystem;
	this->audioSystem = audioSystem;
	auto lControls = controls.lock();
	synths = std::vector<std::shared_ptr<MidiSynth>>(lControls->size());
	for (auto& s : synths)
		s = nullptr;
	observer = new SynthRackObserver(this, controls);
	lControls->addObserver(observer);
}

void SynthRack::setMidiSynth(int i, std::shared_ptr<MidiSynth> synth)
{
	auto lMidiSystem = midiSystem.lock();
	auto lAudioSystem = audioSystem.lock();
	auto old = synths[i];
	if (old != nullptr) {
		lMidiSystem->removeMidiDevice(old.get());
		lAudioSystem->removeAudioDevice(old.get());
	}
	synths[i] = synth;
	if (synths[i] == nullptr) return;
	lMidiSystem->addMidiDevice(synths[i].get());
	lAudioSystem->addAudioDevice(synths[i].get());
}

std::weak_ptr<MidiSynth> SynthRack::getMidiSynth(int i)
{
	return synths[i];
}

void SynthRack::close()
{
	for (auto& s : synths)
		s.reset();
	if (observer != nullptr) {
		delete observer;
	}
}
