#pragma once
#include <observer/Observer.hpp>

namespace ctoot {
	namespace audio {
		namespace system {

			class DefaultAudioSystem;

			class AudioSystemObserver
				: public moduru::observer::Observer
			{

			public:
				void update(moduru::observer::Observable* obs, boost::any arg1) override;

			public:
				DefaultAudioSystem* das;
				
			public:
				AudioSystemObserver(DefaultAudioSystem* das);
				~AudioSystemObserver();
				

			};

		}
	}
}
