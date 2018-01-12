#include <lang/Double.hpp>

#include <lang/DoubleConsts.hpp>
#include <lang/FloatingDecimal.hpp>
#include <lang/FpUtils.hpp>

#include <lang/String.hpp>

#include <cmath>

#ifdef __linux__
#include <cstring>
#endif

using namespace moduru::lang;
using namespace std;

Double::Double(double value)
{
	this->value = value;
}

Double::Double(string s)
{
	value = stod(s);
}

constexpr double Double::POSITIVE_INFINITY;
constexpr double Double::NEGATIVE_INFINITY;
constexpr double Double::NaN;
constexpr double Double::MAX_VALUE;
constexpr double Double::MIN_NORMAL;
constexpr double Double::MIN_VALUE;
constexpr int32_t Double::MAX_EXPONENT;
constexpr int32_t Double::MIN_EXPONENT;
constexpr int32_t Double::SIZE;

//Class* Double::TYPE_;

string Double::toString(double d)
{
	auto tmp = FloatingDecimal(d).toJavaFormatString();
	return tmp->to_cppstring();
}

string Double::toHexString(double d)
{
	/*
    if(!FpUtils::isFinite(d))
        return Double::toString(d);
    else {
        auto answer = new StringBuffer(int32_t(24));
        if(FpUtils::rawCopySign(1.0, d) == -1.0)
            answer.append(u"-");

        npc(answer)->append(u"0x"_j);
        d = abs(d);
        if(d == 0.0) {
            npc(answer)->append(u"0.0p0"_j);
        } else {
            auto subnormal = (d < DoubleConsts::MIN_NORMAL);
            auto signifBits = (Double::doubleToLongBits(d) & DoubleConsts::SIGNIF_BIT_MASK) | int64_t(1152921504606846976LL);
            npc(answer)->append(subnormal ? u"0."_j : u"1."_j);
            auto signif = npc(Long::toHexString(signifBits))->substring(3, 16);
            npc(answer)->append(npc(signif)->equals(static_cast< Object* >(u"0000000000000"_j)) ? u"0"_j : npc(signif)->replaceFirst(u"0{1,12}$"_j, u""_j));
            npc(answer)->append(::StringBuilder().append(u"p"_j)->append((subnormal ? DoubleConsts::MIN_EXPONENT : FpUtils::getExponent(d)))->toString());
        }
        return npc(answer)->toString();
    }
	*/
	return "";
}

Double* Double::valueOf(string s)
{
    //return new Double(npc(FloatingDecimal::readJavaFormatString(s))->doubleValue());
	return nullptr;
}

Double* Double::valueOf(double d)
{
    return new Double(d);
}

double Double::parseDouble(string s)
{
    //return npc(FloatingDecimal::readJavaFormatString(s))->doubleValue();
	return 0;
}

bool Double::isNaN(double v)
{
    return (v != v);
}

bool Double::isInfinite(double v)
{

    return (v == POSITIVE_INFINITY) || (v == NEGATIVE_INFINITY);
}

bool Double::isNaN()
{
    return isNaN(value);
}

bool Double::isInfinite()
{
    return isInfinite(value);
}

string Double::to_cppstring()
{
	return to_string(value);
}

String* Double::toString() {
	return new String(to_string(value));
}

int8_t Double::byteValue()
{
    return static_cast< int8_t >(value);
}

int16_t Double::shortValue()
{
    return static_cast< int16_t >(value);
}

int32_t Double::intValue()
{
    return static_cast< int32_t >(value);
}

int64_t Double::longValue()
{
    return static_cast< int64_t >(value);
}

float Double::floatValue()
{
    return static_cast< float >(value);
}

double Double::doubleValue()
{
    return static_cast< double >(value);
}

/*
int32_t Double::hashCode()
{
    auto bits = doubleToLongBits(value);
    return static_cast< int32_t >((bits ^ (static_cast<int64_t>(static_cast<uint64_t>(bits) >> int32_t(32)))));
}

bool Double::equals(Object* obj)
{
    return (dynamic_cast< Double* >(obj) != nullptr) && (doubleToLongBits(npc((dynamic_cast< Double* >(obj)))->value) == doubleToLongBits(value));
}
*/

int64_t Double::doubleToLongBits(double value)
{
	auto result = doubleToRawLongBits(value);
	if (((result & DoubleConsts::EXP_BIT_MASK) == DoubleConsts::EXP_BIT_MASK) && (result & DoubleConsts::SIGNIF_BIT_MASK) != 0LL)
		result = 9221120237041090560LL;
	return result;
}

uint64_t Double::doubleToRawLongBits(double x) {
	uint64_t bits;
	memcpy(&bits, &x, sizeof bits);
	return bits;
}

double Double::longBitsToDouble(uint64_t bits) {
	Double* d = new Double(0);
	d->value = bits;
	return d->doubleValue();
}

int32_t Double::compareTo(Double* anotherDouble)
{
	return Double::compare(value, anotherDouble->value);
}

/*
int32_t Double::compareTo(Object* o)
{
    return compareTo(dynamic_cast< Double* >(o));
}
*/

int32_t Double::compare(double d1, double d2)
{
	if (d1 < d2)
		return -int32_t(1);

	if (d1 > d2)
		return 1;

	auto thisBits = Double::doubleToLongBits(d1);
	auto anotherBits = Double::doubleToLongBits(d2);
	return (thisBits == anotherBits ? int32_t(0) : (thisBits < anotherBits ? -int32_t(1) : int32_t(1)));
}
