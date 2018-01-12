#include <lang/StringBuffer.hpp>

/*
#include <java/io/ObjectInputStream_GetField.hpp>
#include <java/io/ObjectInputStream.hpp>
#include <java/io/ObjectOutputStream_PutField.hpp>
#include <java/io/ObjectOutputStream.hpp>
#include <java/io/ObjectStreamField.hpp>
*/
#include <lang/AbstractStringBuilder.hpp>
//#include <lang/ArrayStoreException.hpp>
//#include <lang/Boolean.hpp>
#include <lang/CharSequence.hpp>
//#include <lang/Class.hpp>
//#include <lang/ClassCastException.hpp>
#include <lang/Comparable.hpp>
#include <lang/Integer.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/Object.hpp>
#include <lang/String.hpp>
//#include <lang/StringIndexOutOfBoundsException.hpp>
//#include <Array.hpp>
//#include <SubArray.hpp>
//#include <ObjectArray.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::lang;
using namespace std;

StringBuffer::StringBuffer() : StringBuffer(int32_t(16))
{
}

StringBuffer::StringBuffer(int32_t capacity) : AbstractStringBuilder(capacity)
{
}

StringBuffer::StringBuffer(String* str) : AbstractStringBuilder(str->length() + int32_t(16))
{
	append(str);
}

StringBuffer::StringBuffer(CharSequence* seq) : AbstractStringBuilder(seq->length() + int32_t(16))
{
	append(seq);
}

constexpr int64_t StringBuffer::serialVersionUID;

int32_t StringBuffer::length()
{
    return count;
}

int32_t StringBuffer::capacity()
{
	return value.size();
}

void StringBuffer::ensureCapacity(int32_t minimumCapacity)
{
	if (minimumCapacity > value.size()) {
		expandCapacity(minimumCapacity);
	}
}

void StringBuffer::trimToSize()
{
    super::trimToSize();
}

void StringBuffer::setLength(int32_t newLength)
{
    super::setLength(newLength);
}

char16_t StringBuffer::charAt(int32_t index)
{
	if ((index < 0) || (index >= count))
		//throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds");

    return value[index];
}

int32_t StringBuffer::codePointAt(int32_t index)
{
    return super::codePointAt(index);
}

int32_t StringBuffer::codePointBefore(int32_t index)
{
    return super::codePointBefore(index);
}

int32_t StringBuffer::codePointCount(int32_t beginIndex, int32_t endIndex)
{
    return super::codePointCount(beginIndex, endIndex);
}

int32_t StringBuffer::offsetByCodePoints(int32_t index, int32_t codePointOffset)
{
    return super::offsetByCodePoints(index, codePointOffset);
}

void StringBuffer::getChars(int32_t srcBegin, int32_t srcEnd, vector<char16_t>* dst, int32_t dstBegin)
{
	super::getChars(srcBegin, srcEnd, dst, dstBegin);
}

void StringBuffer::setCharAt(int32_t index, char16_t ch)
{
    if((index < 0) || (index >= count))
        //throw new StringIndexOutOfBoundsException(index);
		throw std::invalid_argument("string index out of bounds");

    value[index] = ch;
}

StringBuffer* StringBuffer::append(Object* obj)
{
    super::append(String::valueOf(obj));
    return this;
}

StringBuffer* StringBuffer::append(String* str)
{
    super::append(str);
    return this;
}

StringBuffer* StringBuffer::append(StringBuffer* sb)
{
    super::append(sb);
    return this;
}

StringBuffer* StringBuffer::append(CharSequence* s)
{
	if (s == nullptr)
		s = new String("null");

	if (dynamic_cast<String*>(s) != nullptr)
		return this->append(dynamic_cast<String*>(s));

	if (dynamic_cast<StringBuffer*>(s) != nullptr)
		return this->append(dynamic_cast<StringBuffer*>(s));

	return this->append(s, int32_t(0), s->length());
}

StringBuffer* StringBuffer::append(CharSequence* s, int32_t start, int32_t end)
{
    super::append(s, start, end);
    return this;
}

StringBuffer* StringBuffer::append(::vector<char16_t> str)
{
    super::append(str);
    return this;
}

StringBuffer* StringBuffer::append(::vector<char16_t> str, int32_t offset, int32_t len)
{
    super::append(str, offset, len);
    return this;
}

StringBuffer* StringBuffer::append(bool b)
{
    super::append(b);
    return this;
}

StringBuffer* StringBuffer::append(char16_t c)
{
    super::append(c);
    return this;
}

StringBuffer* StringBuffer::append(int32_t i)
{
    super::append(i);
    return this;
}

StringBuffer* StringBuffer::appendCodePoint(int32_t codePoint)
{
    super::appendCodePoint(codePoint);
    return this;
}

StringBuffer* StringBuffer::append(int64_t lng)
{
    super::append(lng);
    return this;
}

StringBuffer* StringBuffer::append(float f)
{
    super::append(f);
    return this;
}

StringBuffer* StringBuffer::append(double d)
{
    super::append(d);
    return this;
}

StringBuffer* StringBuffer::delete_(int32_t start, int32_t end)
{
    super::delete_(start, end);
    return this;
}

StringBuffer* StringBuffer::deleteCharAt(int32_t index)
{
    super::deleteCharAt(index);
    return this;
}

StringBuffer* StringBuffer::replace(int32_t start, int32_t end, String* str)
{
    super::replace(start, end, str);
    return this;
}

String* StringBuffer::substring(int32_t start)
{
    return substring(start, count);
}

CharSequence* StringBuffer::subSequence(int32_t start, int32_t end)
{
    return super::substring(start, end);
}

String* StringBuffer::substring(int32_t start, int32_t end)
{
    return super::substring(start, end);
}

StringBuffer* StringBuffer::insert(int32_t index, ::vector<char16_t> str, int32_t offset, int32_t len)
{
    super::insert(index, str, offset, len);
    return this;
}

StringBuffer* StringBuffer::insert(int32_t offset, Object* obj)
{
    super::insert(offset, String::valueOf(obj));
    return this;
}

StringBuffer* StringBuffer::insert(int32_t offset, String* str)
{
    super::insert(offset, str);
    return this;
}

StringBuffer* StringBuffer::insert(int32_t offset, ::vector<char16_t> str)
{
    super::insert(offset, str);
    return this;
}

StringBuffer* StringBuffer::insert(int32_t dstOffset, CharSequence* s)
{
	if (s == nullptr)
		s = new String("null");

	if (dynamic_cast<String*>(s) != nullptr)
		return this->insert(dstOffset, dynamic_cast<String*>(s));

	return this->insert(dstOffset, s, int32_t(0), s->length());
}

StringBuffer* StringBuffer::insert(int32_t dstOffset, CharSequence* s, int32_t start, int32_t end)
{
    super::insert(dstOffset, s, start, end);
    return this;
}

StringBuffer* StringBuffer::insert(int32_t offset, bool b)
{
    return insert(offset, String::valueOf(b));
}

StringBuffer* StringBuffer::insert(int32_t offset, char16_t c)
{
    super::insert(offset, c);
    return this;
}

StringBuffer* StringBuffer::insert(int32_t offset, int32_t i)
{
    return insert(offset, String::valueOf(i));
}

StringBuffer* StringBuffer::insert(int32_t offset, int64_t l)
{
    return insert(offset, String::valueOf(l));
}

StringBuffer* StringBuffer::insert(int32_t offset, float f)
{
    return insert(offset, String::valueOf(f));
}

StringBuffer* StringBuffer::insert(int32_t offset, double d)
{
    return insert(offset, String::valueOf(d));
}

int32_t StringBuffer::indexOf(String* str)
{
    return indexOf(str, int32_t(0));
}

int32_t StringBuffer::indexOf(String* str, int32_t fromIndex)
{
    return String::indexOf(&value, 0, count, str->toCharArray_(), 0, str->length(), fromIndex);
}

int32_t StringBuffer::lastIndexOf(String* str)
{
    return lastIndexOf(str, count);
}

int32_t StringBuffer::lastIndexOf(String* str, int32_t fromIndex)
{
    return String::lastIndexOf(&value, 0, count, str->toCharArray_(), 0, str->length(), fromIndex);
}

StringBuffer* StringBuffer::reverse()
{
    super::reverse();
    return this;
}

String* StringBuffer::toString()
{
    return new String(value, int32_t(0), count);
}

/*
java::io::ObjectStreamFieldArray*& StringBuffer::serialPersistentFields()
{

    return serialPersistentFields_;
}
java::io::ObjectStreamFieldArray* StringBuffer::serialPersistentFields_;

void StringBuffer::writeObject(::java::io::ObjectOutputStream* s)
{
    auto fields = npc(s)->putFields();
    npc(fields)->put(u"value"_j, static_cast< Object* >(value));
    npc(fields)->put(u"count"_j, count);
    npc(fields)->put(u"shared"_j, false);
    npc(s)->writeFields();
}

void StringBuffer::readObject(::java::io::ObjectInputStream* s)
{
    auto fields = npc(s)->readFields();
    value = dynamic_cast< ::vector<char16_t> >(npc(fields)->get(u"value"_j, static_cast< Object* >(nullptr)));
    count = npc(fields)->get(u"count"_j, int32_t(0));
}


serialPersistentFields_ = (new ::java::io::ObjectStreamFieldArray({
new ::java::io::ObjectStreamField(u"value"_j, char16_tArray::class_())
, new ::java::io::ObjectStreamField(u"count"_j, Integer::TYPE())
, new ::java::io::ObjectStreamField(u"shared"_j, Boolean::TYPE())
}));

*/

AbstractStringBuilder* StringBuffer::append(AbstractStringBuilder* asb)
{
    return super::append(asb);
}
