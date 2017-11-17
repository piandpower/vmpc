#pragma once
#include <lang/Object.hpp>
#include <lang/Appendable.hpp>
#include <lang/CharSequence.hpp>

#include <cstdint>
#include <vector>

namespace moduru {
	namespace lang {

		class String;
		class StringBuffer;

		class AbstractStringBuilder
			: public virtual Object
			, public virtual Appendable
			, public virtual CharSequence
		{

		public:
			typedef Object super;

		public:
			std::vector<char16_t> value{};
			int32_t count{ 0 };

		public:
			int32_t length() override;
			virtual int32_t capacity();
			virtual void ensureCapacity(int32_t minimumCapacity);

		public:
			virtual void expandCapacity(int32_t minimumCapacity);

		public:
			virtual void trimToSize();
			virtual void setLength(int32_t newLength);
			char16_t charAt(int32_t index) override;
			virtual int32_t codePointAt(int32_t index);
			virtual int32_t codePointBefore(int32_t index);
			virtual int32_t codePointCount(int32_t beginIndex, int32_t endIndex);
			virtual int32_t offsetByCodePoints(int32_t index, int32_t codePointOffset);
			virtual void getChars(int32_t srcBegin, int32_t srcEnd, std::vector<char16_t>* dst, int32_t dstBegin);
			virtual void setCharAt(int32_t index, char16_t ch);
			virtual AbstractStringBuilder* append(Object* obj);
			virtual AbstractStringBuilder* append(String* str);
			virtual AbstractStringBuilder* append(StringBuffer* sb);
			AbstractStringBuilder* append(CharSequence* s) override;
			AbstractStringBuilder* append(CharSequence* s, int32_t start, int32_t end) override;
			virtual AbstractStringBuilder* append(std::vector<char16_t> str);
			virtual AbstractStringBuilder* append(std::vector<char16_t> str, int32_t offset, int32_t len);
			virtual AbstractStringBuilder* append(bool b);
			AbstractStringBuilder* append(char16_t c) override;
			virtual AbstractStringBuilder* append(int32_t i);
			virtual AbstractStringBuilder* append(int64_t l);
			virtual AbstractStringBuilder* append(float f);
			virtual AbstractStringBuilder* append(double d);
			virtual AbstractStringBuilder* delete_(int32_t start, int32_t end);
			virtual AbstractStringBuilder* appendCodePoint(int32_t codePoint);
			virtual AbstractStringBuilder* deleteCharAt(int32_t index);
			virtual AbstractStringBuilder* replace(int32_t start, int32_t end, String* str);
			virtual String* substring(int32_t start);
			CharSequence* subSequence(int32_t start, int32_t end) override;
			virtual String* substring(int32_t start, int32_t end);
			virtual AbstractStringBuilder* insert(int32_t index, std::vector<char16_t> str, int32_t offset, int32_t len);
			virtual AbstractStringBuilder* insert(int32_t offset, Object* obj);
			virtual AbstractStringBuilder* insert(int32_t offset, String* str);
			virtual AbstractStringBuilder* insert(int32_t offset, std::vector<char16_t> str);
			virtual AbstractStringBuilder* insert(int32_t dstOffset, CharSequence* s);
			virtual AbstractStringBuilder* insert(int32_t dstOffset, CharSequence* s, int32_t start, int32_t end);
			virtual AbstractStringBuilder* insert(int32_t offset, bool b);
			virtual AbstractStringBuilder* insert(int32_t offset, char16_t c);
			virtual AbstractStringBuilder* insert(int32_t offset, int32_t i);
			virtual AbstractStringBuilder* insert(int32_t offset, int64_t l);
			virtual AbstractStringBuilder* insert(int32_t offset, float f);
			virtual AbstractStringBuilder* insert(int32_t offset, double d);
			virtual int32_t indexOf(String* str);
			virtual int32_t indexOf(String* str, int32_t fromIndex);
			virtual int32_t lastIndexOf(String* str);
			virtual int32_t lastIndexOf(String* str, int32_t fromIndex);
			virtual AbstractStringBuilder* reverse();
			/*String* toString(); (already declared) */

		public: /* package */
			std::vector<char16_t> getValue();

			AbstractStringBuilder();
			AbstractStringBuilder(int32_t capacity);

		};

	}
}
