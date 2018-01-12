#include <lang/AbstractStringBuilder.hpp>

#include <lang/CharSequence.hpp>
#include <lang/Character.hpp>
//#include <lang/ClassCastException.hpp>
#include <lang/FloatingDecimal.hpp>
//#include <lang/IllegalArgumentException.hpp>
//#include <lang/IndexOutOfBoundsException.hpp>
#include <lang/Integer.hpp>
//#include <lang/Long.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/String.hpp>
#include <lang/StringBuffer.hpp>
#include <lang/StringBuilder.hpp>
//#include <lang/StringIndexOutOfBoundsException.hpp>
//#include <lang/System.hpp>
#include <util/Arrays.hpp>
//#include <Array.hpp>

#include <VecUtil.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::lang;
using namespace std;

AbstractStringBuilder::AbstractStringBuilder()
{
}

AbstractStringBuilder::AbstractStringBuilder(int32_t capacity)
{
    value = vector<char16_t>(capacity);
}

int32_t AbstractStringBuilder::length()
{
    return count;
}

int32_t AbstractStringBuilder::capacity()
{
	return value.size();
}

void AbstractStringBuilder::ensureCapacity(int32_t minimumCapacity)
{
    if(minimumCapacity > value.size()) {
        expandCapacity(minimumCapacity);
    }
}

void AbstractStringBuilder::expandCapacity(int32_t minimumCapacity)
{
    auto newCapacity = (value.size() + int32_t(1)) * int32_t(2);
    if(newCapacity < 0) {
        newCapacity = Integer::MAX_VALUE;
    } else if(minimumCapacity > newCapacity) {
        newCapacity = minimumCapacity;
    }
    value = *moduru::util::Arrays::copyOf(&value, newCapacity);
}

void AbstractStringBuilder::trimToSize()
{
    if(count < value.size()) {
        value = *moduru::util::Arrays::copyOf(&value, count);
    }
}

void AbstractStringBuilder::setLength(int32_t newLength)
{
	if (newLength < 0)
		//throw new StringIndexOutOfBoundsException(newLength);
		throw std::invalid_argument("string index out of bounds");

    if(newLength > value.size())
        expandCapacity(newLength);

    if(count < newLength) {
        for (; count < newLength; count++)
                        value[count] = char16_t(0x0000);

    } else {
        count = newLength;
    }
}

char16_t AbstractStringBuilder::charAt(int32_t index)
{
    if((index < 0) || (index >= count))
        //throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds");

    return value[index];
}

int32_t AbstractStringBuilder::codePointAt(int32_t index)
{
    if((index < 0) || (index >= count)) {
        //throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds");
    }
    return Character::codePointAt(&value, index);
}

int32_t AbstractStringBuilder::codePointBefore(int32_t index)
{
    auto i = index - int32_t(1);
    if((i < 0) || (i >= count)) {
        //throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds");
    }
    return Character::codePointBefore(&value, index);
}

int32_t AbstractStringBuilder::codePointCount(int32_t beginIndex, int32_t endIndex)
{
    if(beginIndex < 0 || endIndex > count || beginIndex > endIndex) {
        //throw new IndexOutOfBoundsException();
		throw std::invalid_argument("index out of bounds");
    }
    return Character::codePointCountImpl(&value, beginIndex, endIndex - beginIndex);
}

int32_t AbstractStringBuilder::offsetByCodePoints(int32_t index, int32_t codePointOffset)
{
    if(index < 0 || index > count) {
        //throw new IndexOutOfBoundsException();
		throw std::invalid_argument("index out of bounds");
    }
    return Character::offsetByCodePointsImpl(&value, 0, count, index, codePointOffset);
}

void AbstractStringBuilder::getChars(int32_t srcBegin, int32_t srcEnd, vector<char16_t>* dst, int32_t dstBegin)
{
    if(srcBegin < 0)
        //throw new StringIndexOutOfBoundsException(srcBegin);
		throw std::invalid_argument("string index out of bounds");

    if((srcEnd < 0) || (srcEnd > count))
        //throw new StringIndexOutOfBoundsException(srcEnd);
		throw std::invalid_argument("string index out of bounds");

    if(srcBegin > srcEnd)
        //throw new StringIndexOutOfBoundsException(u"srcBegin > srcEnd"_j);
		throw std::invalid_argument("string index out of bounds");

    moduru::VecUtil::VecCopy(&value, srcBegin, dst, dstBegin, srcEnd - srcBegin);
}

void AbstractStringBuilder::setCharAt(int32_t index, char16_t ch)
{
    if((index < 0) || (index >= count))
        //throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds");

    value[index] = ch;
}

AbstractStringBuilder* AbstractStringBuilder::append(Object* obj)
{
    return append(String::valueOf(obj));
}

AbstractStringBuilder* AbstractStringBuilder::append(String* str)
{
	if (str == nullptr)
		str = new String("null");

	auto len = str->length();
	if (len == 0)
		return this;

	auto newCount = count + len;
	if (newCount > value.size())
		expandCapacity(newCount);

	str->getChars(0, len, &value, count);
	count = newCount;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(StringBuffer* sb)
{
	if (sb == nullptr)
		return append("null");

	auto len = sb->length();
	auto newCount = count + len;
	if (newCount > value.size())
		expandCapacity(newCount);

	sb->getChars(int32_t(0), len, &value, count);
	count = newCount;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(CharSequence* s)
{
	if (s == nullptr)
		s = new String("null");

    if(dynamic_cast< String* >(s) != nullptr)
        return this->append(dynamic_cast< String* >(s));

    if(dynamic_cast< StringBuffer* >(s) != nullptr)
        return this->append(dynamic_cast< StringBuffer* >(s));

    return this->append(s, int32_t(0), s->length());
}

AbstractStringBuilder* AbstractStringBuilder::append(CharSequence* s, int32_t start, int32_t end)
{
	if (s == nullptr)
		s = new String("null");

	if ((start < 0) || (end < 0) || (start > end) || (end > s->length()))
		/*
		throw new IndexOutOfBoundsException(::StringBuilder().append(u"start "_j)->append(start)
			->append(u", end "_j)
			->append(end)
			->append(u", s.length() "_j)
			->append(s->length())->toString());
			*/
		throw std::invalid_argument("Index out of bounds");
	auto len = end - start;
	if (len == 0)
		return this;

	auto newCount = count + len;
	if (newCount > value.size())
		expandCapacity(newCount);

	for (auto i = start; i < end; i++)
		value[count++] = s->charAt(i);

	count = newCount;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(vector<char16_t> str)
{
	auto newCount = count + str.size();
	if (newCount > value.size())
		expandCapacity(newCount);

	moduru::VecUtil::VecCopy(&str, 0, &value, count, str.size());
	count = newCount;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(vector<char16_t> str, int32_t offset, int32_t len)
{
	auto newCount = count + len;
	if (newCount > value.size())
		expandCapacity(newCount);

	moduru::VecUtil::VecCopy(&str, offset, &value, count, len);
	count = newCount;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(bool b)
{
    if(b) {
        auto newCount = count + int32_t(4);
        if(newCount > value.size())
            expandCapacity(newCount);

        value[count++] = u't';
        value[count++] = u'r';
        value[count++] = u'u';
        value[count++] = u'e';
    } else {
        auto newCount = count + int32_t(5);
        if(newCount > value.size())
            expandCapacity(newCount);

        value[count++] = u'f';
        value[count++] = u'a';
        value[count++] = u'l';
        value[count++] = u's';
        value[count++] = u'e';
    }
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(char16_t c)
{
    auto newCount = count + int32_t(1);
    if(newCount > value.size())
        expandCapacity(newCount);

    value[count++] = c;
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(int32_t i)
{
	if (i == Integer::MIN_VALUE) {
		append("-2147483648");
		return this;
	}
    auto appendedLength = (i < 0) ? Integer::stringSize(-i) + int32_t(1) : Integer::stringSize(i);
    auto spaceNeeded = count + appendedLength;
    if(spaceNeeded > value.size())
        expandCapacity(spaceNeeded);

    Integer::getChars(i, spaceNeeded, &value);
    count = spaceNeeded;
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(int64_t l)
{
	/*
    if(l == Long::MIN_VALUE) {
        append("-9223372036854775808");
        return this;
    }
    auto appendedLength = (l < 0) ? Long::stringSize(-l) + int32_t(1) : Long::stringSize(l);
    auto spaceNeeded = count + appendedLength;
    if(spaceNeeded > value.size())
        expandCapacity(spaceNeeded);

    Long::getChars(l, spaceNeeded, value);
    count = spaceNeeded;
	*/
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(float f)
{
    //(new FloatingDecimal(f))->appendTo(this);
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::append(double d)
{
    //(new FloatingDecimal(d))->appendTo(this);
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::delete_(int32_t start, int32_t end)
{
    if(start < 0)
        //throw new StringIndexOutOfBoundsException(start);
		throw std::invalid_argument("string index out of bounds");

    if(end > count)
        end = count;

    if(start > end)
        //throw new StringIndexOutOfBoundsException();
		throw std::invalid_argument("string index out of bounds");

    auto len = end - start;
    if(len > 0) {
        moduru::VecUtil::VecCopy(&value, start + len, &value, start, count - end);
        count -= len;
    }
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::appendCodePoint(int32_t codePoint)
{
    if(!Character::isValidCodePoint(codePoint)) {
        //throw new IllegalArgumentException();
		throw std::invalid_argument("illegal argument");
    }
    auto n = int32_t(1);
    if(codePoint >= Character::MIN_SUPPLEMENTARY_CODE_POINT) {
        n++;
    }
    auto newCount = count + n;
    if(newCount > value.size()) {
        expandCapacity(newCount);
    }
    if(n == 1) {
        value[count++] = static_cast< char16_t >(codePoint);
    } else {
        Character::toSurrogates(codePoint, &value, count);
        count += n;
    }
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::deleteCharAt(int32_t index)
{
	if ((index < 0) || (index >= count))
		//throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds");

	moduru::VecUtil::VecCopy(&value, index + int32_t(1), &value, index, count - index - int32_t(1));
	count--;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::replace(int32_t start, int32_t end, String* str)
{
    if(start < 0)
        //throw new StringIndexOutOfBoundsException(start);
		throw std::invalid_argument("string index out of bounds");

    if(start > count)
        //throw new StringIndexOutOfBoundsException(u"start > length()"_j);
		throw std::invalid_argument("string index out of bounds");

    if(start > end)
        //throw new StringIndexOutOfBoundsException(u"start > end"_j);
		throw std::invalid_argument("string index out of bounds");

    if(end > count)
        end = count;

    auto len = str->length();
    auto newCount = count + len - (end - start);
    if(newCount > value.size())
        expandCapacity(newCount);

    moduru::VecUtil::VecCopy(&value, end, &value, start + len, count - end);
    str->getChars(&value, start);
    count = newCount;
    return this;
}

String* AbstractStringBuilder::substring(int32_t start)
{
    return substring(start, count);
}

CharSequence* AbstractStringBuilder::subSequence(int32_t start, int32_t end)
{
    return substring(start, end);
}

String* AbstractStringBuilder::substring(int32_t start, int32_t end)
{
    if(start < 0)
        //throw new StringIndexOutOfBoundsException(start);
		throw std::invalid_argument("string index out of bounds");

    if(end > count)
        //throw new StringIndexOutOfBoundsException(end);
		throw std::invalid_argument("string index out of bounds");

    if(start > end)
        //throw new StringIndexOutOfBoundsException(end - start);
		throw std::invalid_argument("string index out of bounds");

    return new String(value, start, end - start);
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t index, vector<char16_t> str, int32_t offset, int32_t len)
{
	if ((index < 0) || (index > length()))
		//throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds");

	if ((offset < 0) || (len < 0) || (offset > str.size() - len))
		/*
		throw new StringIndexOutOfBoundsException(::StringBuilder().append(u"offset "_j)->append(offset)
			->append(u", len "_j)
			->append(len)
			->append(u", str.length "_j)
			->append(str->length)->toString());
			*/
		throw std::invalid_argument("string index out of bounds");

	auto newCount = count + len;
	if (newCount > value.size())
		expandCapacity(newCount);

	moduru::VecUtil::VecCopy(&value, index, &value, index + len, count - index);
	moduru::VecUtil::VecCopy(&str, offset, &value, index, len);
	count = newCount;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, Object* obj)
{
    return insert(offset, String::valueOf(obj));
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, String* str)
{
	if ((offset < 0) || (offset > length()))
		//throw new StringIndexOutOfBoundsException(offset);
		throw std::invalid_argument("string index out of bounds");

	if (str == nullptr)
		str = new String("null");

	auto len = str->length();
	auto newCount = count + len;
	if (newCount > value.size())
		expandCapacity(newCount);

	moduru::VecUtil::VecCopy(&value, offset, &value, offset + len, count - offset);
	str->getChars(&value, offset);
	count = newCount;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, vector<char16_t> str)
{
	if ((offset < 0) || (offset > length()))
		//throw new StringIndexOutOfBoundsException(offset);
		throw std::invalid_argument("string index out of bounds");

	auto len = str.size();
	auto newCount = count + len;
	if (newCount > value.size())
		expandCapacity(newCount);

	moduru::VecUtil::VecCopy(&value, offset, &value, offset + len, count - offset);
	moduru::VecUtil::VecCopy(&str, 0, &value, offset, len);
	count = newCount;
	return this;
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t dstOffset, CharSequence* s)
{
	if (s == nullptr)
		s = new String("null");

    if(dynamic_cast< String* >(s) != nullptr)
        return this->insert(dstOffset, dynamic_cast< String* >(s));

    return this->insert(dstOffset, s, int32_t(0), s->length());
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t dstOffset, CharSequence* s, int32_t start, int32_t end)
{
	if (s == nullptr)
		s = new String("null");

    if((dstOffset < 0) || (dstOffset > this->length()))
        //throw new IndexOutOfBoundsException(::StringBuilder().append(u"dstOffset "_j)->append(dstOffset)->toString());
		throw std::invalid_argument("index out of bounds");

    if((start < 0) || (end < 0) || (start > end)|| (end > s->length()))
        /*
		throw new IndexOutOfBoundsException(::StringBuilder().append(u"start "_j)->append(start)
            ->append(u", end "_j)
            ->append(end)
            ->append(u", s.length() "_j)
            ->append(s->length())->toString());
			*/
		throw std::invalid_argument("index out of bounds");

    auto len = end - start;
    if(len == 0)
        return this;

    auto newCount = count + len;
    if(newCount > value.size())
        expandCapacity(newCount);

    moduru::VecUtil::VecCopy(&value, dstOffset, &value, dstOffset + len, count - dstOffset);
    for (auto i = start; i < end; i++)
                value[dstOffset++] = s->charAt(i);

    count = newCount;
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, bool b)
{
    return insert(offset, String::valueOf(b));
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, char16_t c)
{
    auto newCount = count + int32_t(1);
    if(newCount > value.size())
        expandCapacity(newCount);

    moduru::VecUtil::VecCopy(&value, offset, &value, offset + int32_t(1), count - offset);
    value[offset] = c;
    count = newCount;
    return this;
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, int32_t i)
{
    return insert(offset, String::valueOf(i));
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, int64_t l)
{
    return insert(offset, String::valueOf(l));
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, float f)
{
    return insert(offset, String::valueOf(f));
}

AbstractStringBuilder* AbstractStringBuilder::insert(int32_t offset, double d)
{
    return insert(offset, String::valueOf(d));
}

int32_t AbstractStringBuilder::indexOf(String* str)
{
    return indexOf(str, 0);
}

int32_t AbstractStringBuilder::indexOf(String* str, int32_t fromIndex)
{
    return String::indexOf(&value, 0, count, str->toCharArray_(), 0, str->length(), fromIndex);
}

int32_t AbstractStringBuilder::lastIndexOf(String* str)
{
    return lastIndexOf(str, count);
}

int32_t AbstractStringBuilder::lastIndexOf(String* str, int32_t fromIndex)
{
    return String::lastIndexOf(&value, 0, count, str->toCharArray_(), 0, str->length(), fromIndex);
}

AbstractStringBuilder* AbstractStringBuilder::reverse()
{
    auto hasSurrogate = false;
    auto n = count - int32_t(1);
    for (auto j = (n - int32_t(1)) >> int32_t(1); j >= 0; --j) {
        auto temp = value[j];
        auto temp2 = value[n - j];
        if(!hasSurrogate) {
            hasSurrogate = (temp >= Character::MIN_SURROGATE && temp <= Character::MAX_SURROGATE) || (temp2 >= Character::MIN_SURROGATE && temp2 <= Character::MAX_SURROGATE);
        }
        value[j] = temp2;
        value[n - j] = temp;
    }
    if(hasSurrogate) {
        for (auto i = int32_t(0); i < count - int32_t(1); i++) {
            auto c2 = value[i];
            if(Character::isLowSurrogate(c2)) {
                auto c1 = value[i + int32_t(1)];
                if(Character::isHighSurrogate(c1)) {
                    value[i++] = c1;
                    value[i] = c2;
                }
            }
        }
    }
    return this;
}

vector<char16_t> AbstractStringBuilder::getValue()
{
	return value;
}
