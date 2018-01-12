#include <lang/Character.hpp>

//#include <java/io/Serializable.hpp>
//#include <lang/ArrayStoreException.hpp>
#include <lang/CharSequence.hpp>
#include <lang/CharacterCache.hpp>
//#include <lang/CharacterData.hpp>
//#include <lang/ClassCastException.hpp>
#include <lang/Comparable.hpp>
//#include <lang/IllegalArgumentException.hpp>
//#include <lang/IndexOutOfBoundsException.hpp>
//#include <lang/NullPointerException.hpp>
#include <lang/Object.hpp>
#include <lang/String.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::lang;
using namespace std;

Character::Character(char16_t value)
{
    this->value = value;
}

Character* Character::valueOf(char16_t c)
{
    if(c <= 127) {
        return (*CharacterCache::cache())[static_cast< int32_t >(c)];
    }
    return new Character(c);
}

char16_t Character::charValue()
{
    return value;
}

int32_t Character::hashCode()
{
    return static_cast< int32_t >(value);
}

bool Character::equals(Object* obj)
{
	if (dynamic_cast<Character*>(obj) != nullptr) {
		return value == (dynamic_cast<Character*>(obj))->charValue();
	}
	return false;
}

String* Character::toString()
{
	auto buf = new vector<char16_t>({ value });
	return String::valueOf(buf);
}

String* Character::toString(char16_t c)
{

    return String::valueOf(c);
}

bool Character::isValidCodePoint(int32_t codePoint)
{

    return codePoint >= MIN_CODE_POINT && codePoint <= MAX_CODE_POINT;
}

bool Character::isSupplementaryCodePoint(int32_t codePoint)
{

    return codePoint >= MIN_SUPPLEMENTARY_CODE_POINT && codePoint <= MAX_CODE_POINT;
}

bool Character::isHighSurrogate(char16_t ch)
{

    return ch >= MIN_HIGH_SURROGATE && ch <= MAX_HIGH_SURROGATE;
}

bool Character::isLowSurrogate(char16_t ch)
{

    return ch >= MIN_LOW_SURROGATE && ch <= MAX_LOW_SURROGATE;
}

bool Character::isSurrogatePair(char16_t high, char16_t low)
{

    return isHighSurrogate(high) && isLowSurrogate(low);
}

int32_t Character::charCount(int32_t codePoint)
{

    return codePoint >= MIN_SUPPLEMENTARY_CODE_POINT ? int32_t(2) : int32_t(1);
}

int32_t Character::toCodePoint(char16_t high, char16_t low)
{

    return ((high - MIN_HIGH_SURROGATE) << int32_t(10)) + (low - MIN_LOW_SURROGATE) + MIN_SUPPLEMENTARY_CODE_POINT;
}

int32_t Character::codePointAt(CharSequence* seq, int32_t index)
{

    auto c1 = seq->charAt(index++);
    if(isHighSurrogate(c1)) {
        if(index < seq->length()) {
            auto c2 = seq->charAt(index);
            if(isLowSurrogate(c2)) {
                return toCodePoint(c1, c2);
            }
        }
    }
    return c1;
}

int32_t Character::codePointAt(vector<char16_t>* a, int32_t index)
{

    return codePointAtImpl(a, index, a->size());
}

int32_t Character::codePointAt(vector<char16_t>* a, int32_t index, int32_t limit)
{

	if (index >= limit || limit < 0 || limit > a->size()) {
		//throw new IndexOutOfBoundsException();
		throw std::invalid_argument("index out of bounds");
	}
	return codePointAtImpl(a, index, limit);
}

int32_t Character::codePointAtImpl(vector<char16_t>* a, int32_t index, int32_t limit)
{

	auto c1 = (*a)[index++];
	if (isHighSurrogate(c1)) {
		if (index < limit) {
			auto c2 = (*a)[index];
			if (isLowSurrogate(c2)) {
				return toCodePoint(c1, c2);
			}
		}
	}
	return c1;
}

int32_t Character::codePointBefore(CharSequence* seq, int32_t index)
{

	auto c2 = seq->charAt(--index);
	if (isLowSurrogate(c2)) {
		if (index > 0) {
			auto c1 = seq->charAt(--index);
			if (isHighSurrogate(c1)) {
				return toCodePoint(c1, c2);
			}
		}
	}
	return c2;
}

int32_t Character::codePointBefore(vector<char16_t>* a, int32_t index)
{

	return codePointBeforeImpl(a, index, 0);
}

int32_t Character::codePointBefore(vector<char16_t>* a, int32_t index, int32_t start)
{

    if(index <= start || start < 0 || start >= a->size()) {
        //throw new IndexOutOfBoundsException();
		throw std::invalid_argument("index out of bounds");
    }
    return codePointBeforeImpl(a, index, start);
}

int32_t Character::codePointBeforeImpl(vector<char16_t>* a, int32_t index, int32_t start)
{
	auto c2 = (*a)[--index];
	if (isLowSurrogate(c2)) {
		if (index > start) {
			auto c1 = (*a)[--index];
			if (isHighSurrogate(c1)) {
				return toCodePoint(c1, c2);
			}
		}
	}
	return c2;
}

int32_t Character::toChars(int32_t codePoint, vector<char16_t>* dst, int32_t dstIndex)
{

    if(codePoint < 0 || codePoint > MAX_CODE_POINT) {
        //throw new IllegalArgumentException();
		throw std::invalid_argument("illegal argument");
    }
    if(codePoint < MIN_SUPPLEMENTARY_CODE_POINT) {
        (*dst)[dstIndex] = static_cast< char16_t >(codePoint);
        return 1;
    }
    toSurrogates(codePoint, dst, dstIndex);
    return 2;
}

vector<char16_t> Character::toChars(int32_t codePoint)
{

	if (codePoint < 0 || codePoint > MAX_CODE_POINT) {
		//throw new IllegalArgumentException();
		throw std::invalid_argument("illegal argument");
	}
	if (codePoint < MIN_SUPPLEMENTARY_CODE_POINT) {
		return vector<char16_t>({ static_cast<char16_t>(codePoint) });
	}
	auto result = vector<char16_t>(int32_t(2));
	toSurrogates(codePoint, &result, 0);
	return result;
}

void Character::toSurrogates(int32_t codePoint, vector<char16_t>* dst, int32_t index)
{

    auto offset = codePoint - MIN_SUPPLEMENTARY_CODE_POINT;
    (*dst)[index + int32_t(1)] = static_cast< char16_t >(((offset & int32_t(1023)) + MIN_LOW_SURROGATE));
    (*dst)[index] = static_cast< char16_t >(((static_cast<int32_t>(static_cast<uint32_t>(offset) >> int32_t(10))) + MIN_HIGH_SURROGATE));
}

int32_t Character::codePointCount(CharSequence* seq, int32_t beginIndex, int32_t endIndex)
{
	auto length = seq->length();
	if (beginIndex < 0 || endIndex > length || beginIndex > endIndex) {
		//throw new IndexOutOfBoundsException();
		throw std::invalid_argument("index out of bounds");
	}
	auto n = int32_t(0);
	for (auto i = beginIndex; i < endIndex; ) {
		n++;
		if (isHighSurrogate(seq->charAt(i++))) {
			if (i < endIndex && isLowSurrogate(seq->charAt(i))) {
				i++;
			}
		}
	}
	return n;
}

int32_t Character::codePointCount(vector<char16_t>* a, int32_t offset, int32_t count)
{

	if (count > a->size() - offset || offset < 0 || count < 0) {
		//throw new IndexOutOfBoundsException();
		throw std::invalid_argument("index out of bounds");
	}
	return codePointCountImpl(a, offset, count);
}

int32_t Character::codePointCountImpl(vector<char16_t>* a, int32_t offset, int32_t count)
{

    auto endIndex = offset + count;
    auto n = int32_t(0);
    for (auto i = offset; i < endIndex; ) {
        n++;
        if(isHighSurrogate((*a)[i++])) {
            if(i < endIndex && isLowSurrogate((*a)[i])) {
                i++;
            }
        }
    }
    return n;
}

int32_t Character::offsetByCodePoints(CharSequence* seq, int32_t index, int32_t codePointOffset)
{

	auto length = seq->length();
	if (index < 0 || index > length) {
		//throw new IndexOutOfBoundsException();
		throw std::invalid_argument("index out of bounds");
	}
	auto x = index;
	if (codePointOffset >= 0) {
		int32_t i;
		for (i = 0; x < length && i < codePointOffset; i++) {
			if (isHighSurrogate(seq->charAt(x++))) {
				if (x < length && isLowSurrogate(seq->charAt(x))) {
					x++;
				}
			}
		}
		if (i < codePointOffset) {
			//throw new IndexOutOfBoundsException();
			throw std::invalid_argument("index out of bounds");
		}
	}
	else {
		int32_t i;
		for (i = codePointOffset; x > 0 && i < 0; i++) {
			if (isLowSurrogate(seq->charAt(--x))) {
				if (x > 0 && isHighSurrogate(seq->charAt(x - int32_t(1)))) {
					x--;
				}
			}
		}
		if (i < 0) {
			//throw new IndexOutOfBoundsException();
			throw std::invalid_argument("index out of bounds");
		}
	}
	return x;
}

int32_t Character::offsetByCodePoints(vector<char16_t>* a, int32_t start, int32_t count, int32_t index, int32_t codePointOffset)
{

    if(count > a->size() - start || start < 0 || count < 0 || index < start || index > start + count) {
        //throw new IndexOutOfBoundsException();
		throw std::invalid_argument("index out of bounds");
    }
    return offsetByCodePointsImpl(a, start, count, index, codePointOffset);
}

int32_t Character::offsetByCodePointsImpl(vector<char16_t>* a, int32_t start, int32_t count, int32_t index, int32_t codePointOffset)
{

	auto x = index;
	if (codePointOffset >= 0) {
		auto limit = start + count;
		int32_t i;
		for (i = 0; x < limit && i < codePointOffset; i++) {
			if (isHighSurrogate((*a)[x++])) {
				if (x < limit && isLowSurrogate((*a)[x])) {
					x++;
				}
			}
		}
		if (i < codePointOffset) {
			//throw new IndexOutOfBoundsException();
			throw std::invalid_argument("index out of bounds");
		}
	}
	else {
		int32_t i;
		for (i = codePointOffset; x > start && i < 0; i++) {
			if (isLowSurrogate((*a)[--x])) {
				if (x > start && isHighSurrogate((*a)[x - int32_t(1)])) {
					x--;
				}
			}
		}
		if (i < 0) {
			//throw new IndexOutOfBoundsException();
			throw std::invalid_argument("index out of bounds");
		}
	}
	return x;
}

bool Character::isLowerCase(char16_t ch)
{

    return isLowerCase(static_cast< int32_t >(ch));
}

bool Character::isLowerCase(int32_t codePoint)
{

    return getType(codePoint) == Character::LOWERCASE_LETTER;
}

bool Character::isUpperCase(char16_t ch)
{

    return isUpperCase(static_cast< int32_t >(ch));
}

bool Character::isUpperCase(int32_t codePoint)
{

    return getType(codePoint) == Character::UPPERCASE_LETTER;
}

bool Character::isTitleCase(char16_t ch)
{

    return isTitleCase(static_cast< int32_t >(ch));
}

bool Character::isTitleCase(int32_t codePoint)
{

    return getType(codePoint) == Character::TITLECASE_LETTER;
}

bool Character::isDigit(char16_t ch)
{

    return isDigit(static_cast< int32_t >(ch));
}

bool Character::isDigit(int32_t codePoint)
{

    return getType(codePoint) == Character::DECIMAL_DIGIT_NUMBER;
}

bool Character::isDefined(char16_t ch)
{

    return isDefined(static_cast< int32_t >(ch));
}

bool Character::isDefined(int32_t codePoint)
{

    return getType(codePoint) != Character::UNASSIGNED;
}

bool Character::isLetter(char16_t ch)
{

    return isLetter(static_cast< int32_t >(ch));
}

bool Character::isLetter(int32_t codePoint)
{

    return ((((int32_t(1) << Character::UPPERCASE_LETTER) | (int32_t(1) << Character::LOWERCASE_LETTER) | (int32_t(1) << Character::TITLECASE_LETTER)| (int32_t(1) << Character::MODIFIER_LETTER)| (int32_t(1) << Character::OTHER_LETTER)) >> getType(codePoint)) & int32_t(1)) != 0;
}

bool Character::isLetterOrDigit(char16_t ch)
{

    return isLetterOrDigit(static_cast< int32_t >(ch));
}

bool Character::isLetterOrDigit(int32_t codePoint)
{

    return ((((int32_t(1) << Character::UPPERCASE_LETTER) | (int32_t(1) << Character::LOWERCASE_LETTER) | (int32_t(1) << Character::TITLECASE_LETTER)| (int32_t(1) << Character::MODIFIER_LETTER)| (int32_t(1) << Character::OTHER_LETTER)| (int32_t(1) << Character::DECIMAL_DIGIT_NUMBER)) >> getType(codePoint)) & int32_t(1)) != 0;
}

bool Character::isJavaLetter(char16_t ch)
{

    return isJavaIdentifierStart(ch);
}

bool Character::isJavaLetterOrDigit(char16_t ch)
{

    return isJavaIdentifierPart(ch);
}

bool Character::isJavaIdentifierStart(char16_t ch)
{

    return isJavaIdentifierStart(static_cast< int32_t >(ch));
}

bool Character::isJavaIdentifierStart(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->isJavaIdentifierStart(codePoint);
	return false;
}

bool Character::isJavaIdentifierPart(char16_t ch)
{

    return isJavaIdentifierPart(static_cast< int32_t >(ch));
}

bool Character::isJavaIdentifierPart(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->isJavaIdentifierPart(codePoint);
	return false;
}

bool Character::isUnicodeIdentifierStart(char16_t ch)
{

    return isUnicodeIdentifierStart(static_cast< int32_t >(ch));
}

bool Character::isUnicodeIdentifierStart(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->isUnicodeIdentifierStart(codePoint);
	return false;
}

bool Character::isUnicodeIdentifierPart(char16_t ch)
{

    return isUnicodeIdentifierPart(static_cast< int32_t >(ch));
}

bool Character::isUnicodeIdentifierPart(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->isUnicodeIdentifierPart(codePoint);
	return false;
}

bool Character::isIdentifierIgnorable(char16_t ch)
{

    return isIdentifierIgnorable(static_cast< int32_t >(ch));
}

bool Character::isIdentifierIgnorable(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->isIdentifierIgnorable(codePoint);
	return false;
}

char16_t Character::toLowerCase(char16_t ch)
{

    return static_cast< char16_t >(toLowerCase(static_cast< int32_t >(ch)));
}

int32_t Character::toLowerCase(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->toLowerCase(codePoint);
	return codePoint;
}

char16_t Character::toUpperCase(char16_t ch)
{

    return static_cast< char16_t >(toUpperCase(static_cast< int32_t >(ch)));
}

int32_t Character::toUpperCase(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->toUpperCase(codePoint);
	return codePoint;
}

char16_t Character::toTitleCase(char16_t ch)
{

    return static_cast< char16_t >(toTitleCase(static_cast< int32_t >(ch)));
}

int32_t Character::toTitleCase(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->toTitleCase(codePoint);
	return codePoint;
}

int32_t Character::digit(char16_t ch, int32_t radix)
{

    return digit(static_cast< int32_t >(ch), radix);
}

int32_t Character::digit(int32_t codePoint, int32_t radix)
{

    //return CharacterData::of(codePoint)->digit(codePoint, radix);
	return codePoint;
}

int32_t Character::getNumericValue(char16_t ch)
{

    return getNumericValue(static_cast< int32_t >(ch));
}

int32_t Character::getNumericValue(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->getNumericValue(codePoint);
	return codePoint;
}

bool Character::isSpace(char16_t ch)
{

    return (ch <= 32) && (((((int64_t(1LL) << int32_t(9)) | (int64_t(1LL) << int32_t(10)) | (int64_t(1LL) << int32_t(12))| (int64_t(1LL) << int32_t(13))| (int64_t(1LL) << int32_t(32))) >> ch) & int64_t(1LL)) != 0);
}

bool Character::isSpaceChar(char16_t ch)
{

    return isSpaceChar(static_cast< int32_t >(ch));
}

bool Character::isSpaceChar(int32_t codePoint)
{

    return ((((int32_t(1) << Character::SPACE_SEPARATOR) | (int32_t(1) << Character::LINE_SEPARATOR) | (int32_t(1) << Character::PARAGRAPH_SEPARATOR)) >> getType(codePoint)) & int32_t(1)) != 0;
}

bool Character::isWhitespace(char16_t ch)
{

    return isWhitespace(static_cast< int32_t >(ch));
}

bool Character::isWhitespace(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->isWhitespace(codePoint);
	return false;
}

bool Character::isISOControl(char16_t ch)
{

    return isISOControl(static_cast< int32_t >(ch));
}

bool Character::isISOControl(int32_t codePoint)
{

    return (codePoint >= 0 && codePoint <= 31) || (codePoint >= 127 && codePoint <= 159);
}

int32_t Character::getType(char16_t ch)
{

    return getType(static_cast< int32_t >(ch));
}

int32_t Character::getType(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->getType(codePoint);
	return codePoint;
}

char16_t Character::forDigit(int32_t digit, int32_t radix)
{

    if((digit >= radix) || (digit < 0)) {
        return char16_t(0x0000);
    }
    if((radix < Character::MIN_RADIX) || (radix > Character::MAX_RADIX)) {
        return char16_t(0x0000);
    }
    if(digit < 10) {
        return static_cast< char16_t >((u'0' + digit));
    }
    return static_cast< char16_t >((u'a' - int32_t(10) + digit));
}

int8_t Character::getDirectionality(char16_t ch)
{

    return getDirectionality(static_cast< int32_t >(ch));
}

int8_t Character::getDirectionality(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->getDirectionality(codePoint);
	return -1;
}

bool Character::isMirrored(char16_t ch)
{

    return isMirrored(static_cast< int32_t >(ch));
}

bool Character::isMirrored(int32_t codePoint)
{

    //return CharacterData::of(codePoint)->isMirrored(codePoint);
	return false;
}

int32_t Character::compareTo(Character* anotherCharacter)
{
    return this->value - anotherCharacter->value;
}

int32_t Character::compareTo(Object* o)
{
    return compareTo(dynamic_cast< Character* >(o));
}

int32_t Character::toUpperCaseEx(int32_t codePoint)
{
    //return CharacterData::of(codePoint)->toUpperCaseEx(codePoint);
	return codePoint;
}

vector<char16_t> Character::toUpperCaseCharArray_(int32_t codePoint)
{
	//return CharacterData::of(codePoint)->toUpperCaseCharArray_(codePoint);
	return vector<char16_t>();
}

constexpr int32_t Character::SIZE;

char16_t Character::reverseBytes(char16_t ch)
{

    return static_cast< char16_t >((((ch & int32_t(65280)) >> int32_t(8)) | (ch << int32_t(8))));
}
