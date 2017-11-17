#pragma once
#include <audio/server/AudioServer.hpp>

//#include <RtAudio.h>

#include <string>
#include <vector>
#include <memory>

namespace ctoot {
	namespace audio {
		namespace server {

			class DirectSoundServer
				: public AudioServer {

			public:
				//RtAudio* getRtAudio();

			public:
				void start() override;
				void stop() override;
				bool isRunning() override;
				void close() override;
				void setClient(std::weak_ptr<AudioClient> client) override;
				std::vector<std::string> getAvailableOutputNames() override;
				std::vector<std::string> getAvailableInputNames() override;
				IOAudioProcess* openAudioOutput(std::string name, std::string label) override;
				IOAudioProcess* openAudioInput(std::string name, std::string glabel) override;
				void closeAudioOutput(ctoot::audio::server::IOAudioProcess* output) override;
				void closeAudioInput(ctoot::audio::server::IOAudioProcess* input) override;
				float getSampleRate() override;
				float getLoad() override;
				int getInputLatencyFrames() override;
				int getOutputLatencyFrames() override;
				int getTotalLatencyFrames() override;

			private:
				//RtAudio* rtAudio{ nullptr };
				int inputID = 0;
				int outputID = 0;

			private:
				int getDeviceCount();
				std::string getDeviceName(int devIndex);
				int getDeviceIndex(std::string name);
				bool isDeviceProbed(int devIndex);
				int getInputChannelCount(int devIndex);
				int getOutputChannelCount(int devIndex);
				void destroyRtAudio();
				//static int AudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nFrames,
				//	double streamTime, RtAudioStreamStatus status, void *userData);

			public:
				bool startStream();

			public:
				void work() override;

			public:
				DirectSoundServer(std::string inputDevice, std::string outputDevice);

			};
		}
	}
}
