#pragma once
#include <audio/core/AudioProcess.hpp>

#include <string>

namespace ctoot {
	namespace audio {
		namespace system {

			struct AudioPort
				: public virtual ctoot::audio::core::AudioProcess
			{
				virtual std::string getName() = 0;

			};

		}
	}
}
