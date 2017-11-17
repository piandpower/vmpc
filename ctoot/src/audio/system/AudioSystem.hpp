#pragma once
#include <observer/Observable.hpp>

#include <vector>

namespace ctoot {
	namespace audio {
		namespace system {
			struct AudioDevice;
			struct AudioInput;
			struct AudioOutput;
			struct AudioSystem
				: public virtual moduru::observer::Observable
			{
				virtual void addAudioDevice(AudioDevice* device) = 0;
				virtual void removeAudioDevice(AudioDevice* device) = 0;
				virtual std::vector<AudioDevice*> getAudioDevices() = 0;
				virtual std::vector<AudioInput*> getAudioInputs() = 0;
				virtual std::vector<AudioOutput*> getAudioOutputs() = 0;
				virtual void setAutoConnect(bool autoConnect) = 0;
				virtual void close() = 0;

			};

		}
	}
}
