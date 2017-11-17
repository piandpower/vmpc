#pragma once

namespace moduru {
	namespace math {

		class Math {

		public:
			static long min(long v1, long v2);
			static long max(long v1, long v2);
			
			static float minf(float v1, float v2);
			static float maxf(float v1, float v2);

			static unsigned int highestOneBit(unsigned int i);

		};

	}
}