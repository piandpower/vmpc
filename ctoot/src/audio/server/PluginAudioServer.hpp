#ifndef SRC_UK_ORG_TOOT_AUDIO_SERVER_PLUGINAUDIOSERVER_HPP_
#define SRC_UK_ORG_TOOT_AUDIO_SERVER_PLUGINAUDIOSERVER_HPP_

#include <audio/server/AudioServer.hpp>
#include <audio/server/AudioClient.hpp>
#include <audio/server/IOAudioProcess.hpp>
#include <audio/server/NamedIO.hpp>

#include <vector>
#include <string>

namespace ctoot {
	namespace audio
	{
		namespace server
		{

			class PluginAudioServer
				: public AudioServer
			{
			public:
				void close() override;
				void start() override;
				void stop() override;
				bool isRunning() override;
				void setClient(std::weak_ptr<AudioClient> client) override;
				std::vector<std::string> getAvailableOutputNames() override;
				std::vector<std::string> getAvailableInputNames() override;
				IOAudioProcess* openAudioOutput(std::string name, std::string label) override;
				IOAudioProcess* openAudioInput(std::string name, std::string label) override;
				void closeAudioOutput(IOAudioProcess* output) override;
				void closeAudioInput(IOAudioProcess* input) override;
				float getSampleRate() override;
				float getLoad() override;
				int getInputLatencyFrames() override;
				int getOutputLatencyFrames() override;
				int getTotalLatencyFrames() override;

			public:
				void work(double** inputs, double** outputs, int nFrames);
				void work() override {}; // nothing to do, custom work call

			private:
				void detectOutputs();
				void detectInputs();

			private:
				std::vector<NamedIO> availableOutputs;
				std::vector<NamedIO> availableInputs;

				std::vector<IOAudioProcess*> activeInputs;
				std::vector<IOAudioProcess*> activeOutputs;
				int inCount{ 0 };
				int outCount{ 0 };
				std::weak_ptr<AudioClient> client{ };
				bool running{ false };
				int frameCounter{ 0 };

				int bufferPointer{ 0 };

			public:
				PluginAudioServer(int inCount, int outCount);
				~PluginAudioServer();

			};

		}
	}
}
#endif /* SRC_UK_ORG_TOOT_AUDIO_SERVER_PLUGINAUDIOSERVER_HPP_ */
