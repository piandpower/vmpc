// Generated from /imagemap/src/java/lang/StringBuilder.java

#pragma once

#include <lang/AbstractStringBuilder.hpp>
//#include <java/io/Serializable.hpp>
#include <lang/CharSequence.hpp>

namespace moduru {
	namespace lang {

		class StringBuilder final
			: public AbstractStringBuilder
			, public virtual CharSequence
		{

		public:
			typedef AbstractStringBuilder super;

		public: /* package */
			static constexpr int64_t serialVersionUID { int64_t(4383685877147921099LL) };

		public:
			StringBuilder* append(Object* obj) override;
			StringBuilder* append(String* str) override;

		private:
			StringBuilder* append(StringBuilder* sb);

		public:
			StringBuilder* append(StringBuffer* sb) override;
			StringBuilder* append(CharSequence* s) override;
			StringBuilder* append(CharSequence* s, int32_t start, int32_t end) override;
			StringBuilder* append(std::vector<char16_t> str) override;
			StringBuilder* append(std::vector<char16_t> str, int32_t offset, int32_t len) override;
			StringBuilder* append(bool b) override;
			StringBuilder* append(char16_t c) override;
			StringBuilder* append(int32_t i) override;
			StringBuilder* append(int64_t lng) override;
			StringBuilder* append(float f) override;
			StringBuilder* append(double d) override;
			StringBuilder* appendCodePoint(int32_t codePoint) override;
			StringBuilder* delete_(int32_t start, int32_t end) override;
			StringBuilder* deleteCharAt(int32_t index) override;
			StringBuilder* replace(int32_t start, int32_t end, String* str) override;
			StringBuilder* insert(int32_t index, std::vector<char16_t> str, int32_t offset, int32_t len) override;
			StringBuilder* insert(int32_t offset, Object* obj) override;
			StringBuilder* insert(int32_t offset, String* str) override;
			StringBuilder* insert(int32_t offset, std::vector<char16_t> str) override;
			StringBuilder* insert(int32_t dstOffset, CharSequence* s) override;
			StringBuilder* insert(int32_t dstOffset, CharSequence* s, int32_t start, int32_t end) override;
			StringBuilder* insert(int32_t offset, bool b) override;
			StringBuilder* insert(int32_t offset, char16_t c) override;
			StringBuilder* insert(int32_t offset, int32_t i) override;
			StringBuilder* insert(int32_t offset, int64_t l) override;
			StringBuilder* insert(int32_t offset, float f) override;
			StringBuilder* insert(int32_t offset, double d) override;
			int32_t indexOf(String* str) override;
			int32_t indexOf(String* str, int32_t fromIndex) override;
			int32_t lastIndexOf(String* str) override;
			int32_t lastIndexOf(String* str, int32_t fromIndex) override;
			StringBuilder* reverse() override;
			String* toString() override;

		private:
			//void writeObject(::java::io::ObjectOutputStream* s) /* throws(java.io.IOException) */;
			//void readObject(::java::io::ObjectInputStream* s) /* throws(java.io.IOException, ClassNotFoundException) */;


		public:
			StringBuilder();
			StringBuilder(int32_t capacity);
			StringBuilder(String* str);
			StringBuilder(CharSequence* seq);

		public:
			virtual char16_t charAt(int32_t index);
			virtual int32_t length();
			virtual CharSequence* subSequence(int32_t start, int32_t end);

		public: /* package */
			virtual AbstractStringBuilder* append(AbstractStringBuilder* asb);

		};

	}
}
