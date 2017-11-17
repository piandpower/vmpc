#pragma once
#include <synth/synths/multi/MultiMidiSynth.hpp>

#include <observer/Observer.hpp>

namespace ctoot {
	namespace synth {
		namespace synths {
			namespace multi {


				class MultiMidiSynthObserver
					: public moduru::observer::Observer
				{

				public:
					void update(moduru::observer::Observable* obs, boost::any a) override;

					MultiMidiSynthObserver(MultiMidiSynth* mms, std::weak_ptr<MultiSynthControls> controls);
					MultiMidiSynth* mms{ nullptr };
					std::weak_ptr<MultiSynthControls> controls{ };

					~MultiMidiSynthObserver();

				private:
					//    friend class MultiMidiSynth;

				};

			}
		}
	}
}
