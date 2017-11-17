#pragma once
#include <synth/BasicMidiSynth.hpp>
#include <synth/synths/multi/MultiSynthControls.hpp>

#include <memory>

namespace ctoot {
	namespace synth {
		namespace synths {
			namespace multi {

				class MultiMidiSynthObserver;

				class MultiMidiSynth
					: public ctoot::synth::BasicMidiSynth
				{

				private:
					ctoot::synth::synths::multi::MultiMidiSynthObserver* observer{ nullptr };

				public:
					typedef ctoot::synth::BasicMidiSynth super;

				public:
					void setChannel(int chan, std::shared_ptr<ctoot::synth::SynthChannel> synthChannel) override;

				public:
					MultiMidiSynth(std::shared_ptr<MultiSynthControls> controls);
					~MultiMidiSynth();
				private:
					friend class MultiMidiSynthObserver;

				};

			}
		}
	}
}
