#include <lang/Float.hpp>

#include <lang/Double.hpp>
#include <lang/DoubleConsts.hpp>
#include <lang/FloatConsts.hpp>
#include <lang/FloatingDecimal.hpp>
#include <lang/FpUtils.hpp>

#include <lang/String.hpp>

#include <cmath>

#ifdef __APPLE__
#include <cfloat>
#endif

#ifdef __linux__
#include <cfloat>
#include <cstring>
#endif

using namespace moduru::lang;
using namespace std;

Float::Float(float value)
{
	this->value = value;
}

Float::Float(double value)
{
	this->value = static_cast<float>(value);
}

Float::Float(string s)
{
	this->value = stof(s);
}

const float Float::POSITIVE_INFINITY = std::numeric_limits<float>::infinity();
const float Float::NEGATIVE_INFINITY = (-std::numeric_limits<float>::infinity());
const float Float::NaN = std::numeric_limits<float>::quiet_NaN();
//float Float::MAX_VALUE = 3.4028235E38f;
float Float::MAX_VALUE = FLT_MAX;
const float Float::MIN_NORMAL = 1.17549435E-38f;
const float Float::MIN_VALUE =  1.4E-45f;
const int32_t Float::MAX_EXPONENT;
const int32_t Float::MIN_EXPONENT;
const int32_t Float::SIZE;

String* Float::toString(float f)
{
    return (new FloatingDecimal(f))->toJavaFormatString();
}

/*
String* Float::toHexString(float f)
{
    if(abs(f) < FloatConsts::MIN_NORMAL && f != 0.0f) {
        auto s = Double::toHexString(FpUtils::scalb(static_cast< double >(f), DoubleConsts::MIN_EXPONENT - FloatConsts::MIN_EXPONENT));
        return npc(s)->replaceFirst(u"p-1022$"_j, u"p-126"_j);
    } else
        return Double::toHexString(f);
}

Float* Float::valueOf(String* s)
{
    return new Float(npc(FloatingDecimal::readJavaFormatString(s))->floatValue());
}
*/

Float* Float::valueOf(float f)
{
   return new Float(f);
}

/*
float Float::parseFloat(String* s)
{
    return npc(FloatingDecimal::readJavaFormatString(s))->floatValue();
}
*/

bool Float::isNaN(float v)
{
    return (v != v);
}

bool Float::isInfinite(float v)
{
    return (v == POSITIVE_INFINITY) || (v == NEGATIVE_INFINITY);
}

bool Float::isNaN()
{
    return isNaN(value);
}

bool Float::isInfinite()
{
    return isInfinite(value);
}

string Float::to_cppstring()
{
	return to_string(value);
}

String* Float::toString() {
	return new String(to_cppstring());
}

int8_t Float::byteValue()
{
    return static_cast< int8_t >(value);
}

int16_t Float::shortValue()
{
    return static_cast< int16_t >(value);
}

int32_t Float::intValue()
{
    return static_cast< int32_t >(value);
}

int64_t Float::longValue()
{
    return static_cast< int64_t >(value);
}

float Float::floatValue()
{
    return value;
}

double Float::doubleValue()
{
    return static_cast< double >(value);
}

/*
int32_t Float::hashCode()
{
    return floatToIntBits(value);
}

bool Float::equals(Object* obj)
{
    return (dynamic_cast< Float* >(obj) != nullptr) && (floatToIntBits(npc((dynamic_cast< Float* >(obj)))->value) == floatToIntBits(value));
}
*/

int32_t Float::floatToRawIntBits(float x)
{
	unsigned y;
	memcpy(&y, &x, 4);
	return y;
}

int32_t Float::floatToIntBits(float value)
{

    auto result = floatToRawIntBits(value);
    if(((result & FloatConsts::EXP_BIT_MASK) == FloatConsts::EXP_BIT_MASK) && (result & FloatConsts::SIGNIF_BIT_MASK) != 0)
        result = 2143289344;

    return result;
}

float Float::intBitsToFloat(int32_t bits) {
	return reinterpret_cast<float &>(bits);
}

int32_t Float::compareTo(Float* anotherFloat)
{
    return Float::compare(value, anotherFloat->value);
}

/*
int32_t Float::compareTo(Object* o)
{
    return compareTo(dynamic_cast< Float* >(o));
}
*/

int32_t Float::compare(float f1, float f2)
{
	if (f1 < f2)
		return -int32_t(1);

	if (f1 > f2)
		return 1;

	auto thisBits = Float::floatToIntBits(f1);
	auto anotherBits = Float::floatToIntBits(f2);
	return (thisBits == anotherBits ? int32_t(0) : (thisBits < anotherBits ? -int32_t(1) : int32_t(1)));
}
