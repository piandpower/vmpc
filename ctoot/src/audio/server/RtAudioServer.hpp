#pragma once
#include <audio/server/AudioServer.hpp>

//#include <RtAudio.h>

#include <string>
#include <vector>
#include <memory>
#include <thread>

namespace ctoot {
	namespace audio {
		namespace server {

			class RtAudioServer
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
				float getLoad() override;
				int getInputLatencyFrames() override;
				int getOutputLatencyFrames() override;
				int getTotalLatencyFrames() override;

			public:
				void work() override;
				void work(const float** InAudio, float** OutAudio, int nFrames, int inputChannels, int outputChannels);
				void work(float** InAudio, float** OutAudio, int nFrames, int inputChannels, int outputChannels);
				void work(double** InAudio, double** OutAudio, int nFrames, int outputChannels);

			public:
				RtAudioServer();

			};
		}
	}
}
