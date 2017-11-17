#pragma once
#include <audio/server/AudioServer.hpp>
#include <audio/server/AudioClient.hpp>

#include <thread>

#include <string>
#include <vector>

namespace ctoot {
	namespace audio {

			namespace server {

			class NonRealTimeAudioServer
				: public virtual AudioServer
				, public virtual AudioClient
			{

			private:

				std::weak_ptr<NonRealTimeAudioServer> me{};

				bool realTime{ true };
				bool isRunning_{ false };
				std::weak_ptr<AudioServer> server{ };
				std::weak_ptr<AudioClient> client{ };
				std::thread nrtThread;
				bool startASAP{ false };

			private:
				static void static_nrts(void * args);

			private:
				void run();
				void startNRT();
				void stopNRT();

			public:
				void setRealTime(bool rt);
				bool isRealTime();

				void setWeakPtr(std::shared_ptr<NonRealTimeAudioServer> sharedPtr);

			public:
				// override AudioServer
				void start() override;
				void stop() override;
				void close() override;
				void setClient(std::weak_ptr<AudioClient> client) override;
				bool isRunning() override;
				float getLoad() override;
				void setEnabled(bool enable) override;
				void work() override {}; // nothing to do (?)
				ctoot::audio::core::AudioBuffer* createAudioBuffer(std::string name) override;
				void removeAudioBuffer(ctoot::audio::core::AudioBuffer* buffer) override;
				std::vector<std::string> getAvailableOutputNames() override;
				std::vector<std::string> getAvailableInputNames() override;
				IOAudioProcess* openAudioOutput(std::string name, std::string label) override;
				IOAudioProcess* openAudioInput(std::string name, std::string label) override;
				void closeAudioOutput(IOAudioProcess* output) override;
				void closeAudioInput(IOAudioProcess* input) override;
				float getSampleRate() override;
				int getInputLatencyFrames() override;
				int getOutputLatencyFrames() override;
				int getTotalLatencyFrames() override;
				void resizeBuffers(int newSize) override;

			public:
				void work(int nFrames) override; // override AudioClient::work()

			public:
				NonRealTimeAudioServer(std::weak_ptr<AudioServer> server);
				~NonRealTimeAudioServer();
			};

		}
	}
}
