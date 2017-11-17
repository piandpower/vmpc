#pragma once
#include <audio/core/AudioProcess.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			struct MixerInterconnection
			{
				virtual ctoot::audio::core::AudioProcess* getInputProcess() = 0;
				virtual ctoot::audio::core::AudioProcess* getOutputProcess() = 0;

			};

		}
	}
}
