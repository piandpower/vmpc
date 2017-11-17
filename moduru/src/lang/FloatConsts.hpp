#pragma once

#include <cstdint>
#include <limits>

namespace moduru {
	namespace lang {

		class FloatConsts
		{

		public:
			static constexpr float POSITIVE_INFINITY{ std::numeric_limits<float>::infinity() };
			static constexpr float NEGATIVE_INFINITY{ (-std::numeric_limits<float>::infinity()) };
			static constexpr float NaN{ std::numeric_limits<float>::quiet_NaN() };
			static float MAX_VALUE;
			static float MIN_VALUE;
			static float MIN_NORMAL;
			static constexpr int32_t SIGNIFICAND_WIDTH{ int32_t(24) };
			static constexpr int32_t MAX_EXPONENT{ int32_t(127) };
			static constexpr int32_t MIN_EXPONENT{ int32_t(-126) };
			static constexpr int32_t MIN_SUB_EXPONENT{ int32_t(-149) };
			static constexpr int32_t EXP_BIAS{ int32_t(127) };
			static constexpr int32_t SIGN_BIT_MASK{ int32_t(-0x7fffffff - 1) };
			static constexpr int32_t EXP_BIT_MASK{ int32_t(2139095040) };
			static constexpr int32_t SIGNIF_BIT_MASK{ int32_t(8388607) };

		private:
			FloatConsts();
		};

	}
}
