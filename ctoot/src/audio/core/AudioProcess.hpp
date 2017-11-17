#pragma once

#include <cstdint>
#include <audio/core/AudioBuffer.hpp>

namespace ctoot {
	namespace audio {
		namespace core {

			class AudioProcess
			{
			public:
				static const int AUDIO_OK{ 0 };
				static const int AUDIO_DISCONNECT{ 1 };
				static const int AUDIO_SILENCE{ 2 };
				virtual void open() = 0;
				virtual int processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) = 0;
				virtual void close() = 0;

				virtual ~AudioProcess() {};

			};
		}
	}
}
