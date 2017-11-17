#pragma once
#include <cstdint>

namespace ctoot {
	namespace control {
		namespace id {
			struct ProviderId

			{
				static const int TOOT_PROVIDER_ID{ 1 };
				static const int FRINIKA_PROVIDER_ID{ 2 };
				static const int VST_PROVIDER_ID{ 126 };
				static const int TEMPORARY_PROVIDER_ID{ 127 };
			};
		};
	};
};