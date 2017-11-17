#pragma once
#include <audio/system/AudioDevice.hpp>

namespace ctoot {
	namespace audio {
		namespace system {

			class AbstractAudioDevice
				: public virtual AudioDevice
			{

			public:
				std::vector<AudioInput*> inputs{};
				std::vector<AudioOutput*> outputs{};

			private:
				std::string name{ "" };

			public:
				std::vector<AudioInput*> getAudioInputs() override;
				std::vector<AudioOutput*> getAudioOutputs() override;
				std::string getName() override;
				std::string toString();

			public:
				virtual void addAudioInput(AudioInput* input);
				virtual void removeAudioInput(AudioInput* input);
				virtual void addAudioOutput(AudioOutput* output);
				virtual void removeAudioOutput(AudioOutput* output);

			public:
				AbstractAudioDevice(std::string name);
				~AbstractAudioDevice();

			};

		}
	}
}
