#pragma once
#include <cstdint>

namespace ctoot {
	namespace audio {
		namespace server {

			struct AudioClient
			{
				virtual void work(int nFrames) = 0;
				virtual void setEnabled(bool enabled) = 0;

			};
		}
	}
}
