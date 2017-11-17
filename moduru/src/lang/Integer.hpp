#pragma once

#include <lang/Number.hpp>
#include <lang/Comparable.hpp>
#include <lang/Object.hpp>

#include <cstdint>
#include <string>
#include <vector>

namespace moduru {
	namespace lang {

		class String;

		class Integer final
			: public virtual Object
			, public Number
			, public Comparable
		{

		public:
			typedef Number super;
			static constexpr int32_t MIN_VALUE{ int32_t(-0x7fffffff - 1) };
			static constexpr int32_t MAX_VALUE{ int32_t(2147483647) };

		private:
			//static Class* TYPE_;
			static std::vector<char16_t> digits_;

		public:
			static std::string  toString(int32_t i, int32_t radix);
			static std::string  toHexString(int32_t i);
			static std::string  toOctalString(int32_t i);
			static std::string  toBinaryString(int32_t i);

		private:
			static std::string  toUnsignedString(int32_t i, int32_t shift);
			static std::vector<char16_t> DigitTens_;
			static std::vector<char16_t> DigitOnes_;

		public:
			static std::string  toString(int32_t i);

		public:
			static void getChars(int32_t i, int32_t index, std::vector<char16_t>* buf);

		private:
			static std::vector<int32_t> sizeTable_;

		public:
			static int32_t stringSize(int32_t x);

		public:
			static int32_t parseInt(std::string  s, int32_t radix);
			static int32_t parseInt(std::string  s);
			static Integer* valueOf(std::string  s, int32_t radix);
			static Integer* valueOf(std::string  s);
			static Integer* valueOf(int32_t i);

		private:
			int32_t value{};

		public:
			int8_t byteValue() override;
			int16_t shortValue() override;
			int32_t intValue() override;
			int64_t longValue() override;
			float floatValue() override;
			double doubleValue() override;
			std::string  to_cppstring() override;
			String* toString() override;
			//int32_t hashCode() override;
			//bool equals(Object* obj) override;
			static Integer* getInteger(std::string  nm);
			static Integer* getInteger(std::string  nm, int32_t val);
			static Integer* getInteger(std::string  nm, Integer* val);
			static Integer* decode(std::string  nm);
			int32_t compareTo(Integer* anotherInteger);
			static constexpr int32_t SIZE{ int32_t(32) };
			static int32_t highestOneBit(int32_t i);
			static int32_t lowestOneBit(int32_t i);
			static int32_t numberOfLeadingZeros(int32_t i);
			static int32_t numberOfTrailingZeros(int32_t i);
			static int32_t bitCount(int32_t i);
			static int32_t rotateLeft(int32_t i, int32_t distance);
			static int32_t rotateRight(int32_t i, int32_t distance);
			static int32_t reverse(int32_t i);
			static int32_t signum(int32_t i);
			static int32_t reverseBytes(int32_t i);

		public:
			Integer(int32_t value);
			Integer(std::string  s);
			Integer(String* s);

		public:
			//static void 
			virtual int32_t compareTo(Object* o) override;
			//static Class*& TYPE();

		public:
			static std::vector<char16_t> digits();
			static std::vector<char16_t> DigitTens();
			static std::vector<char16_t> DigitOnes();
			static std::vector<int32_t> sizeTable();

		private:
			//friend class IntegerCache;
		};

	}
}
