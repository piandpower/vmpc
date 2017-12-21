#pragma once
#include <synth/synths/multi/MultiMidiSynth.hpp>
#include <synth/synths/multi/MultiSynthControls.hpp>

#include <vector>

namespace mpc {
	namespace ctootextensions {

		class Voice;

		class MpcMultiMidiSynth
			: public ctoot::synth::synths::multi::MultiMidiSynth
		{

		private:
			std::vector<Voice*> voices;

		public:
			std::vector<Voice*>* getVoices();

		public:
			typedef ctoot::synth::synths::multi::MultiMidiSynth super;

		public:
			virtual void mpcTransportChannel(int track, ctoot::midi::core::MidiMessage* msg, int chan, int varType, int varValue, int l);

		public:
			virtual void mpcTransport(int track, ctoot::midi::core::MidiMessage* msg, int timestamp, int varType, int varValue, int l);

		public:
			virtual void mpcTransportChannel(int track, ctoot::midi::core::MidiMessage* msg, ctoot::synth::SynthChannel* synthChannel, int varType, int varValue, int l);
			std::weak_ptr<ctoot::synth::SynthChannel> mapChannel(int chan) override;

		public:
			MpcMultiMidiSynth(std::shared_ptr<ctoot::synth::synths::multi::MultiSynthControls> controls);
			~MpcMultiMidiSynth();

		};

	}
}
