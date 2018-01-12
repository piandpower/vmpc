#include <lang/String.hpp>

//#include <java/io/ObjectStreamField.hpp>
//#include <java/io/Serializable.hpp>
#include <lang/AbstractStringBuilder.hpp>
//#include <lang/ArrayStoreException.hpp>
#include <lang/CharSequence.hpp>
#include <lang/Character.hpp>
//#include <lang/ClassCastException.hpp>
#include <lang/Comparable.hpp>
//#include <lang/ConditionalSpecialCasing.hpp>
#include <lang/Double.hpp>
#include <lang/Float.hpp>
//#include <lang/IllegalArgumentException.hpp>
//#include <lang/IndexOutOfBoundsException.hpp>
#include <lang/Integer.hpp>
//#include <lang/Long.hpp>
//#include <lang/Math.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/Object.hpp>
#include <lang/CaseInsensitiveComparator.hpp>
#include <lang/StringBuffer.hpp>
#include <lang/StringBuilder.hpp>
//#include <lang/StringCoding.hpp>
//#include <lang/StringIndexOutOfBoundsException.hpp>
//#include <lang/System.hpp>
//#include <nio/charset/Charset.hpp>
#include <util/Arrays.hpp>
#include <util/Comparator.hpp>
//#include <util/Formatter.hpp>
//#include <util/Locale.hpp>
//#include <util/regex/Matcher.hpp>
//#include <util/regex/Pattern.hpp>
//#include <Array.hpp>
//#include <SubArray.hpp>
//#include <ObjectArray.hpp>

#include <lang/StrUtil.hpp>
#include <VecUtil.hpp>
#include <math/Math.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::lang;
using namespace std;

String::String(string str) {
	auto size = str.length();
	count = size;
	for (char16_t c : str)
		value.push_back(c);
}

String::String(int offset, int count, vector<char16_t> value) {
	this->value = value;
	this->offset = offset;
	this->count = count;
}

String::String()
{
	this->offset = 0;
	this->count = 0;
	this->value = vector<char16_t>(int32_t(0));
}

String::String(String* original)
{
	auto size = original->count;
	auto originalValue = original->value;
	vector<char16_t> v;
	if (originalValue.size() > size) {
		auto off = original->offset;
		v = *moduru::util::Arrays::copyOfRange(&originalValue, off, off + size);
	}
	else {
		v = vector<char16_t>();
		for (char16_t c : originalValue)
			v.push_back(c);
	}
	this->offset = 0;
	this->count = size;
	this->value = v;
}

String::String(vector<char16_t> value)
{
	auto size = value.size();
	offset = 0;
	count = size;
	value = *moduru::util::Arrays::copyOf(&value, size);
}

String::String(vector<char16_t> value, int32_t offset, int32_t count)
{
	if (offset < 0) {
		//throw new StringIndexOutOfBoundsException(offset);
		throw std::invalid_argument("stringindexoutofbounds");
	}
	if (count < 0) {
		//throw new StringIndexOutOfBoundsException(count);
		throw std::invalid_argument("stringindexoutofbounds");
	}
	if (offset > (int32_t)(value.size()) - count) {
		//throw new StringIndexOutOfBoundsException(offset + count);
		throw std::invalid_argument("stringindexoutofbounds");
	}
	this->offset = 0;
	this->count = count;
	value = *moduru::util::Arrays::copyOfRange(&value, offset, offset + count);
}

String::String(vector<int32_t> codePoints, int32_t offset, int32_t count)
{
	if (offset < 0) {
		//throw new StringIndexOutOfBoundsException(offset);
		throw std::invalid_argument("stringindexoutofbounds");
	}
	if (count < 0) {
		//throw new StringIndexOutOfBoundsException(count);
		throw std::invalid_argument("stringindexoutofbounds");
	}
	if (offset > (int32_t) (codePoints.size()) - count) {
		//throw new StringIndexOutOfBoundsException(offset + count);
		throw std::invalid_argument("stringindexoutofbounds");
	}
	auto n = int32_t(0);
	for (auto i = offset; i < offset + count; i++) {
		auto c = codePoints[i];
		if (c >= Character::MIN_CODE_POINT && c < Character::MIN_SUPPLEMENTARY_CODE_POINT)
			n += 1;
		else if (Character::isSupplementaryCodePoint(c))
			n += 2;
		else
			//throw new IllegalArgumentException(Integer::toString(c));
			throw std::invalid_argument("illegal argument exception");
	}
	auto v = vector<char16_t>(n);
	for (int32_t i = offset, j = int32_t(0); i < offset + count; i++) {
		auto c = codePoints[i];
		if (c < Character::MIN_SUPPLEMENTARY_CODE_POINT) {
			v[j++] = static_cast<char16_t>(c);
		}
		else {
			Character::toSurrogates(c, &v, j);
			j += 2;
		}
	}
	value = v;
	count = v.size();
	offset = 0;
}

String::String(vector<int8_t> ascii, int32_t hibyte, int32_t offset, int32_t count)
{
	checkBounds(ascii, offset, count);
	auto value = vector<char16_t>(count);
	if (hibyte == 0) {
		for (auto i = count; i-- > 0; ) {
			value[i] = static_cast<char16_t>((ascii[i + offset] & int32_t(255)));
		}
	}
	else {
		hibyte <<= 8;
		for (auto i = count; i-- > 0; ) {
			value[i] = static_cast<char16_t>((hibyte | (ascii[i + offset] & int32_t(255))));
		}
	}
	this->offset = 0;
	this->count = count;
	this->value = value;
}

String::String(vector<int8_t> ascii, int32_t hibyte) : String(ascii, hibyte, int32_t(0), ascii.size())
{
}

String::String(vector<int8_t> bytes, int32_t offset, int32_t length, String* charsetName)
{
	if (charsetName == nullptr)
		//throw new NullPointerException(u"charsetName"_j);
		throw std::invalid_argument("null pointer exception");

	checkBounds(bytes, offset, length);
	//auto v = StringCoding::decode(charsetName, bytes, offset, length);
	offset = 0;
	//this->count = v->length;
	//this->value = v;
}

String::String(vector<int8_t> bytes, int32_t offset, int32_t length, moduru::nio::charset::Charset* charset)
{
	if (charset == nullptr)
		//throw new NullPointerException(u"charset"_j);
		throw std::invalid_argument("null pointer exception");

	checkBounds(bytes, offset, length);
	//auto v = StringCoding::decode(charset, bytes, offset, length);
	this->offset = 0;
	//this->count = v->length;
	//this->value = v;
}

String::String(vector<int8_t> bytes, String* charsetName) : String(bytes, int32_t(0), bytes.size(), charsetName)
{
}

String::String(vector<int8_t> bytes, moduru::nio::charset::Charset* charset) : String(bytes, int32_t(0), bytes.size(), charset)
{
}

String::String(vector<int8_t> bytes, int32_t offset, int32_t length)
{
	checkBounds(bytes, offset, length);
	//auto v = StringCoding::decode(bytes, offset, length);
	this->offset = 0;
	//this->count = v->size();
	//this->value = v;
}

String::String(vector<int8_t> bytes) : String(bytes, int32_t(0), bytes.size())
{
}

String::String(StringBuffer* buffer)
{
	auto result = buffer->toString();
	this->value = result->value;
	this->count = result->count;
	this->offset = result->offset;
}

String::String(StringBuilder* builder)
{
	auto result = builder->toString();
	this->value = result->value;
	this->count = result->count;
	this->offset = result->offset;
}

constexpr int64_t String::serialVersionUID;

void String::checkBounds(vector<int8_t> bytes, int32_t offset, int32_t length)
{

	if (length < 0)
		//throw new StringIndexOutOfBoundsException(length);
		throw std::invalid_argument("String index out of bounds");

	if (offset < 0)
		//throw new StringIndexOutOfBoundsException(offset);
		throw std::invalid_argument("String index out of bounds");

	if (offset > bytes.size() - length)
		//throw new StringIndexOutOfBoundsException(offset + length);
		throw std::invalid_argument("String index out of bounds");
}

int32_t String::length()
{
    return count;
}

bool String::isEmpty()
{
    return count == 0;
}

char16_t String::charAt(int32_t index)
{
    if((index < 0) || (index >= count)) {
        //throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds exception");
    }
    return value[index + offset];
}

int32_t String::codePointAt(int32_t index)
{
	if ((index < 0) || (index >= count)) {
		//throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds exception");
	}
	return Character::codePointAtImpl(&value, offset + index, offset + count);
}

int32_t String::codePointBefore(int32_t index)
{
    auto i = index - int32_t(1);
    if((i < 0) || (i >= count)) {
        //throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds exception");
    }
    return Character::codePointBeforeImpl(&value, offset + index, offset);
}

int32_t String::codePointCount(int32_t beginIndex, int32_t endIndex)
{
	if (beginIndex < 0 || endIndex > count || beginIndex > endIndex) {
		//throw new IndexOutOfBoundsException();
		throw std::invalid_argument("Index out of bounds");
	}
	return Character::codePointCountImpl(&value, offset + beginIndex, endIndex - beginIndex);
}

int32_t String::offsetByCodePoints(int32_t index, int32_t codePointOffset)
{
    if(index < 0 || index > count) {
        //throw new IndexOutOfBoundsException();
		throw std::invalid_argument("string index out of bounds exception");
    }
    return Character::offsetByCodePointsImpl(&value, offset, count, offset + index, codePointOffset) - offset;
}

void String::getChars(vector<char16_t>* dst, int32_t dstBegin)
{
    moduru::VecUtil::VecCopy(&value, offset, dst, dstBegin, count);
}

void String::getChars(int32_t srcBegin, int32_t srcEnd, vector<char16_t>* dst, int32_t dstBegin)
{
	if (srcBegin < 0) {
		//throw new StringIndexOutOfBoundsException(srcBegin);
		throw std::invalid_argument("string index out of bounds exception");
	}
	if (srcEnd > count) {
		//throw new StringIndexOutOfBoundsException(srcEnd);
		throw std::invalid_argument("string index out of bounds exception");
	}
	if (srcBegin > srcEnd) {
		//throw new StringIndexOutOfBoundsException(srcEnd - srcBegin);
		throw std::invalid_argument("string index out of bounds exception");
	}
	moduru::VecUtil::VecCopy(&value, offset + srcBegin, dst, dstBegin, srcEnd - srcBegin);
}

void String::getBytes(int32_t srcBegin, int32_t srcEnd, vector<int8_t>* dst, int32_t dstBegin)
{
	if (srcBegin < 0) {
		//throw new StringIndexOutOfBoundsException(srcBegin);
		throw std::invalid_argument("string index out of bounds exception");
	}
	if (srcEnd > count) {
		//throw new StringIndexOutOfBoundsException(srcEnd);
		throw std::invalid_argument("string index out of bounds exception");
	}
	if (srcBegin > srcEnd) {
		//throw new StringIndexOutOfBoundsException(srcEnd - srcBegin);
		throw std::invalid_argument("string index out of bounds exception");
	}
	auto j = dstBegin;
	auto n = offset + srcEnd;
	auto i = offset + srcBegin;
	auto val = value;
	while (i < n) {
		(*dst)[j++] = static_cast<int8_t>(val[i++]);
	}
}

vector<int8_t> String::getBytes(String* charsetName)
{
	if (charsetName == nullptr)
		//throw new NullPointerException();
		throw std::invalid_argument("null pointer");

	//return StringCoding::encode(charsetName, value, offset, count);
	return vector<int8_t>();
}

vector<int8_t> String::getBytes(moduru::nio::charset::Charset* charset)
{
    if(charset == nullptr)
        //throw new NullPointerException();
		throw std::invalid_argument("null pointer");

    //return StringCoding::encode(charset, value, offset, count);
	return vector<int8_t>();
}

vector<int8_t> String::getBytes()
{
    //return StringCoding::encode(value, offset, count);
	return vector<int8_t>();
}

bool String::equals(Object* anObject)
{
	if (static_cast<Object*>(this) == anObject) {
		return true;
	}
	if (dynamic_cast<String*>(anObject) != nullptr) {
		auto anotherString = dynamic_cast<String*>(anObject);
		auto n = count;
		if (n == anotherString->count) {
			auto v1 = value;
			auto v2 = anotherString->value;
			auto i = offset;
			auto j = anotherString->offset;
			while (n-- != 0) {
				if (v1[i++] != v2[j++])
					return false;

			}
			return true;
		}
	}
	return false;
}

bool String::contentEquals(StringBuffer* sb)
{
	/*
    {
        synchronized synchronized_0(sb);
        {
            return contentEquals(dynamic_cast< CharSequence* >(sb));
        }
    }
	*/
	return false;
}

bool String::contentEquals(CharSequence* cs)
{
    if(count != cs->length())
        return false;

    if(dynamic_cast< AbstractStringBuilder* >(cs) != nullptr) {
        auto v1 = value;
        auto v2 = dynamic_cast< AbstractStringBuilder* >(cs)->getValue();
        auto i = offset;
        auto j = int32_t(0);
        auto n = count;
        while (n-- != 0) {
            if(v1[i++] != v2[j++])
                return false;

        }
        return true;
    }
    if(cs->equals(this))
        return true;

    auto v1 = value;
    auto i = offset;
    auto j = int32_t(0);
    auto n = count;
    while (n-- != 0) {
        if(v1[i++] != cs->charAt(j++))
            return false;
    }
    return true;
}

bool String::equalsIgnoreCase(String anotherString)
{
	//return (this == &anotherString) ? true : (&anotherString != nullptr) && (anotherString.count == count) && regionMatches(true, 0, anotherString, 0, count);
	string s1 = this->to_cppstring();
	string s2 = anotherString.to_cppstring();
	return moduru::lang::StrUtil::eqIgnoreCase(s1, s2);
}

int32_t String::compareTo(String anotherString)
{
	auto len1 = count;
	auto len2 = anotherString.count;
	auto n = moduru::math::Math::min(len1, len2);
	auto v1 = value;
	auto v2 = anotherString.value;
	auto i = offset;
	auto j = anotherString.offset;
	if (i == j) {
		auto k = i;
		auto lim = n + i;
		while (k < lim) {
			auto c1 = v1[k];
			auto c2 = v2[k];
			if (c1 != c2) {
				return c1 - c2;
			}
			k++;
		}
	}
	else {
		while (n-- != 0) {
			auto c1 = v1[i++];
			auto c2 = v2[j++];
			if (c1 != c2) {
				return c1 - c2;
			}
		}
	}
	return len1 - len2;
}

int32_t String::compareTo(Object* o)
{
	if (dynamic_cast<String*>(o) == nullptr) return -1;
	return compareTo(dynamic_cast<String*>(o));
}

moduru::util::Comparator*& String::CASE_INSENSITIVE_ORDER()
{

    return CASE_INSENSITIVE_ORDER_;
}
moduru::util::Comparator* String::CASE_INSENSITIVE_ORDER_ = new CaseInsensitiveComparator();

int32_t String::compareToIgnoreCase(String str)
{
    //return CASE_INSENSITIVE_ORDER_->compare(this, &str);
	string s1 = this->to_cppstring();
	string s2 = str.to_cppstring();
	return moduru::lang::StrUtil::eqIgnoreCase(s1, s2);
}

bool String::regionMatches(int32_t toffset, String other, int32_t ooffset, int32_t len)
{
    auto ta = value;
    auto to = offset + toffset;
    auto pa = other.value;
    auto po = other.offset + ooffset;
    if((ooffset < 0) || (toffset < 0) || (toffset > static_cast< int64_t >(count) - len)|| (ooffset > static_cast< int64_t >(other.count) - len)) {
        return false;
    }
    while (len-- > 0) {
        if(ta[to++] != pa[po++]) {
            return false;
        }
    }
    return true;
}

bool String::regionMatches(bool ignoreCase, int32_t toffset, String other, int32_t ooffset, int32_t len)
{
    auto ta = value;
    auto to = offset + toffset;
    auto pa = other.value;
    auto po = other.offset + ooffset;
    if((ooffset < 0) || (toffset < 0) || (toffset > static_cast< int64_t >(count) - len)|| (ooffset > static_cast< int64_t >(other.count) - len)) {
        return false;
    }
    while (len-- > 0) {
        auto c1 = ta[to++];
        auto c2 = pa[po++];
        if(c1 == c2) {
            continue;
        }
        if(ignoreCase) {
            auto u1 = Character::toUpperCase(c1);
            auto u2 = Character::toUpperCase(c2);
            if(u1 == u2) {
                continue;
            }
            if(Character::toLowerCase(u1) == Character::toLowerCase(u2)) {
                continue;
            }
        }
        return false;
    }
    return true;
}

bool String::startsWith(String prefix, int32_t toffset)
{
	auto ta = value;
	auto to = offset + toffset;
	auto pa = prefix.value;
	auto po = prefix.offset;
	auto pc = prefix.count;
	if ((toffset < 0) || (toffset > count - pc)) {
		return false;
	}
	while (--pc >= 0) {
		if (ta[to++] != pa[po++]) {
			return false;
		}
	}
	return true;
}

bool String::startsWith(String prefix)
{
	return startsWith(prefix, 0);
}

bool String::endsWith(String suffix)
{
	return startsWith(suffix, count - suffix.count);
}

int32_t String::hashCode()
{
    auto h = hash;
    if(h == 0) {
        auto off = offset;
        auto val = value;
        auto len = count;
        for (auto i = int32_t(0); i < len; i++) {
            h = int32_t(31) * h + val[off++];
        }
        hash = h;
    }
    return h;
}

int32_t String::indexOf(int32_t ch)
{
    return indexOf(ch, int32_t(0));
}

int32_t String::indexOf(int32_t ch, int32_t fromIndex)
{
	auto max = offset + count;
	auto v = value;
	if (fromIndex < 0) {
		fromIndex = 0;
	}
	else if (fromIndex >= count) {
		return -int32_t(1);
	}
	auto i = offset + fromIndex;
	if (ch < Character::MIN_SUPPLEMENTARY_CODE_POINT) {
		for (; i < max; i++) {
			if (v[i] == ch) {
				return i - offset;
			}
		}
		return -int32_t(1);
	}
	if (ch <= Character::MAX_CODE_POINT) {
		auto surrogates = Character::toChars(ch);
		for (; i < max; i++) {
			if (v[i] == surrogates[0]) {
				if (i + int32_t(1) == max) {
					break;
				}
				if (v[i + int32_t(1)] == surrogates[1]) {
					return i - offset;
				}
			}
		}
	}
	return -int32_t(1);
}

int32_t String::lastIndexOf(int32_t ch)
{
    return lastIndexOf(ch, count - int32_t(1));
}

int32_t String::lastIndexOf(int32_t ch, int32_t fromIndex)
{
	auto min = offset;
	auto v = value;
	auto i = offset + ((fromIndex >= count) ? count - int32_t(1) : fromIndex);
	if (ch < Character::MIN_SUPPLEMENTARY_CODE_POINT) {
		for (; i >= min; i--) {
			if (v[i] == ch) {
				return i - offset;
			}
		}
		return -int32_t(1);
	}
	auto max = offset + count;
	if (ch <= Character::MAX_CODE_POINT) {
		auto surrogates = Character::toChars(ch);
		for (; i >= min; i--) {
			if (v[i] == surrogates[0]) {
				if (i + 1 == max) {
					break;
				}
				if (v[i + 1] == surrogates[1]) {
					return i - offset;
				}
			}
		}
	}
	return -int32_t(1);
}

int32_t String::indexOf(String str)
{
    return indexOf(str, int32_t(0));
}

int32_t String::indexOf(String str, int32_t fromIndex)
{
	return indexOf(&value, offset, count, &str.value, str.offset, str.count, fromIndex);
}

int32_t String::indexOf(vector<char16_t>* source, int32_t sourceOffset, int32_t sourceCount, vector<char16_t>* target, int32_t targetOffset, int32_t targetCount, int32_t fromIndex)
{

	if (fromIndex >= sourceCount) {
		return (targetCount == 0 ? sourceCount : -int32_t(1));
	}
	if (fromIndex < 0) {
		fromIndex = 0;
	}
	if (targetCount == 0) {
		return fromIndex;
	}
	auto first = (*target)[targetOffset];
	auto max = sourceOffset + (sourceCount - targetCount);
	for (auto i = sourceOffset + fromIndex; i <= max; i++) {
		if ((*source)[i] != first) {
			while (++i <= max && (*source)[i] != first)
				;

		}
		if (i <= max) {
			auto j = i + int32_t(1);
			auto end = j + targetCount - int32_t(1);
			for (auto k = targetOffset + int32_t(1); j < end && (*source)[j] == (*target)[k]; j++, k++)
				;

			if (j == end) {
				return i - sourceOffset;
			}
		}
	}
	return -int32_t(1);
}

int32_t String::lastIndexOf(String str)
{
    return lastIndexOf(str, count);
}

int32_t String::lastIndexOf(String str, int32_t fromIndex)
{
    return lastIndexOf(&value, offset, count, &str.value, str.offset, str.count, fromIndex);
}

int32_t String::lastIndexOf(vector<char16_t>* source, int32_t sourceOffset, int32_t sourceCount, vector<char16_t>* target, int32_t targetOffset, int32_t targetCount, int32_t fromIndex)
{

	auto rightIndex = sourceCount - targetCount;
	if (fromIndex < 0) {
		return -int32_t(1);
	}
	if (fromIndex > rightIndex) {
		fromIndex = rightIndex;
	}
	if (targetCount == 0) {
		return fromIndex;
	}
	auto strLastIndex = targetOffset + targetCount - int32_t(1);
	auto strLastChar = (*target)[strLastIndex];
	auto min = sourceOffset + targetCount - int32_t(1);
	auto i = min + fromIndex;
	while (true) {
		bool startSearchForLastChar0_continue = false;
		while (i >= min && (*source)[i] != strLastChar) {
			i--;
		}
		if (i < min) {
			return -int32_t(1);
		}
		auto j = i - int32_t(1);
		auto start = j - (targetCount - int32_t(1));
		auto k = strLastIndex - int32_t(1);
		while (j > start) {
			if ((*source)[j--] != (*target)[k--]) {
				i--;
				{ startSearchForLastChar0_continue = true; break; }
			}
		}
		return start - sourceOffset + int32_t(1);

	}

startSearchForLastChar0_break:;
}

String* String::substring(int32_t beginIndex)
{
    return substring(beginIndex, count);
}

String* String::substring(int32_t beginIndex, int32_t endIndex)
{
	if (beginIndex < 0) {
		//throw new StringIndexOutOfBoundsException(beginIndex);
		throw std::invalid_argument("String index out of bounds");
	}
	if (endIndex > count) {
		//throw new StringIndexOutOfBoundsException(endIndex);
		throw std::invalid_argument("String index out of bounds");
	}
	if (beginIndex > endIndex) {
		//throw new StringIndexOutOfBoundsException(endIndex - beginIndex);
		throw std::invalid_argument("String index out of bounds");
	}
	return ((beginIndex == 0) && (endIndex == count)) ? this : new String(offset + beginIndex, endIndex - beginIndex, value);
}

CharSequence* String::subSequence(int32_t beginIndex, int32_t endIndex)
{
    return this->substring(beginIndex, endIndex);
}

String* String::concat(String* str)
{
	auto otherLen = str->length();
	if (otherLen == 0) {
		return this;
	}
	auto buf = vector<char16_t>(count + otherLen);
	getChars(0, count, &buf, 0);
	str->getChars(0, otherLen, &buf, count);
	return new String(int32_t(0), count + otherLen, buf);
}

String* String::replace(char16_t oldChar, char16_t newChar)
{
	if (oldChar != newChar) {
		auto len = count;
		auto i = -int32_t(1);
		auto val = value;
		auto off = offset;
		while (++i < len) {
			if (val[off + i] == oldChar) {
				break;
			}
		}
		if (i < len) {
			auto buf = vector<char16_t>(len);
			for (auto j = int32_t(0); j < i; j++) {
				buf[j] = val[off + j];
			}
			while (i < len) {
				auto c = val[off + i];
				buf[i] = (c == oldChar) ? newChar : c;
				i++;
			}
			return new String(int32_t(0), len, buf);
		}
	}
	return this;
}

bool String::matches(String* regex)
{
    //return moduru::util::regex::Pattern::matches(regex, this);
	return false;
}

bool String::contains(CharSequence* s)
{
	return indexOf(s->toString()) > -int32_t(1);
}

String* String::replaceFirst(String* regex, String* replacement)
{
    //return moduru::util::regex::Pattern::compile(regex)->matcher(this)->replaceFirst(replacement);
	return nullptr;
}

String* String::replaceAll(String* regex, String* replacement)
{
	//return moduru::util::regex::Pattern::compile(regex)->matcher(this)->replaceAll(replacement);
	return nullptr;
}

String* String::replace(CharSequence* target, CharSequence* replacement)
{
    //return moduru::util::regex::Pattern::compile(target->toString(), moduru::util::regex::Pattern::LITERAL)->matcher(this)->replaceAll(moduru::util::regex::Matcher::quoteReplacement(npc(replacement)->toString()));
	return nullptr;
}

vector<String*> String::split(String* regex, int32_t limit)
{
	//return moduru::util::regex::Pattern::compile(regex)->split(this, limit);
	return vector<String*>();
}

vector<String*> String::split(String* regex)
{
    return split(regex, 0);
}

/*
String* String::toLowerCase(moduru::util::Locale* locale)
{
    if(locale == nullptr) {
        //throw new NullPointerException();
		throw std::invalid_argument("nullptr");
    }
    int32_t firstUpper;
{
        for (firstUpper = 0; firstUpper < count; ) {
            auto c = value[offset + firstUpper];
            if((c >= Character::MIN_HIGH_SURROGATE) && (c <= Character::MAX_HIGH_SURROGATE)) {
                auto supplChar = codePointAt(firstUpper);
                if(supplChar != Character::toLowerCase(supplChar)) {
                    goto scan0_break;
                }
                firstUpper += Character::charCount(supplChar);
            } else {
                if(c != Character::toLowerCase(c)) {
                    goto scan0_break;
                }
                firstUpper++;
            }
        }
        return this;
    }
scan0_break:;
    auto result = vector<char16_t>(count);
    auto resultOffset = int32_t(0);
    System::arraycopy(value, offset, result, 0, firstUpper);
    auto lang = npc(locale)->getLanguage();
    auto localeDependent = (lang == u"tr"_j || lang == u"az"_j || lang == u"lt"_j);
    vector<char16_t> lowerCharArray_;
    int32_t lowerChar;
    int32_t srcChar;
    int32_t srcCount;
    for (auto i = firstUpper; i < count; i += srcCount) {
        srcChar = static_cast< int32_t >(value[offset + i]);
        if(static_cast< char16_t >(srcChar) >= Character::MIN_HIGH_SURROGATE && static_cast< char16_t >(srcChar) <= Character::MAX_HIGH_SURROGATE) {
            srcChar = codePointAt(i);
            srcCount = Character::charCount(srcChar);
        } else {
            srcCount = 1;
        }
        if(localeDependent || srcChar == u'\u03a3') {
            lowerChar = ConditionalSpecialCasing::toLowerCaseEx(this, i, locale);
        } else {
            lowerChar = Character::toLowerCase(srcChar);
        }
        if((lowerChar == Character::ERROR) || (lowerChar >= Character::MIN_SUPPLEMENTARY_CODE_POINT)) {
            if(lowerChar == Character::ERROR) {
                lowerCharArray_ = ConditionalSpecialCasing::toLowerCaseCharArray_(this, i, locale);
            } else if(srcCount == 2) {
                resultOffset += Character::toChars(lowerChar, result, i + resultOffset) - srcCount;
                continue;
            } else {
                lowerCharArray_ = Character::toChars(lowerChar);
            }
            auto mapLen = npc(lowerCharArray_)->length;
            if(mapLen > srcCount) {
                auto result2 = vector<char16_t>(result->length + mapLen - srcCount);
                System::arraycopy(result, 0, result2, 0, i + resultOffset);
                result = result2;
            }
            for (auto x = int32_t(0); x < mapLen; ++x) {
                (*result)[i + resultOffset + x] = (*lowerCharArray_)[x];
            }
            resultOffset += (mapLen - srcCount);
        } else {
            (*result)[i + resultOffset] = static_cast< char16_t >(lowerChar);
        }
    }
    return new String(int32_t(0), count + resultOffset, result);
}
*/

String* String::toLowerCase()
{
    //return toLowerCase(moduru::util::Locale::getDefault());
	return nullptr;
}

/*
String* String::toUpperCase(moduru::util::Locale* locale)
{
    if(locale == nullptr) {
        //throw new NullPointerException();
		throw std::invalid_argument("nullptr");
    }
    int32_t firstLower;
{
        for (firstLower = 0; firstLower < count; ) {
            auto c = static_cast< int32_t >(value[offset + firstLower]);
            int32_t srcCount;
            if((c >= Character::MIN_HIGH_SURROGATE) && (c <= Character::MAX_HIGH_SURROGATE)) {
                c = codePointAt(firstLower);
                srcCount = Character::charCount(c);
            } else {
                srcCount = 1;
            }
            auto upperCaseChar = Character::toUpperCaseEx(c);
            if((upperCaseChar == Character::ERROR) || (c != upperCaseChar)) {
                goto scan1_break;
            }
            firstLower += srcCount;
        }
        return this;
    }
scan1_break:;
    auto result = vector<char16_t>(count);
    auto resultOffset = int32_t(0);
    System::arraycopy(value, offset, result, 0, firstLower);
    auto lang = npc(locale)->getLanguage();
    auto localeDependent = (lang == u"tr"_j || lang == u"az"_j || lang == u"lt"_j);
    vector<char16_t> upperCharArray_;
    int32_t upperChar;
    int32_t srcChar;
    int32_t srcCount;
    for (auto i = firstLower; i < count; i += srcCount) {
        srcChar = static_cast< int32_t >(value[offset + i]);
        if(static_cast< char16_t >(srcChar) >= Character::MIN_HIGH_SURROGATE && static_cast< char16_t >(srcChar) <= Character::MAX_HIGH_SURROGATE) {
            srcChar = codePointAt(i);
            srcCount = Character::charCount(srcChar);
        } else {
            srcCount = 1;
        }
        if(localeDependent) {
            upperChar = ConditionalSpecialCasing::toUpperCaseEx(this, i, locale);
        } else {
            upperChar = Character::toUpperCaseEx(srcChar);
        }
        if((upperChar == Character::ERROR) || (upperChar >= Character::MIN_SUPPLEMENTARY_CODE_POINT)) {
            if(upperChar == Character::ERROR) {
                if(localeDependent) {
                    upperCharArray_ = ConditionalSpecialCasing::toUpperCaseCharArray_(this, i, locale);
                } else {
                    upperCharArray_ = Character::toUpperCaseCharArray_(srcChar);
                }
            } else if(srcCount == 2) {
                resultOffset += Character::toChars(upperChar, result, i + resultOffset) - srcCount;
                continue;
            } else {
                upperCharArray_ = Character::toChars(upperChar);
            }
            auto mapLen = npc(upperCharArray_)->length;
            if(mapLen > srcCount) {
                auto result2 = vector<char16_t>(result->length + mapLen - srcCount);
                System::arraycopy(result, 0, result2, 0, i + resultOffset);
                result = result2;
            }
            for (auto x = int32_t(0); x < mapLen; ++x) {
                (*result)[i + resultOffset + x] = (*upperCharArray_)[x];
            }
            resultOffset += (mapLen - srcCount);
        } else {
            (*result)[i + resultOffset] = static_cast< char16_t >(upperChar);
        }
    }
    return new String(int32_t(0), count + resultOffset, result);
}
*/

String* String::toUpperCase()
{
    //return toUpperCase(moduru::util::Locale::getDefault());
	return nullptr;
}

String* String::trim()
{
    auto len = count;
    auto st = int32_t(0);
    auto off = offset;
    auto val = value;
    while ((st < len) && (val[off + st] <= u' ')) {
        st++;
    }
    while ((st < len) && (val[off + len - int32_t(1)] <= u' ')) {
        len--;
    }
    return ((st > 0) || (len < count)) ? substring(st, len) : this;
}

String* String::toString()
{
    return this;
}

vector<char16_t>* String::toCharArray_()
{
	auto result = new vector<char16_t>(count);
	getChars(0, count, result, 0);
	return result;
}

String* String::format(String* format, vector<moduru::lang::Object*> args)
{
    //return new moduru::util::Formatter()->format(format, args)->toString();
	return nullptr;
}

/*
String* String::format(moduru::util::Locale* l, String* format, ObjectArray args)
{

    return npc((new moduru::util::Formatter(l))->format(format, args))->toString();
}
*/

String* String::valueOf(Object* obj)
{

	return (obj == nullptr) ? new String("null") : obj->toString();
}

String* String::valueOf(vector<char16_t> data)
{

	return new String(data);
}

String* String::valueOf(vector<char16_t> data, int32_t offset, int32_t count)
{

    return new String(data, offset, count);
}

String* String::copyValueOf(vector<char16_t> data, int32_t offset, int32_t count)
{

    return new String(data, offset, count);
}

String* String::copyValueOf(vector<char16_t> data)
{

    return copyValueOf(data, 0, data.size());
}

String* String::valueOf(bool b)
{

	return b ? new String("true") : new String("false");
}

String* String::valueOf(char16_t c)
{

	auto data = (vector<char16_t>({ c }));
	return new String(int32_t(0), int32_t(1), data);
}

String* String::valueOf(int32_t i)
{

	return new String(Integer::toString(i, 10));
}

String* String::valueOf(int64_t l)
{

	//return Long::toString(l, 10);
	return new String(to_string(l));
}

String* String::valueOf(float f)
{

	return new String(Float::toString(f));
}

String* String::valueOf(double d)
{

    //return Double::toString(d);
	return nullptr;
}

string String::to_cppstring() {
	string res = "";
	for (char16_t c : value)
		res.push_back((char)c);
	return res;
}

//serialPersistentFields_ = new ::java::io::ObjectStreamFieldArray(int32_t(0));

String::~String() {
}
