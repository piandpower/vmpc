#include <lang/StringBuilder.hpp>

//#include <java/io/ObjectInputStream.hpp>
//#include <java/io/ObjectOutputStream.hpp>
#include <lang/AbstractStringBuilder.hpp>
#include <lang/CharSequence.hpp>
//#include <lang/ClassCastException.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/Object.hpp>
#include <lang/String.hpp>
#include <lang/StringBuffer.hpp>
//#include <Array.hpp>

using namespace moduru::lang;
using namespace std;

StringBuilder::StringBuilder() : StringBuilder(int32_t(16))
{
}

StringBuilder::StringBuilder(int32_t capacity) : AbstractStringBuilder(capacity)
{
}

StringBuilder::StringBuilder(String* str) : AbstractStringBuilder(str->length() + int32_t(16))
{
	append(str);
}

StringBuilder::StringBuilder(CharSequence* seq) : AbstractStringBuilder(seq->length() + int32_t(16))
{
	append(seq);
}

constexpr int64_t StringBuilder::serialVersionUID;

StringBuilder* StringBuilder::append(Object* obj)
{
    return append(String::valueOf(obj));
}

StringBuilder* StringBuilder::append(String* str)
{
	super::append(str);
	return this;
}

StringBuilder* StringBuilder::append(StringBuilder* sb)
{
	if (sb == nullptr)
		return append("null");

	auto len = sb->length();
	auto newcount = count + len;
	if (newcount > value.size())
		expandCapacity(newcount);

	sb->getChars(0, len, &value, count);
	count = newcount;
	return this;
}

StringBuilder* StringBuilder::append(StringBuffer* sb)
{
    super::append(sb);
    return this;
}

StringBuilder* StringBuilder::append(CharSequence* s)
{
	if (s == nullptr)
		s = new String("null");

    if(dynamic_cast< String* >(s) != nullptr)
        return this->append(dynamic_cast< String* >(s));

    if(dynamic_cast< StringBuffer* >(s) != nullptr)
        return this->append(dynamic_cast< StringBuffer* >(s));

    if(dynamic_cast< StringBuilder* >(s) != nullptr)
        return this->append(dynamic_cast< StringBuilder* >(s));

    return this->append(s, int32_t(0), s->length());
}

StringBuilder* StringBuilder::append(CharSequence* s, int32_t start, int32_t end)
{
    super::append(s, start, end);
    return this;
}

StringBuilder* StringBuilder::append(vector<char16_t> str)
{
    super::append(str);
    return this;
}

StringBuilder* StringBuilder::append(vector<char16_t> str, int32_t offset, int32_t len)
{
    super::append(str, offset, len);
    return this;
}

StringBuilder* StringBuilder::append(bool b)
{
    super::append(b);
    return this;
}

StringBuilder* StringBuilder::append(char16_t c)
{
    super::append(c);
    return this;
}

StringBuilder* StringBuilder::append(int32_t i)
{
    super::append(i);
    return this;
}

StringBuilder* StringBuilder::append(int64_t lng)
{
    super::append(lng);
    return this;
}

StringBuilder* StringBuilder::append(float f)
{
    super::append(f);
    return this;
}

StringBuilder* StringBuilder::append(double d)
{
    super::append(d);
    return this;
}

StringBuilder* StringBuilder::appendCodePoint(int32_t codePoint)
{
    super::appendCodePoint(codePoint);
    return this;
}

StringBuilder* StringBuilder::delete_(int32_t start, int32_t end)
{
    super::delete_(start, end);
    return this;
}

StringBuilder* StringBuilder::deleteCharAt(int32_t index)
{
    super::deleteCharAt(index);
    return this;
}

StringBuilder* StringBuilder::replace(int32_t start, int32_t end, String* str)
{
    super::replace(start, end, str);
    return this;
}

StringBuilder* StringBuilder::insert(int32_t index, vector<char16_t> str, int32_t offset, int32_t len)
{
    super::insert(index, str, offset, len);
    return this;
}

StringBuilder* StringBuilder::insert(int32_t offset, Object* obj)
{
    return insert(offset, String::valueOf(obj));
}

StringBuilder* StringBuilder::insert(int32_t offset, String* str)
{
    super::insert(offset, str);
    return this;
}

StringBuilder* StringBuilder::insert(int32_t offset, vector<char16_t> str)
{
    super::insert(offset, str);
    return this;
}

StringBuilder* StringBuilder::insert(int32_t dstOffset, CharSequence* s)
{
	if (s == nullptr)
		s = new String("null");

	if (dynamic_cast<String*>(s) != nullptr)
		return this->insert(dstOffset, dynamic_cast<String*>(s));

	return this->insert(dstOffset, s, int32_t(0), s->length());
}

StringBuilder* StringBuilder::insert(int32_t dstOffset, CharSequence* s, int32_t start, int32_t end)
{
    super::insert(dstOffset, s, start, end);
    return this;
}

StringBuilder* StringBuilder::insert(int32_t offset, bool b)
{
    super::insert(offset, b);
    return this;
}

StringBuilder* StringBuilder::insert(int32_t offset, char16_t c)
{
    super::insert(offset, c);
    return this;
}

StringBuilder* StringBuilder::insert(int32_t offset, int32_t i)
{
    return insert(offset, String::valueOf(i));
}

StringBuilder* StringBuilder::insert(int32_t offset, int64_t l)
{
    return insert(offset, String::valueOf(l));
}

StringBuilder* StringBuilder::insert(int32_t offset, float f)
{
    return insert(offset, String::valueOf(f));
}

StringBuilder* StringBuilder::insert(int32_t offset, double d)
{
    return insert(offset, String::valueOf(d));
}

int32_t StringBuilder::indexOf(String* str)
{
    return indexOf(str, int32_t(0));
}

int32_t StringBuilder::indexOf(String* str, int32_t fromIndex)
{
	return String::indexOf(&value, 0, count, str->toCharArray_(), 0, str->length(), fromIndex);
}

int32_t StringBuilder::lastIndexOf(String* str)
{
    return lastIndexOf(str, count);
}

int32_t StringBuilder::lastIndexOf(String* str, int32_t fromIndex)
{
    return String::lastIndexOf(&value, 0, count, str->toCharArray_(), 0, str->length(), fromIndex);
}

StringBuilder* StringBuilder::reverse()
{
    super::reverse();
    return this;
}

String* StringBuilder::toString()
{
    return new String(value, int32_t(0), count);
}

/*
void StringBuilder::writeObject(::java::io::ObjectOutputStream* s)
{
    s->defaultWriteObject();
    s->writeInt(count);
    s->writeObject(static_cast< Object* >(value));
}

void StringBuilder::readObject(::java::io::ObjectInputStream* s)
{
    s->defaultReadObject();
    count = s->readInt();
    value = dynamic_cast< vector<char16_t> >(s->readObject());
}
*/

char16_t StringBuilder::charAt(int32_t index)
{
    return AbstractStringBuilder::charAt(index);
}

int32_t StringBuilder::length()
{
    return AbstractStringBuilder::length();
}

CharSequence* StringBuilder::subSequence(int32_t start, int32_t end)
{
    return AbstractStringBuilder::subSequence(start, end);
}

AbstractStringBuilder* StringBuilder::append(AbstractStringBuilder* asb)
{
    return super::append(asb);
}
