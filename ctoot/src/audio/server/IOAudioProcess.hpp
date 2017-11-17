#pragma once
#include <audio/core/AudioProcess.hpp>

#include <vector>

namespace ctoot {
	namespace audio {
		namespace server {

			class IOAudioProcess
				: public virtual ctoot::audio::core::AudioProcess
			{
			public:
				std::vector<double> localBuffer;
				virtual ctoot::audio::core::ChannelFormat* getChannelFormat() = 0;
				virtual std::string getName() = 0;

			};
		}
	}
}
