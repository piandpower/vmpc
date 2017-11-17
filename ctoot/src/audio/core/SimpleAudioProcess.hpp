#pragma once
#include <audio/core/AudioProcess.hpp>

namespace ctoot {
	namespace audio {
		namespace core {

			class SimpleAudioProcess
				: public virtual AudioProcess
			{

			public:
				void open() override {};
				void close() override {};

				virtual ~SimpleAudioProcess() {};

			};
		}
	}
}
