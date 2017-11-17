#pragma once

//#include <misc/IObservable.hpp>
#include <observer/Observable.hpp>

#include <string>
#include <vector>

namespace ctoot {
	namespace audio {
		namespace system {

			struct AudioInput;
			struct AudioOutput;
			struct AudioDevice
				: virtual moduru::observer::Observable
			{
				virtual std::string getName() = 0;
				virtual std::vector<AudioInput*> getAudioInputs() = 0;
				virtual std::vector<AudioOutput*> getAudioOutputs() = 0;
				virtual void closeAudio() = 0;

			};

		}
	}
}
