#pragma once

#include <cstdint>

namespace moduru {
	namespace lang {

		class FpUtils
		{

		private:
			static double twoToTheDoubleScaleUp;
			static double twoToTheDoubleScaleDown;

		public:
			static int32_t getExponent(double d);
			static int32_t getExponent(float f);

		public:
			static double powerOfTwoD(int32_t n);
			static float powerOfTwoF(int32_t n);

		public:
			static double rawCopySign(double magnitude, double sign);
			static float rawCopySign(float magnitude, float sign);
			static bool isFinite(double d);
			static bool isFinite(float f);
			static bool isInfinite(double d);
			static bool isInfinite(float f);
			static bool isNaN(double d);
			static bool isNaN(float f);
			static bool isUnordered(double arg1, double arg2);
			static bool isUnordered(float arg1, float arg2);
			static int32_t ilogb(double d);
			static int32_t ilogb(float f);
			static double scalb(double d, int32_t scale_factor);
			static float scalb(float f, int32_t scale_factor);
			static double nextAfter(double start, double direction);
			static float nextAfter(float start, double direction);
			static double nextUp(double d);
			static float nextUp(float f);
			static double nextDown(double d);
			static double nextDown(float f);
			static double copySign(double magnitude, double sign);
			static float copySign(float magnitude, float sign);
			static double ulp(double d);
			static float ulp(float f);
			static double signum(double d);
			static float signum(float f);

		};

	}
}
