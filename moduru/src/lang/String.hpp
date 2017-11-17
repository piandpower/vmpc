#pragma once

#include <stddef.h>
#include <lang/Object.hpp>
#include <lang/Comparable.hpp>
#include <lang/CharSequence.hpp>

#include <vector>
#include <string>

namespace moduru {

	namespace nio {
		namespace charset {
			class Charset;
		}
	}

	namespace util {
		class Comparator;
	}

	namespace lang {

		class StringBuffer;
		class StringBuilder;

		class String final
			: public virtual Object
			, public Comparable
			, public CharSequence
		{

		public:
			typedef Object super;

		private:
			std::vector<char16_t> value{};
			int32_t offset{ 0 };
			int32_t count{ 0 };
			int32_t hash{ 0 };
			static constexpr int64_t serialVersionUID{ int64_t(-6849794470754667710LL) };
			//static ::java::io::ObjectStreamFieldArray* serialPersistentFields_;

		private:
			static void checkBounds(std::vector<int8_t> bytes, int32_t offset, int32_t length);

		public:
			int32_t length() override;
			bool isEmpty();
			char16_t charAt(int32_t index) override;
			int32_t codePointAt(int32_t index);
			int32_t codePointBefore(int32_t index);
			int32_t codePointCount(int32_t beginIndex, int32_t endIndex);
			int32_t offsetByCodePoints(int32_t index, int32_t codePointOffset);

		public:
			void getChars(std::vector<char16_t>* dst, int32_t dstBegin);

		public:
			void getChars(int32_t srcBegin, int32_t srcEnd, std::vector<char16_t>* dst, int32_t dstBegin);
			void getBytes(int32_t srcBegin, int32_t srcEnd, std::vector<int8_t>* dst, int32_t dstBegin);
			std::vector<int8_t> getBytes(String* charsetName);
			std::vector<int8_t> getBytes(moduru::nio::charset::Charset* charset);
			std::vector<int8_t> getBytes();
			bool equals(Object* anObject) override;
			bool contentEquals(StringBuffer* sb);
			bool contentEquals(CharSequence* cs);
			bool equalsIgnoreCase(String anotherString);
			int32_t compareTo(String anotherString);

		private:
			static moduru::util::Comparator* CASE_INSENSITIVE_ORDER_;

		public:
			int32_t compareToIgnoreCase(String str);
			bool regionMatches(int32_t toffset, String other, int32_t ooffset, int32_t len);
			bool regionMatches(bool ignoreCase, int32_t toffset, String other, int32_t ooffset, int32_t len);
			bool startsWith(String prefix, int32_t toffset);
			bool startsWith(String prefix);
			bool endsWith(String suffix);
			int32_t hashCode() override;
			int32_t indexOf(int32_t ch);
			int32_t indexOf(int32_t ch, int32_t fromIndex);
			int32_t lastIndexOf(int32_t ch);
			int32_t lastIndexOf(int32_t ch, int32_t fromIndex);
			int32_t indexOf(String str);
			int32_t indexOf(String str, int32_t fromIndex);

		public: /* package */
			static int32_t indexOf(std::vector<char16_t>* source, int32_t sourceOffset, int32_t sourceCount, std::vector<char16_t>* target, int32_t targetOffset, int32_t targetCount, int32_t fromIndex);

		public:
			int32_t lastIndexOf(String str);
			int32_t lastIndexOf(String str, int32_t fromIndex);

		public: /* package */
			static int32_t lastIndexOf(std::vector<char16_t>* source, int32_t sourceOffset, int32_t sourceCount, std::vector<char16_t>* target, int32_t targetOffset, int32_t targetCount, int32_t fromIndex);

		public:
			String* substring(int32_t beginIndex);
			String* substring(int32_t beginIndex, int32_t endIndex);
			CharSequence* subSequence(int32_t beginIndex, int32_t endIndex) override;
			String* concat(String* str);
			String* replace(char16_t oldChar, char16_t newChar);
			bool matches(String* regex);
			bool contains(CharSequence* s);
			String* replaceFirst(String* regex, String* replacement);
			String* replaceAll(String* regex, String* replacement);
			String* replace(CharSequence* target, CharSequence* replacement);
			std::vector<String*> split(String* regex, int32_t limit);
			std::vector<String*> split(String* regex);
			//String* toLowerCase(moduru::util::Locale* locale);
			String* toLowerCase();
			//String* toUpperCase(moduru::util::Locale* locale);
			String* toUpperCase();
			String* trim();
			String* toString() override;
			std::vector<char16_t>* toCharArray_();
			static String* format(String* format, std::vector<Object*> args);
			//static String* format(moduru::util::Locale* l, String* format, std::vector<Object*> args);
			static String* valueOf(Object* obj);
			static String* valueOf(std::vector<char16_t> data);
			static String* valueOf(std::vector<char16_t> data, int32_t offset, int32_t count);
			static String* copyValueOf(std::vector<char16_t> data, int32_t offset, int32_t count);
			static String* copyValueOf(std::vector<char16_t> data);
			static String* valueOf(bool b);
			static String* valueOf(char16_t c);
			static String* valueOf(int32_t i);
			static String* valueOf(int64_t l);
			static String* valueOf(float f);
			static String* valueOf(double d);
			//String* intern();

			std::string to_cppstring();

			String();
			String(String* original);
			String(std::vector<char16_t> value);
			String(std::vector<char16_t> value, int32_t offset, int32_t count);
			String(std::vector<int32_t> codePoints, int32_t offset, int32_t count);
			String(std::vector<int8_t> ascii, int32_t hibyte, int32_t offset, int32_t count);
			String(std::vector<int8_t> ascii, int32_t hibyte);
			String(std::vector<int8_t> bytes, int32_t offset, int32_t length, String* charsetName);
			String(std::vector<int8_t> bytes, int32_t offset, int32_t length, moduru::nio::charset::Charset* charset);
			String(std::vector<int8_t> bytes, String* charsetName);
			String(std::vector<int8_t> bytes, moduru::nio::charset::Charset* charset);
			String(std::vector<int8_t> bytes, int32_t offset, int32_t length);
			String(std::vector<int8_t> bytes);
			String(StringBuffer* buffer);
			String(StringBuilder* builder);

			String(std::string str);
			~String();

		public:
			String(int offset, int count, std::vector<char16_t> value);


		public:
			//static ::Class *class_();
			//static void 
			virtual int32_t compareTo(Object* o) override;

		private:
			//static moduru::io::ObjectStreamFieldArray*& serialPersistentFields();

		public:
			static moduru::util::Comparator*& CASE_INSENSITIVE_ORDER();

		private:
			//friend class String_CaseInsensitiveComparator;
		};

	}
}
