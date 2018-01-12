#include <util/StringTokenizer.hpp>

#include <lang/Character.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/String.hpp>
//#include <util/NoSuchElementException.hpp>
//#include <Array.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::util;
using namespace std;

moduru::util::StringTokenizer::StringTokenizer(moduru::lang::String* str, moduru::lang::String* delim, bool returnDelims)
{
	currentPosition = 0;
	newPosition = -int32_t(1);
	delimsChanged = false;
	this->str = str;
	maxPosition = str->length();
	delimiters = delim;
	retDelims = returnDelims;
	setMaxDelimCodePoint();
}

moduru::util::StringTokenizer::StringTokenizer(moduru::lang::String* str, moduru::lang::String* delim) : StringTokenizer(str, delim, false)
{
}

moduru::util::StringTokenizer::StringTokenizer(moduru::lang::String* str)
: StringTokenizer(str, new moduru::lang::String(" \t\n\r\f"), false)
{
}

void moduru::util::StringTokenizer::setMaxDelimCodePoint()
{
    if(delimiters == nullptr) {
        maxDelimCodePoint = 0;
        return;
    }
    auto m = int32_t(0);
    int32_t c;
    auto count = int32_t(0);
    for (auto i = int32_t(0); i < delimiters->length(); i += moduru::lang::Character::charCount(c)) {
        c = delimiters->charAt(i);
        if(c >= moduru::lang::Character::MIN_HIGH_SURROGATE && c <= moduru::lang::Character::MAX_LOW_SURROGATE) {
            c = delimiters->codePointAt(i);
            hasSurrogates = true;
        }
        if(m < c)
            m = c;

        count++;
    }
    maxDelimCodePoint = m;
    if(hasSurrogates) {
        delimiterCodePoints = new vector<int32_t>(count);
        for (int32_t i = int32_t(0), j = int32_t(0); i < count; i++, j += moduru::lang::Character::charCount(c)) {
            c = delimiters->codePointAt(j);
            (*delimiterCodePoints)[i] = c;
        }
    }
}

int32_t moduru::util::StringTokenizer::skipDelimiters(int32_t startPos)
{
	if (delimiters == nullptr)
		//throw new moduru::lang::NullPointerException();
		throw std::invalid_argument("nullptr");

    auto position = startPos;
    while (!retDelims && position < maxPosition) {
        if(!hasSurrogates) {
            auto c = str->charAt(position);
            if((c > maxDelimCodePoint) || (delimiters->indexOf(static_cast< int32_t >(c)) < 0))
                break;

            position++;
        } else {
            auto c = str->codePointAt(position);
            if((c > maxDelimCodePoint) || !isDelimiter(c)) {
                break;
            }
            position += moduru::lang::Character::charCount(c);
        }
    }
    return position;
}

int32_t moduru::util::StringTokenizer::scanToken(int32_t startPos)
{
    auto position = startPos;
    while (position < maxPosition) {
        if(!hasSurrogates) {
            auto c = str->charAt(position);
            if((c <= maxDelimCodePoint) && (delimiters->indexOf(static_cast< int32_t >(c)) >= 0))
                break;

            position++;
        } else {
            auto c = str->codePointAt(position);
            if((c <= maxDelimCodePoint) && isDelimiter(c))
                break;

            position += moduru::lang::Character::charCount(c);
        }
    }
    if(retDelims && (startPos == position)) {
        if(!hasSurrogates) {
            auto c = str->charAt(position);
            if((c <= maxDelimCodePoint) && (delimiters->indexOf(static_cast< int32_t >(c)) >= 0))
                position++;

        } else {
            auto c = str->codePointAt(position);
            if((c <= maxDelimCodePoint) && isDelimiter(c))
                position += moduru::lang::Character::charCount(c);

        }
    }
    return position;
}

bool moduru::util::StringTokenizer::isDelimiter(int32_t codePoint)
{
	for (auto i = int32_t(0); i < delimiterCodePoints->size(); i++) {
		if ((*delimiterCodePoints)[i] == codePoint) {
			return true;
		}
	}
	return false;
}

bool moduru::util::StringTokenizer::hasMoreTokens()
{
    newPosition = skipDelimiters(currentPosition);
    return (newPosition < maxPosition);
}

moduru::lang::String* moduru::util::StringTokenizer::nextToken()
{
    currentPosition = (newPosition >= 0 && !delimsChanged) ? newPosition : skipDelimiters(currentPosition);
    delimsChanged = false;
    newPosition = -int32_t(1);
	if (currentPosition >= maxPosition)
		//throw new NoSuchElementException();
		throw std::invalid_argument("no such element");

    auto start = currentPosition;
    currentPosition = scanToken(currentPosition);
    return str->substring(start, currentPosition);
}

moduru::lang::String* moduru::util::StringTokenizer::nextToken(moduru::lang::String* delim)
{
    delimiters = delim;
    delimsChanged = true;
    setMaxDelimCodePoint();
    return nextToken();
}

bool moduru::util::StringTokenizer::hasMoreElements()
{
    return hasMoreTokens();
}

moduru::lang::Object* moduru::util::StringTokenizer::nextElement()
{
    return nextToken();
}

int32_t moduru::util::StringTokenizer::countTokens()
{
	auto count = int32_t(0);
	auto currpos = currentPosition;
	while (currpos < maxPosition) {
		currpos = skipDelimiters(currpos);
		if (currpos >= maxPosition)
			break;

		currpos = scanToken(currpos);
		count++;
	}
	return count;
}
