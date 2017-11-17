#pragma once
#include <audio/system/AudioSystem.hpp>

namespace ctoot {
	namespace audio {
		namespace system {

			class DefaultAudioSystem
				: public AudioSystem
			{

			protected:
				std::vector<AudioDevice*> devices{};
				moduru::observer::Observer* observer{ nullptr };

			public:
				bool autoConnect{ false };

			public:
				void addAudioDevice(AudioDevice* device) override;

			public:
				virtual void checkUniqueDeviceName(AudioDevice* device);

			public:
				void removeAudioDevice(AudioDevice* device) override;
				std::vector<AudioDevice*> getAudioDevices() override;
				std::vector<AudioInput*> getAudioInputs() override;
				std::vector<AudioOutput*> getAudioOutputs() override;
				void setAutoConnect(bool autoConnect) override;
				void close() override;

				DefaultAudioSystem();
				virtual ~DefaultAudioSystem();

			};

		}
	}
}
