#pragma once

#include <lang/Object.hpp>
//#include <java/io/Serializable.hpp>
#include <lang/Comparable.hpp>

#include <vector>
#include <cstdint>

namespace moduru {
	namespace lang {

		class String;
		class CharSequence;

		class Character final
			: public virtual Object
			, public Comparable
		{

		public:
			typedef Object super;
			static constexpr int32_t MIN_RADIX{ int32_t(2) };
			static constexpr int32_t MAX_RADIX{ int32_t(36) };
			static constexpr char16_t MIN_VALUE{ char16_t(0x0000) };
			static constexpr char16_t MAX_VALUE{ char16_t(0xffff) };
			static constexpr int8_t UNASSIGNED{ int8_t(0) };
			static constexpr int8_t UPPERCASE_LETTER{ int8_t(1) };
			static constexpr int8_t LOWERCASE_LETTER{ int8_t(2) };
			static constexpr int8_t TITLECASE_LETTER{ int8_t(3) };
			static constexpr int8_t MODIFIER_LETTER{ int8_t(4) };
			static constexpr int8_t OTHER_LETTER{ int8_t(5) };
			static constexpr int8_t NON_SPACING_MARK{ int8_t(6) };
			static constexpr int8_t ENCLOSING_MARK{ int8_t(7) };
			static constexpr int8_t COMBINING_SPACING_MARK{ int8_t(8) };
			static constexpr int8_t DECIMAL_DIGIT_NUMBER{ int8_t(9) };
			static constexpr int8_t LETTER_NUMBER{ int8_t(10) };
			static constexpr int8_t OTHER_NUMBER{ int8_t(11) };
			static constexpr int8_t SPACE_SEPARATOR{ int8_t(12) };
			static constexpr int8_t LINE_SEPARATOR{ int8_t(13) };
			static constexpr int8_t PARAGRAPH_SEPARATOR{ int8_t(14) };
			static constexpr int8_t CONTROL{ int8_t(15) };
			static constexpr int8_t FORMAT{ int8_t(16) };
			static constexpr int8_t PRIVATE_USE{ int8_t(18) };
			static constexpr int8_t SURROGATE{ int8_t(19) };
			static constexpr int8_t DASH_PUNCTUATION{ int8_t(20) };
			static constexpr int8_t START_PUNCTUATION{ int8_t(21) };
			static constexpr int8_t END_PUNCTUATION{ int8_t(22) };
			static constexpr int8_t CONNECTOR_PUNCTUATION{ int8_t(23) };
			static constexpr int8_t OTHER_PUNCTUATION{ int8_t(24) };
			static constexpr int8_t MATH_SYMBOL{ int8_t(25) };
			static constexpr int8_t CURRENCY_SYMBOL{ int8_t(26) };
			static constexpr int8_t MODIFIER_SYMBOL{ int8_t(27) };
			static constexpr int8_t OTHER_SYMBOL{ int8_t(28) };
			static constexpr int8_t INITIAL_QUOTE_PUNCTUATION{ int8_t(29) };
			static constexpr int8_t FINAL_QUOTE_PUNCTUATION{ int8_t(30) };

		public: /* package */
			static constexpr int32_t ERROR{ int32_t(-1) };

		public:
			static constexpr int8_t DIRECTIONALITY_UNDEFINED{ int8_t(-1) };
			static constexpr int8_t DIRECTIONALITY_LEFT_TO_RIGHT{ int8_t(0) };
			static constexpr int8_t DIRECTIONALITY_RIGHT_TO_LEFT{ int8_t(1) };
			static constexpr int8_t DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC{ int8_t(2) };
			static constexpr int8_t DIRECTIONALITY_EUROPEAN_NUMBER{ int8_t(3) };
			static constexpr int8_t DIRECTIONALITY_EUROPEAN_NUMBER_SEPARATOR{ int8_t(4) };
			static constexpr int8_t DIRECTIONALITY_EUROPEAN_NUMBER_TERMINATOR{ int8_t(5) };
			static constexpr int8_t DIRECTIONALITY_ARABIC_NUMBER{ int8_t(6) };
			static constexpr int8_t DIRECTIONALITY_COMMON_NUMBER_SEPARATOR{ int8_t(7) };
			static constexpr int8_t DIRECTIONALITY_NONSPACING_MARK{ int8_t(8) };
			static constexpr int8_t DIRECTIONALITY_BOUNDARY_NEUTRAL{ int8_t(9) };
			static constexpr int8_t DIRECTIONALITY_PARAGRAPH_SEPARATOR{ int8_t(10) };
			static constexpr int8_t DIRECTIONALITY_SEGMENT_SEPARATOR{ int8_t(11) };
			static constexpr int8_t DIRECTIONALITY_WHITESPACE{ int8_t(12) };
			static constexpr int8_t DIRECTIONALITY_OTHER_NEUTRALS{ int8_t(13) };
			static constexpr int8_t DIRECTIONALITY_LEFT_TO_RIGHT_EMBEDDING{ int8_t(14) };
			static constexpr int8_t DIRECTIONALITY_LEFT_TO_RIGHT_OVERRIDE{ int8_t(15) };
			static constexpr int8_t DIRECTIONALITY_RIGHT_TO_LEFT_EMBEDDING{ int8_t(16) };
			static constexpr int8_t DIRECTIONALITY_RIGHT_TO_LEFT_OVERRIDE{ int8_t(17) };
			static constexpr int8_t DIRECTIONALITY_POP_DIRECTIONAL_FORMAT{ int8_t(18) };
			static constexpr char16_t MIN_HIGH_SURROGATE{ char16_t(0xd800) };
			static constexpr char16_t MAX_HIGH_SURROGATE{ char16_t(0xdbff) };
			static constexpr char16_t MIN_LOW_SURROGATE{ char16_t(0xdc00) };
			static constexpr char16_t MAX_LOW_SURROGATE{ char16_t(0xdfff) };
			static constexpr char16_t MIN_SURROGATE{ char16_t(0xd800) };
			static constexpr char16_t MAX_SURROGATE{ char16_t(0xdfff) };
			static constexpr int32_t MIN_SUPPLEMENTARY_CODE_POINT{ int32_t(65536) };
			static constexpr int32_t MIN_CODE_POINT{ int32_t(0) };
			static constexpr int32_t MAX_CODE_POINT{ int32_t(1114111) };

		private:
			char16_t value{};
			static constexpr int64_t serialVersionUID{ int64_t(3786198910865385080LL) };

		public:
			static Character* valueOf(char16_t c);
			char16_t charValue();
			int32_t hashCode() override;
			bool equals(Object* obj) override;
			String* toString() override;
			static String* toString(char16_t c);
			static bool isValidCodePoint(int32_t codePoint);
			static bool isSupplementaryCodePoint(int32_t codePoint);
			static bool isHighSurrogate(char16_t ch);
			static bool isLowSurrogate(char16_t ch);
			static bool isSurrogatePair(char16_t high, char16_t low);
			static int32_t charCount(int32_t codePoint);
			static int32_t toCodePoint(char16_t high, char16_t low);
			static int32_t codePointAt(CharSequence* seq, int32_t index);
			static int32_t codePointAt(std::vector<char16_t>* a, int32_t index);
			static int32_t codePointAt(std::vector<char16_t>* a, int32_t index, int32_t limit);

		public: /* package */
			static int32_t codePointAtImpl(std::vector<char16_t>* a, int32_t index, int32_t limit);

		public:
			static int32_t codePointBefore(CharSequence* seq, int32_t index);
			static int32_t codePointBefore(std::vector<char16_t>* a, int32_t index);
			static int32_t codePointBefore(std::vector<char16_t>* a, int32_t index, int32_t start);

		public: /* package */
			static int32_t codePointBeforeImpl(std::vector<char16_t>* a, int32_t index, int32_t start);

		public:
			static int32_t toChars(int32_t codePoint, std::vector<char16_t>* dst, int32_t dstIndex);
			static std::vector<char16_t> toChars(int32_t codePoint);

		public: /* package */
			static void toSurrogates(int32_t codePoint, std::vector<char16_t>* dst, int32_t index);

		public:
			static int32_t codePointCount(CharSequence* seq, int32_t beginIndex, int32_t endIndex);
			static int32_t codePointCount(std::vector<char16_t>* a, int32_t offset, int32_t count);

		public: /* package */
			static int32_t codePointCountImpl(std::vector<char16_t>* a, int32_t offset, int32_t count);

		public:
			static int32_t offsetByCodePoints(CharSequence* seq, int32_t index, int32_t codePointOffset);
			static int32_t offsetByCodePoints(std::vector<char16_t>* a, int32_t start, int32_t count, int32_t index, int32_t codePointOffset);

		public: /* package */
			static int32_t offsetByCodePointsImpl(std::vector<char16_t>* a, int32_t start, int32_t count, int32_t index, int32_t codePointOffset);

		public:
			static bool isLowerCase(char16_t ch);
			static bool isLowerCase(int32_t codePoint);
			static bool isUpperCase(char16_t ch);
			static bool isUpperCase(int32_t codePoint);
			static bool isTitleCase(char16_t ch);
			static bool isTitleCase(int32_t codePoint);
			static bool isDigit(char16_t ch);
			static bool isDigit(int32_t codePoint);
			static bool isDefined(char16_t ch);
			static bool isDefined(int32_t codePoint);
			static bool isLetter(char16_t ch);
			static bool isLetter(int32_t codePoint);
			static bool isLetterOrDigit(char16_t ch);
			static bool isLetterOrDigit(int32_t codePoint);
			static bool isJavaLetter(char16_t ch);
			static bool isJavaLetterOrDigit(char16_t ch);
			static bool isJavaIdentifierStart(char16_t ch);
			static bool isJavaIdentifierStart(int32_t codePoint);
			static bool isJavaIdentifierPart(char16_t ch);
			static bool isJavaIdentifierPart(int32_t codePoint);
			static bool isUnicodeIdentifierStart(char16_t ch);
			static bool isUnicodeIdentifierStart(int32_t codePoint);
			static bool isUnicodeIdentifierPart(char16_t ch);
			static bool isUnicodeIdentifierPart(int32_t codePoint);
			static bool isIdentifierIgnorable(char16_t ch);
			static bool isIdentifierIgnorable(int32_t codePoint);
			static char16_t toLowerCase(char16_t ch);
			static int32_t toLowerCase(int32_t codePoint);
			static char16_t toUpperCase(char16_t ch);
			static int32_t toUpperCase(int32_t codePoint);
			static char16_t toTitleCase(char16_t ch);
			static int32_t toTitleCase(int32_t codePoint);
			static int32_t digit(char16_t ch, int32_t radix);
			static int32_t digit(int32_t codePoint, int32_t radix);
			static int32_t getNumericValue(char16_t ch);
			static int32_t getNumericValue(int32_t codePoint);
			static bool isSpace(char16_t ch);
			static bool isSpaceChar(char16_t ch);
			static bool isSpaceChar(int32_t codePoint);
			static bool isWhitespace(char16_t ch);
			static bool isWhitespace(int32_t codePoint);
			static bool isISOControl(char16_t ch);
			static bool isISOControl(int32_t codePoint);
			static int32_t getType(char16_t ch);
			static int32_t getType(int32_t codePoint);
			static char16_t forDigit(int32_t digit, int32_t radix);
			static int8_t getDirectionality(char16_t ch);
			static int8_t getDirectionality(int32_t codePoint);
			static bool isMirrored(char16_t ch);
			static bool isMirrored(int32_t codePoint);
			int32_t compareTo(Character* anotherCharacter);

		public: /* package */
			static int32_t toUpperCaseEx(int32_t codePoint);
			static std::vector<char16_t> toUpperCaseCharArray_(int32_t codePoint);

		public:
			static constexpr int32_t SIZE{ int32_t(16) };
			static char16_t reverseBytes(char16_t ch);

			Character(char16_t value);

		public:
			
			virtual int32_t compareTo(Object* o) override;

		private:
			//friend class Subset;
			//friend class UnicodeBlock;
			//friend class CharacterCache;
		};

	}
}
