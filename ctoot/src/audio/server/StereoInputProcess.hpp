#include <audio/server/AudioServerProcess.hpp>

#include <audio/core/AudioBuffer.hpp>

namespace ctoot {
	namespace audio {

		namespace server {

			class StereoInputProcess
				: public AudioServerProcess			
			{

			public:
				
				void open() override {};
				void close() override {};

				int processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) override;

				StereoInputProcess(std::string name, bool mono, std::string location);

			};

		}
	}
}
