#include <lang/FloatConsts.hpp>

#ifdef __APPLE__
#include <cfloat>
#endif

#ifdef __linux__
#include <cfloat>
#endif

using namespace moduru::lang;

FloatConsts::FloatConsts()
{
}

constexpr float FloatConsts::POSITIVE_INFINITY;
constexpr float FloatConsts::NEGATIVE_INFINITY;
constexpr float FloatConsts::NaN;
//float FloatConsts::MAX_VALUE = 3.4028235E38f;
float FloatConsts::MAX_VALUE = FLT_MAX;
float FloatConsts::MIN_VALUE = 1.4E-45f;
float FloatConsts::MIN_NORMAL = 1.17549435E-38f;
constexpr int32_t FloatConsts::SIGNIFICAND_WIDTH;
constexpr int32_t FloatConsts::MAX_EXPONENT;
constexpr int32_t FloatConsts::MIN_EXPONENT;
constexpr int32_t FloatConsts::MIN_SUB_EXPONENT;
constexpr int32_t FloatConsts::EXP_BIAS;
constexpr int32_t FloatConsts::SIGN_BIT_MASK;
constexpr int32_t FloatConsts::EXP_BIT_MASK;
constexpr int32_t FloatConsts::SIGNIF_BIT_MASK;
