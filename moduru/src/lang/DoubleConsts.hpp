#pragma once

#include <limits>
#include <cstdint>

namespace moduru {
	namespace lang {

		class DoubleConsts
		{

		public:
			static constexpr double POSITIVE_INFINITY{ std::numeric_limits<double>::infinity() };
			static constexpr double NEGATIVE_INFINITY{ (-std::numeric_limits<double>::infinity()) };
			static constexpr double NaN{ std::numeric_limits<double>::quiet_NaN() };
			static constexpr double MAX_VALUE{ 1.7976931348623157E308 };
			static constexpr double MIN_VALUE{ 4.9E-324 };
			static constexpr double MIN_NORMAL{ 2.2250738585072014E-308 };
			static constexpr int32_t SIGNIFICAND_WIDTH{ int32_t(53) };
			static constexpr int32_t MAX_EXPONENT{ int32_t(1023) };
			static constexpr int32_t MIN_EXPONENT{ int32_t(-1022) };
			static constexpr int32_t MIN_SUB_EXPONENT{ int32_t(-1074) };
			static constexpr int32_t EXP_BIAS{ int32_t(1023) };
			static constexpr int64_t SIGN_BIT_MASK{ int64_t(-0x7fffffffffffffffLL - 1) };
			static constexpr int64_t EXP_BIT_MASK{ int64_t(9218868437227405312LL) };
			static constexpr int64_t SIGNIF_BIT_MASK{ int64_t(4503599627370495LL) };

		};

	}
}
