#pragma once
#include <audio/core/AudioProcess.hpp>

namespace ctoot {
	namespace audio {
		namespace core {

			class AudioProcessAdapter
				: public virtual AudioProcess
			{

			protected:
				AudioProcess* process{ nullptr };

			public:
				void open() override;
				int processAudio(ctoot::audio::core::AudioBuffer* buf, int nFrames) override;
				void close() override;

				AudioProcessAdapter(AudioProcess* process);
				virtual ~AudioProcessAdapter();

			};

		}
	}
}
