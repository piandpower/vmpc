#include <lang/FpUtils.hpp>

#include <lang/Double.hpp>
#include <lang/Float.hpp>
#include <math/Math.hpp>
#include <lang/DoubleConsts.hpp>
#include <lang/FloatConsts.hpp>

#include <cmath>

using namespace moduru::lang;

double FpUtils::twoToTheDoubleScaleUp = powerOfTwoD(512);
double FpUtils::twoToTheDoubleScaleDown = powerOfTwoD(-int32_t(512));

int32_t FpUtils::getExponent(double d)
{
    return static_cast< int32_t >((((Double::doubleToRawLongBits(d) & DoubleConsts::EXP_BIT_MASK) >> (DoubleConsts::SIGNIFICAND_WIDTH - int32_t(1))) - DoubleConsts::EXP_BIAS));
}

int32_t FpUtils::getExponent(float f)
{
    return ((Float::floatToRawIntBits(f) & FloatConsts::EXP_BIT_MASK) >> (FloatConsts::SIGNIFICAND_WIDTH - int32_t(1))) - FloatConsts::EXP_BIAS;
}

double FpUtils::powerOfTwoD(int32_t n)
{
    return Double::longBitsToDouble(((static_cast< int64_t >(n) + static_cast< int64_t >(DoubleConsts::EXP_BIAS)) << (DoubleConsts::SIGNIFICAND_WIDTH - int32_t(1))) & DoubleConsts::EXP_BIT_MASK);
}

float FpUtils::powerOfTwoF(int32_t n)
{
    return Float::intBitsToFloat(((n + FloatConsts::EXP_BIAS) << (FloatConsts::SIGNIFICAND_WIDTH - int32_t(1))) & FloatConsts::EXP_BIT_MASK);
}

double FpUtils::rawCopySign(double magnitude, double sign)
{
    return Double::longBitsToDouble((Double::doubleToRawLongBits(sign) & (DoubleConsts::SIGN_BIT_MASK)) | (Double::doubleToRawLongBits(magnitude) & (DoubleConsts::EXP_BIT_MASK | DoubleConsts::SIGNIF_BIT_MASK)));
}

float FpUtils::rawCopySign(float magnitude, float sign)
{
    return Float::intBitsToFloat((Float::floatToRawIntBits(sign) & (FloatConsts::SIGN_BIT_MASK)) | (Float::floatToRawIntBits(magnitude) & (FloatConsts::EXP_BIT_MASK | FloatConsts::SIGNIF_BIT_MASK)));
}

bool FpUtils::isFinite(double d)
{
    return fabs(d) <= DoubleConsts::MAX_VALUE;
}

bool FpUtils::isFinite(float f)
{   
    return fabs(f) <= FloatConsts::MAX_VALUE;
}

bool FpUtils::isInfinite(double d)
{   
    return Double::isInfinite(d);
}

bool FpUtils::isInfinite(float f)
{   
    return Float::isInfinite(f);
}

bool FpUtils::isNaN(double d)
{   
    return Double::isNaN(d);
}

bool FpUtils::isNaN(float f)
{   
    return Float::isNaN(f);
}

bool FpUtils::isUnordered(double arg1, double arg2)
{   
    return isNaN(arg1) || isNaN(arg2);
}

bool FpUtils::isUnordered(float arg1, float arg2)
{   
    return isNaN(arg1) || isNaN(arg2);
}

int32_t FpUtils::ilogb(double d)
{
	auto exponent = getExponent(d);
	switch (exponent) {
	case DoubleConsts::MAX_EXPONENT + int32_t(1):
		if (isNaN(d))
			return (int32_t(1) << int32_t(30));
		else
			return (int32_t(1) << int32_t(28));
	case DoubleConsts::MIN_EXPONENT - int32_t(1):
		if (d == 0.0) {
			return -(int32_t(1) << int32_t(28));
		}
		else {
			auto transducer = Double::doubleToRawLongBits(d);
			transducer &= DoubleConsts::SIGNIF_BIT_MASK;
			while (transducer < (int64_t(1LL) << (DoubleConsts::SIGNIFICAND_WIDTH - int32_t(1)))) {
				transducer *= 2;
				exponent--;
			}
			exponent++;
			return exponent;
		}
	default:
		return exponent;
	}
}

int32_t FpUtils::ilogb(float f)
{
    auto exponent = getExponent(f);
    switch (exponent) {
    case FloatConsts::MAX_EXPONENT + int32_t(1):
        if(isNaN(f))
            return (int32_t(1) << int32_t(30));
        else
            return (int32_t(1) << int32_t(28));
    case FloatConsts::MIN_EXPONENT - int32_t(1):
        if(f == 0.0f) {
            return -(int32_t(1) << int32_t(28));
        } else {
            auto transducer = Float::floatToRawIntBits(f);
            transducer &= FloatConsts::SIGNIF_BIT_MASK;
            while (transducer < (int32_t(1) << (FloatConsts::SIGNIFICAND_WIDTH - int32_t(1)))) {
                transducer *= 2;
                exponent--;
            }
            exponent++;
            return exponent;
        }
    default:
        return exponent;
    }
}

double FpUtils::scalb(double d, int32_t scale_factor)
{
	auto const MAX_SCALE = DoubleConsts::MAX_EXPONENT + -DoubleConsts::MIN_EXPONENT + DoubleConsts::SIGNIFICAND_WIDTH + int32_t(1);
	auto exp_adjust = int32_t(0);
	auto scale_increment = int32_t(0);
	auto exp_delta = Double::NaN;
	if (scale_factor < 0) {
		scale_factor = moduru::math::Math::max(scale_factor, -MAX_SCALE);
		scale_increment = -int32_t(512);
		exp_delta = twoToTheDoubleScaleDown;
	}
	else {
		scale_factor = moduru::math::Math::min(scale_factor, MAX_SCALE);
		scale_increment = 512;
		exp_delta = twoToTheDoubleScaleUp;
	}
	auto t = static_cast<int32_t>(static_cast<uint32_t>((scale_factor >> int32_t(9) - int32_t(1))) >> int32_t(32) - int32_t(9));
	exp_adjust = ((scale_factor + t) & (int32_t(512) - int32_t(1))) - t;
	d *= powerOfTwoD(exp_adjust);
	scale_factor -= exp_adjust;
	while (scale_factor != 0) {
		d *= exp_delta;
		scale_factor -= scale_increment;
	}
	return d;
}

float FpUtils::scalb(float f, int32_t scale_factor)
{
	auto const MAX_SCALE = FloatConsts::MAX_EXPONENT + -FloatConsts::MIN_EXPONENT + FloatConsts::SIGNIFICAND_WIDTH + int32_t(1);
	scale_factor = moduru::math::Math::max(moduru::math::Math::min(scale_factor, MAX_SCALE), -MAX_SCALE);
	return static_cast<float>((static_cast<double>(f) * powerOfTwoD(scale_factor)));
}

double FpUtils::nextAfter(double start, double direction)
{
	if (isNaN(start) || isNaN(direction)) {
		return start + direction;
	}
	else if (start == direction) {
		return direction;
	}
	else {
		auto transducer = Double::doubleToRawLongBits(start + 0.0);
		if (direction > start) {
			transducer = transducer + (transducer >= 0LL ? int64_t(1LL) : -int64_t(1LL));
		}
		else {
			if (transducer > 0LL)
				--transducer;
			else if (transducer < 0LL)
				++transducer;
			else
				transducer = DoubleConsts::SIGN_BIT_MASK | int64_t(1LL);
		}
		return Double::longBitsToDouble(transducer);
	}
}

float FpUtils::nextAfter(float start, double direction)
{

	if (isNaN(start) || isNaN(direction)) {
		return start + static_cast<float>(direction);
	}
	else if (start == direction) {
		return static_cast<float>(direction);
	}
	else {
		auto transducer = Float::floatToRawIntBits(start + 0.0f);
		if (direction > start) {
			transducer = transducer + (transducer >= 0 ? int32_t(1) : -int32_t(1));
		}
		else {
			if (transducer > 0)
				--transducer;
			else if (transducer < 0)
				++transducer;
			else
				transducer = FloatConsts::SIGN_BIT_MASK | int32_t(1);
		}
		return Float::intBitsToFloat(transducer);
	}
}

double FpUtils::nextUp(double d)
{
    
    if(isNaN(d) || d == Double::POSITIVE_INFINITY)
        return d;
    else {
        d += 0.0;
        return Double::longBitsToDouble(Double::doubleToRawLongBits(d) + ((d >= 0.0) ? +int64_t(1LL) : -int64_t(1LL)));
    }
}

float FpUtils::nextUp(float f)
{

	if (isNaN(f) || f == FloatConsts::POSITIVE_INFINITY)
		return f;
	else {
		f += 0.0f;
		return Float::intBitsToFloat(Float::floatToRawIntBits(f) + ((f >= 0.0f) ? +int32_t(1) : -int32_t(1)));
	}
}

double FpUtils::nextDown(double d)
{
    
    if(isNaN(d) || d == Double::NEGATIVE_INFINITY)
        return d;
    else {
        if(d == 0.0)
            return -Double::MIN_VALUE;
        else
            return Double::longBitsToDouble(Double::doubleToRawLongBits(d) + ((d > 0.0) ? -int64_t(1LL) : +int64_t(1LL)));
    }
}

double FpUtils::nextDown(float f)
{

	if (isNaN(f) || f == Float::NEGATIVE_INFINITY)
		return f;
	else {
		if (f == 0.0f)
			return -Float::MIN_VALUE;
		else
			return Float::intBitsToFloat(Float::floatToRawIntBits(f) + ((f > 0.0f) ? -int32_t(1) : +int32_t(1)));
	}
}

double FpUtils::copySign(double magnitude, double sign)
{
    
    return rawCopySign(magnitude, (isNaN(sign) ? 1.0 : sign));
}

float FpUtils::copySign(float magnitude, float sign)
{
    
    return rawCopySign(magnitude, (isNaN(sign) ? 1.0f : sign));
}

double FpUtils::ulp(double d)
{

	auto exp = getExponent(d);
	switch (exp) {
	case DoubleConsts::MAX_EXPONENT + int32_t(1):
		return fabs(d);
	case DoubleConsts::MIN_EXPONENT - int32_t(1):
		return Double::MIN_VALUE;
	default:
		exp = exp - (DoubleConsts::SIGNIFICAND_WIDTH - int32_t(1));
		if (exp >= DoubleConsts::MIN_EXPONENT) {
			return powerOfTwoD(exp);
		}
		else {
			return Double::longBitsToDouble(int64_t(1LL) << (exp - (DoubleConsts::MIN_EXPONENT - (DoubleConsts::SIGNIFICAND_WIDTH - int32_t(1)))));
		}
	}
}

float FpUtils::ulp(float f)
{

	auto exp = getExponent(f);
	switch (exp) {
	case FloatConsts::MAX_EXPONENT + int32_t(1):
		return fabs(f);
	case FloatConsts::MIN_EXPONENT - int32_t(1):
		return FloatConsts::MIN_VALUE;
	default:
		exp = exp - (FloatConsts::SIGNIFICAND_WIDTH - int32_t(1));
		if (exp >= FloatConsts::MIN_EXPONENT) {
			return powerOfTwoF(exp);
		}
		else {
			return Float::intBitsToFloat(int32_t(1) << (exp - (FloatConsts::MIN_EXPONENT - (FloatConsts::SIGNIFICAND_WIDTH - int32_t(1)))));
		}
	}
}

double FpUtils::signum(double d)
{
    
    return (d == 0.0 || isNaN(d)) ? d : copySign(1.0, d);
}

float FpUtils::signum(float f)
{
	return (f == 0.0f || isNaN(f)) ? f : copySign(1.0f, f);
}
