#pragma once
#include <cstdint>

namespace ctoot {
	namespace audio {
		namespace id {

			struct TootAudioControlsId

			{
				static const int EQ_BASE_ID{ 0 };
				static const int DELAY_BASE_ID{ 16 };
				static const int DYNAMICS_BASE_ID{ 32 };
				static const int BASIC_BASE_ID{ 48 };
				static const int REVERB_BASE_ID{ 80 };
				static const int TOOL_BASE_ID{ 96 };
				static const int DISTORT_BASE_ID{ 112 };
				static const int MIXER_BASE_ID{ 120 };
			};
		};
	};
};