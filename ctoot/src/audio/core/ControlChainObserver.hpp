#pragma once
#include <observer/Observer.hpp>

namespace ctoot {
	namespace audio {
		namespace core {

			class AudioProcessChain;

			class ControlChainObserver
				: public moduru::observer::Observer
			{

			private:
				AudioProcessChain* apc{ nullptr };

			public:
				 void update(moduru::observer::Observable* o, boost::any arg) override;

				 ControlChainObserver(AudioProcessChain* apc);
				 ~ControlChainObserver();

			};

		}
	}
}
