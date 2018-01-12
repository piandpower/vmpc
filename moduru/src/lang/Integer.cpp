#include <lang/Integer.hpp>

#include <lang/Character.hpp>
#include <lang/Comparable.hpp>
#include <lang/IntegerCache.hpp>
#include <lang/Number.hpp>
#include <lang/String.hpp>
#include <lang/StringBuilder.hpp>
//#include <lang/System.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::lang;
using namespace std;

Integer::Integer(int32_t value)
{
	this->value = value;
}

Integer::Integer(string s)
{
	this->value = parseInt(s, 10);
}

Integer::Integer(String* s)
{
	this->value = Integer::parseInt(s->to_cppstring(), 10);
}

constexpr int32_t Integer::MIN_VALUE;
constexpr int32_t Integer::MAX_VALUE;

vector<char16_t> Integer::digits()
{
    return digits_;
}
vector<char16_t> Integer::digits_ = vector<char16_t>({ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' });


string Integer::toString(int32_t i, int32_t radix)
{
    if(radix < Character::MIN_RADIX || radix > Character::MAX_RADIX)
        radix = 10;

    if(radix == 10) {
        return toString(i);
    }
    auto buf = vector<char16_t>(int32_t(33));
    auto negative = (i < 0);
    auto charPos = int32_t(32);
    if(!negative) {
        i = -i;
    }
    while (i <= -radix) {
        buf[charPos--] = digits_[-(i % radix)];
        i = i / radix;
    }
    buf[charPos] = digits_[-i];
    if(negative) {
        buf[--charPos] = '-';
    }
    //return new string(buf, charPos, (int32_t(33) - charPos));
	return "";
}

string Integer::toHexString(int32_t i)
{
    return toUnsignedString(i, 4);
}

string Integer::toOctalString(int32_t i)
{
    return toUnsignedString(i, 3);
}

string Integer::toBinaryString(int32_t i)
{
    return toUnsignedString(i, 1);
}

string Integer::toUnsignedString(int32_t i, int32_t shift)
{
	auto buf = vector<char16_t>(int32_t(32));
	auto charPos = int32_t(32);
	auto radix = int32_t(1) << shift;
	auto mask = radix - int32_t(1);
	do {
		buf[--charPos] = digits_[i & mask];
		i = static_cast<uint32_t>(i) >> shift;
	} while (i != 0);
	//return new String(buf, charPos, (int32_t(32) - charPos));
	return "";
}

vector<char16_t> Integer::DigitTens()
{
    return DigitTens_;
}
vector<char16_t> Integer::DigitTens_ = vector<char16_t>({ '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '4', '4', '4', '4', '4', '4', '4', '4', '4', '4', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5', '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '7', '7', '7', '7', '7', '7', '7', '7', '7', '7', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8', '9', '9', '9', '9', '9', '9', '9', '9', '9', '9' });

vector<char16_t> Integer::DigitOnes()
{
    return DigitOnes_;
}
vector<char16_t>  Integer::DigitOnes_ = vector<char16_t>({ '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1'
	, '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' });


string Integer::toString(int32_t i)
{
    if(i == Integer::MIN_VALUE)
        return "-2147483648";

    auto size = (i < 0) ? stringSize(-i) + int32_t(1) : stringSize(i);
    auto buf = vector<char16_t>(size);
    getChars(i, size, &buf);
    //return new String(int32_t(0), size, buf);
	return "";
}

void Integer::getChars(int32_t i, int32_t index, vector<char16_t>* buf)
{
	int32_t q, r;
	auto charPos = index;
	char16_t sign = int32_t(0);
	if (i < 0) {
		sign = '-';
		i = -i;
	}
	while (i >= 65536) {
		q = i / int32_t(100);
		r = i - ((q << int32_t(6)) + (q << int32_t(5)) + (q << int32_t(2)));
		i = q;
		(*buf)[--charPos] = DigitOnes_[r];
		(*buf)[--charPos] = DigitTens_[r];
	}
	for (; ; ) {
		q = static_cast<int32_t>(static_cast<uint32_t>((i * int32_t(52429))) >> (int32_t(16) + int32_t(3)));
		r = i - ((q << int32_t(3)) + (q << int32_t(1)));
		(*buf)[--charPos] = digits_[r];
		i = q;
		if (i == 0)
			break;

	}
	if (sign != 0) {
		(*buf)[--charPos] = sign;
	}
}

vector<int> Integer::sizeTable()
{
    return sizeTable_;
}
vector<int32_t> Integer::sizeTable_;

int32_t Integer::stringSize(int32_t x)
{
	for (auto i = int32_t(0); ; i++)
		if (x <= sizeTable_[i])
			return i + int32_t(1);
}

int32_t Integer::parseInt(string s, int32_t radix)
{
	if (&s == nullptr) {
		throw std::invalid_argument("null");
	}
	if (radix < Character::MIN_RADIX) {
		string msg = "radix " + to_string(radix) + " less than Character.MIN_RADIX";
		throw std::invalid_argument(msg.c_str());
	}
	if (radix > Character::MAX_RADIX) {
		string msg = "radix " + to_string(radix) + " greater than Character.MAX_RADIX";
		//throw new NumberFormatException(msg.c_str());
		throw std::invalid_argument(msg.c_str());
	}
	auto result = int32_t(0);
	auto negative = false;
	int32_t i = int32_t(0), len = s.length();
	auto limit = -Integer::MAX_VALUE;
	int32_t multmin;
	int32_t digit;
	if (len > 0) {
		auto firstChar = s[0];
		if (firstChar < '0') {
			if (firstChar == '-') {
				negative = true;
				limit = Integer::MIN_VALUE;
			}
			else
				//throw NumberFormatException::forInputString(s);
				throw std::invalid_argument("NumberFormatException");
			if (len == 1)
				//throw NumberFormatException::forInputString(s);
				throw std::invalid_argument("NumberFormatException");
			i++;
		}
		multmin = limit / radix;
		while (i < len) {
			digit = Character::digit(s[i++], radix);
			if (digit < 0) {
				//throw NumberFormatException::forInputString(s);
				throw std::invalid_argument("NumberFormatException");
			}
			if (result < multmin) {
				//throw NumberFormatException::forInputString(s);
				throw std::invalid_argument("NumberFormatException");
			}
			result *= radix;
			if (result < limit + digit) {
				//throw NumberFormatException::forInputString(s);
				throw std::invalid_argument("NumberFormatException");
			}
			result -= digit;
		}
	}
	else {
		//throw NumberFormatException::forInputString(s);
		throw std::invalid_argument("NumberFormatException");
	}
	return negative ? result : -result;
}

int32_t Integer::parseInt(string s)
{
    return parseInt(s, 10);
}

Integer* Integer::valueOf(string s, int32_t radix)
{
    return new Integer(parseInt(s, radix));
}

Integer* Integer::valueOf(string s)
{
    return new Integer(parseInt(s, 10));
}

Integer* Integer::valueOf(int32_t i)
{

	auto const offset = int32_t(128);
	if (i >= -int32_t(128) && i <= 127) {
		return IntegerCache::cache()[i + offset];
	}
	return new Integer(i);
}

int8_t Integer::byteValue()
{
    return static_cast< int8_t >(value);
}

int16_t Integer::shortValue()
{
    return static_cast< int16_t >(value);
}

int32_t Integer::intValue()
{
    return value;
}

int64_t Integer::longValue()
{
    return static_cast< int64_t >(value);
}

float Integer::floatValue()
{
    return static_cast< float >(value);
}

double Integer::doubleValue()
{
    return static_cast< double >(value);
}

string Integer::to_cppstring()
{
    return to_string(value);
}

String* Integer::toString()
{
	return new String(to_string(value));
}

/*
int32_t Integer::hashCode()
{
    return value;
}

bool Integer::equals(Object* obj)
{
    if(dynamic_cast< Integer* >(obj) != nullptr) {
        return value == npc((dynamic_cast< Integer* >(obj)))->intValue();
    }
    return false;
}
*/

Integer* Integer::getInteger(string nm)
{
	return getInteger(nm, static_cast<Integer*>(nullptr));
}

Integer* Integer::getInteger(string nm, int32_t val)
{
	auto result = getInteger(nm, static_cast<Integer*>(nullptr));
	return (result == nullptr) ? new Integer(val) : result;
}

Integer* Integer::getInteger(string nm, Integer* val)
{
	/*
	string v = nullptr;
	try {
		v = System::getProperty(nm);
	} catch (IllegalArgumentException* e) {
	} catch (NullPointerException* e) {
	}
	if(v != nullptr) {
		try {
			return Integer::decode(v);
		} catch (NumberFormatException* e) {
		}
	}
	*/
	return val;
}

Integer* Integer::decode(string nm)
{
	auto radix = int32_t(10);
	auto index = int32_t(0);
	auto negative = false;
	Integer* result;
	if (nm.length() == 0)
		throw std::invalid_argument("Zero length string");

	auto firstChar = nm[int32_t(0)];
	if (firstChar == '-') {
		negative = true;
		index++;
	}
	if (nm.find("0x") == index || nm.find("0X") == index) {
		index += 2;
		radix = 16;
	}
	else if (nm.find("#") == index) {
		index++;
		radix = 16;
	}
	else if (nm.find("0") == index && nm.length() > int32_t(1) + index) {
		index++;
		radix = 8;
	}
	if (nm.find("-") == index)
		throw std::invalid_argument("Sign character in wrong position");

	try {
		result = Integer::valueOf(nm.substr(index), radix);
		result = negative ? new Integer(-result->intValue()) : result;
	}
	catch (exception* e) {
		string constant = negative ? "-" + nm.substr(index) : nm.substr(index);
		result = Integer::valueOf(constant, radix);
	}
	return result;
}

int32_t Integer::compareTo(Integer* anotherInteger)
{
    auto thisVal = this->value;
    auto anotherVal = anotherInteger->value;
    return (thisVal < anotherVal ? -int32_t(1) : (thisVal == anotherVal ? int32_t(0) : int32_t(1)));
}

int32_t Integer::compareTo(Object* o)
{
    return compareTo(dynamic_cast< Integer* >(o));
}

constexpr int32_t Integer::SIZE;

int32_t Integer::highestOneBit(int32_t i)
{
    i |= (i >> int32_t(1));
    i |= (i >> int32_t(2));
    i |= (i >> int32_t(4));
    i |= (i >> int32_t(8));
    i |= (i >> int32_t(16));
    return i - (static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(1)));
}

int32_t Integer::lowestOneBit(int32_t i)
{
    return i & -i;
}

int32_t Integer::numberOfLeadingZeros(int32_t i)
{
    if(i == 0)
        return 32;

    auto n = int32_t(1);
    if(static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(16)) == 0) {
        n += 16;
        i <<= 16;
    }
    if(static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(24)) == 0) {
        n += 8;
        i <<= 8;
    }
    if(static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(28)) == 0) {
        n += 4;
        i <<= 4;
    }
    if(static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(30)) == 0) {
        n += 2;
        i <<= 2;
    }
    n -= static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(31));
    return n;
}

int32_t Integer::numberOfTrailingZeros(int32_t i)
{
    int32_t y;
    if(i == 0)
        return 32;

    auto n = int32_t(31);
    y = i << int32_t(16);
    if(y != 0) {
        n = n - int32_t(16);
        i = y;
    }
    y = i << int32_t(8);
    if(y != 0) {
        n = n - int32_t(8);
        i = y;
    }
    y = i << int32_t(4);
    if(y != 0) {
        n = n - int32_t(4);
        i = y;
    }
    y = i << int32_t(2);
    if(y != 0) {
        n = n - int32_t(2);
        i = y;
    }
    return n - (static_cast<int32_t>(static_cast<uint32_t>((i << int32_t(1))) >> int32_t(31)));
}

int32_t Integer::bitCount(int32_t i)
{
    i = i - ((static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(1))) & int32_t(1431655765));
    i = (i & int32_t(858993459)) + ((static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(2))) & int32_t(858993459));
    i = (i + (static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(4)))) & int32_t(252645135);
    i = i + (static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(8)));
    i = i + (static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(16)));
    return i & int32_t(63);
}

int32_t Integer::rotateLeft(int32_t i, int32_t distance)
{
    return (i << distance) | (static_cast<int32_t>(static_cast<uint32_t>(i) >> -distance));
}

int32_t Integer::rotateRight(int32_t i, int32_t distance)
{
    return (static_cast<int32_t>(static_cast<uint32_t>(i) >> distance)) | (i << -distance);
}

int32_t Integer::reverse(int32_t i)
{
	i = (i & int32_t(1431655765)) << int32_t(1) | (static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(1))) & int32_t(1431655765);
	i = (i & int32_t(858993459)) << int32_t(2) | (static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(2))) & int32_t(858993459);
	i = (i & int32_t(252645135)) << int32_t(4) | (static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(4))) & int32_t(252645135);
	i = (i << int32_t(24)) | ((i & int32_t(65280)) << int32_t(8)) | ((static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(8))) & int32_t(65280)) | (static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(24)));
	return i;
}

int32_t Integer::signum(int32_t i)
{
	return (i >> int32_t(31)) | (static_cast<int32_t>(static_cast<uint32_t>(-i) >> int32_t(31)));
}

int32_t Integer::reverseBytes(int32_t i)
{

    return ((static_cast<int32_t>(static_cast<uint32_t>(i) >> int32_t(24)))) | ((i >> int32_t(8)) & int32_t(65280)) | ((i << int32_t(8)) & int32_t(16711680))| ((i << int32_t(24)));
}
