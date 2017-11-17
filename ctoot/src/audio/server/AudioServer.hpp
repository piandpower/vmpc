#pragma once
#include <string>
#include <vector>
#include <audio/server/AudioClient.hpp>
#include <audio/server/IOAudioProcess.hpp>
#include <audio/core/AudioBuffer.hpp>

#include <memory>

namespace ctoot {
	namespace audio {

		namespace server {

			class AudioServer
			{

			protected:
				unsigned int bufferSize{ 512 };
				AudioClient* client{ nullptr };
				bool running{ false };
				std::vector<IOAudioProcess*> activeInputs;
				std::vector<IOAudioProcess*> activeOutputs;
				std::vector<ctoot::audio::core::AudioBuffer*> buffers{};

			public:
				virtual void resizeBuffers(int newSize);

			public:
				const std::vector<IOAudioProcess*>& getActiveInputs();
				const std::vector<IOAudioProcess*>& getActiveOutputs();
				const std::vector<ctoot::audio::core::AudioBuffer*>& getBuffers();
				const unsigned int getBufferSize();

			public:
				virtual void start() = 0;
				virtual void stop() = 0;
				virtual bool isRunning() = 0;
				virtual void close() = 0;
				virtual void setClient(std::weak_ptr<AudioClient> client) = 0;
				virtual std::vector<std::string> getAvailableOutputNames() = 0;
				virtual std::vector<std::string> getAvailableInputNames() = 0;
				virtual IOAudioProcess* openAudioOutput(std::string name, std::string label) = 0;
				virtual IOAudioProcess* openAudioInput(std::string name, std::string label) = 0;
				virtual void closeAudioOutput(IOAudioProcess* output) = 0;
				virtual void closeAudioInput(IOAudioProcess* input) = 0;
				virtual float getSampleRate() = 0;
				virtual float getLoad() = 0;
				virtual int getInputLatencyFrames() = 0;
				virtual int getOutputLatencyFrames() = 0;
				virtual int getTotalLatencyFrames() = 0;
				virtual void work() = 0;

			public:
				virtual ctoot::audio::core::AudioBuffer* createAudioBuffer(std::string name);
				virtual void removeAudioBuffer(ctoot::audio::core::AudioBuffer* buffer);
				//virtual void setRequestedBufferSize(const unsigned int nFrames);

			public:
				virtual ~AudioServer();

			private:
				friend class StereoInputProcess;
				friend class StereoOutputProcess;

			};

		}
	}
}
