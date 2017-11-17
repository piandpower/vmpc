#include <synth/synths/multi/MultiMidiSynth.hpp>
#include <audio/system/AudioOutput.hpp>
#include <synth/SynthChannel.hpp>
#include <synth/synths/multi/MultiMidiSynthObserver.hpp>
#include <synth/synths/multi/MultiSynthControls.hpp>

using namespace ctoot::synth::synths::multi;
using namespace std;

MultiMidiSynth::MultiMidiSynth(shared_ptr<MultiSynthControls> controls)
	: BasicMidiSynth(controls->getName())
{
	observer = new MultiMidiSynthObserver(this, controls);
	controls->addObserver(observer);
}

void MultiMidiSynth::setChannel(int chan, shared_ptr<ctoot::synth::SynthChannel> synthChannel)
{
	auto old = getChannel(chan).lock();
	if (old != nullptr && dynamic_pointer_cast<ctoot::audio::system::AudioOutput>(old) != nullptr) {
		removeAudioOutput(dynamic_pointer_cast<ctoot::audio::system::AudioOutput>(old).get());
	}
	super::setChannel(chan, synthChannel);
	if (synthChannel != nullptr && dynamic_pointer_cast<ctoot::audio::system::AudioOutput>(synthChannel) != nullptr) {
		addAudioOutput(dynamic_pointer_cast<ctoot::audio::system::AudioOutput>(synthChannel).get());
	}
}

MultiMidiSynth::~MultiMidiSynth() {
	if (observer != nullptr) {
		delete observer;
	}
}
