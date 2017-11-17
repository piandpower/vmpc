#pragma once

#include <lang/AbstractStringBuilder.hpp>
#include <lang/CharSequence.hpp>

#include <vector>

namespace moduru {
	namespace lang {

		class String;

		class StringBuffer final
			: public AbstractStringBuilder
			, public virtual CharSequence
		{

		public:
			typedef AbstractStringBuilder super;

		public: /* package */
			static constexpr int64_t serialVersionUID{ int64_t(3388685877147921107LL) };

		public:
			int32_t length() override;
			int32_t capacity() override;
			void ensureCapacity(int32_t minimumCapacity) override;
			void trimToSize() override;
			void setLength(int32_t newLength) override;
			char16_t charAt(int32_t index) override;
			int32_t codePointAt(int32_t index) override;
			int32_t codePointBefore(int32_t index) override;
			int32_t codePointCount(int32_t beginIndex, int32_t endIndex) override;
			int32_t offsetByCodePoints(int32_t index, int32_t codePointOffset) override;
			void getChars(int32_t srcBegin, int32_t srcEnd, std::vector<char16_t>* dst, int32_t dstBegin) override;
			void setCharAt(int32_t index, char16_t ch) override;
			StringBuffer* append(Object* obj) override;
			StringBuffer* append(String* str) override;
			StringBuffer* append(StringBuffer* sb) override;
			StringBuffer* append(CharSequence* s) override;
			StringBuffer* append(CharSequence* s, int32_t start, int32_t end) override;
			StringBuffer* append(std::vector<char16_t> str) override;
			StringBuffer* append(std::vector<char16_t> str, int32_t offset, int32_t len) override;
			StringBuffer* append(bool b) override;
			StringBuffer* append(char16_t c) override;
			StringBuffer* append(int32_t i) override;
			StringBuffer* appendCodePoint(int32_t codePoint) override;
			StringBuffer* append(int64_t lng) override;
			StringBuffer* append(float f) override;
			StringBuffer* append(double d) override;
			StringBuffer* delete_(int32_t start, int32_t end) override;
			StringBuffer* deleteCharAt(int32_t index) override;
			StringBuffer* replace(int32_t start, int32_t end, String* str) override;
			String* substring(int32_t start) override;
			CharSequence* subSequence(int32_t start, int32_t end) override;
			String* substring(int32_t start, int32_t end) override;
			StringBuffer* insert(int32_t index, ::std::vector<char16_t> str, int32_t offset, int32_t len) override;
			StringBuffer* insert(int32_t offset, Object* obj) override;
			StringBuffer* insert(int32_t offset, String* str) override;
			StringBuffer* insert(int32_t offset, ::std::vector<char16_t> str) override;
			StringBuffer* insert(int32_t dstOffset, CharSequence* s) override;
			StringBuffer* insert(int32_t dstOffset, CharSequence* s, int32_t start, int32_t end) override;
			StringBuffer* insert(int32_t offset, bool b) override;
			StringBuffer* insert(int32_t offset, char16_t c) override;
			StringBuffer* insert(int32_t offset, int32_t i) override;
			StringBuffer* insert(int32_t offset, int64_t l) override;
			StringBuffer* insert(int32_t offset, float f) override;
			StringBuffer* insert(int32_t offset, double d) override;
			int32_t indexOf(String* str) override;
			int32_t indexOf(String* str, int32_t fromIndex) override;
			int32_t lastIndexOf(String* str) override;
			int32_t lastIndexOf(String* str, int32_t fromIndex) override;
			StringBuffer* reverse() override;
			String* toString() override;

		private:
			//static ::java::io::ObjectStreamFieldArray* serialPersistentFields_;
			//void writeObject(::java::io::ObjectOutputStream* s) /* throws(java.io.IOException) */;
			//void readObject(::java::io::ObjectInputStream* s) /* throws(java.io.IOException, ClassNotFoundException) */;

		public:
			StringBuffer();
			StringBuffer(int32_t capacity);
			StringBuffer(String* str);
			StringBuffer(CharSequence* seq);

		public:
			
		public: /* package */
			virtual AbstractStringBuilder* append(AbstractStringBuilder* asb);

		private:
			//static ::java::io::ObjectStreamFieldArray*& serialPersistentFields();
			//virtual ::Class* getClass0();
		};

	}
}
