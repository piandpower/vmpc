#pragma once
#include <audio/server/AudioServer.hpp>

//#include "RtAudio.h"

#include <string>
#include <vector>
#include <memory>

namespace ctoot {
	namespace audio {
		namespace server {

			class NamedIO;

			class AsioServer
				: public AudioServer {

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
				int deviceID = -1;

				std::vector<NamedIO> availableOutputs;
				std::vector<NamedIO> availableInputs;

			private:
				void detectOutputs();
				void detectInputs();

			private:
				bool isDeviceProbed(int devIndex);
				void destroyRtAudio();
				//static int AudioCallback(void *outputBuffer, void *inputBuffer, unsigned int nFrames,
					//double streamTime, RtAudioStreamStatus status, void *userData);

			public:
				bool startStream();
				void work() override;
				//RtAudio* getRtAudio();

			public:
				AsioServer(int deviceID);

			};
		}
	}
}
