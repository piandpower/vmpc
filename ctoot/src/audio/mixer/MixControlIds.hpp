#pragma once

#include <cstdint>

namespace ctoot {
	namespace audio {
		namespace mixer {

			struct MixControlIds
			{
				static const int GAIN{ 1 };
				static const int MUTE{ 2 };
				static const int SOLO{ 3 };
				static const int ROUTE{ 4 };
				static const int LCR{ 5 };
				static const int FRONT_SURROUND{ 6 };
			};

		}
	}
}
