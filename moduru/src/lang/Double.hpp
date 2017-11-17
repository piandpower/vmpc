#pragma once

#include <lang/Number.hpp>
//#include <lang/Comparable.hpp>

#include <string>
#include <cstdint>

namespace moduru {
	namespace lang {

		class String;

		class Double final
			: public Number
			//, public Comparable
		{

		public:
			typedef Number super;
			static constexpr double POSITIVE_INFINITY{ std::numeric_limits<double>::infinity() };
			static constexpr double NEGATIVE_INFINITY{ (-std::numeric_limits<double>::infinity()) };
			static constexpr double NaN{ std::numeric_limits<double>::quiet_NaN() };
			static constexpr double MAX_VALUE{ 1.7976931348623157E308 };
			static constexpr double MIN_NORMAL{ 2.2250738585072014E-308 };
			static constexpr double MIN_VALUE{ 4.9E-324 };
			static constexpr int MAX_EXPONENT{ 1023 };
			static constexpr int MIN_EXPONENT{ -1022 };
			static constexpr int SIZE{ 64 };

		private:
			//static Class* TYPE_;

		public:
			static std::string toString(double d);
			static std::string toHexString(double d);
			static Double* valueOf(std::string s);
			static Double* valueOf(double d);
			static double parseDouble(std::string s);
			static bool isNaN(double v);
			static bool isInfinite(double v);

		private:
			double value{};

		public:
			bool isNaN();
			bool isInfinite();
			std::string to_cppstring() override;
			String* toString() override;
			int8_t byteValue() override;
			int16_t shortValue() override;
			int intValue() override;
			int64_t longValue() override;
			float floatValue() override;
			double doubleValue() override;
			//int hashCode() override;
			//bool equals(Object* obj) override;
			static int64_t doubleToLongBits(double value);
			static uint64_t doubleToRawLongBits(double value);
			static double longBitsToDouble(uint64_t bits);
			int compareTo(Double* anotherDouble);
			static int compare(double d1, double d2);

		public:
			Double(double value);
			Double(std::string s);

		public:
			//virtual int compareTo(Object* o) override;
			//static Class*& TYPE();

		};

	}
}
