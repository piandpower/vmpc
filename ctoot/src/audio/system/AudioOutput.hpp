#pragma once

#include <audio/system/AudioPort.hpp>

namespace ctoot {
	namespace audio {
		namespace system {

			struct AudioOutput
				: public virtual AudioPort
			{
				virtual std::string getLocation() = 0;

			};

		}
	}
}
