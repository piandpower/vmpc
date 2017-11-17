#pragma once

#include <lang/Number.hpp>
//#include <lang/Comparable.hpp>

#include <string>
#include <cstdint>

namespace moduru {
	namespace lang {

		class String;

		class Float final
			: public Number
			//, public Comparable
		{

		public:
			typedef Number super;
			static const float POSITIVE_INFINITY;
			static const float NEGATIVE_INFINITY;
			static const float NaN;
			static float MAX_VALUE;
			static const float MIN_NORMAL;
			static const float MIN_VALUE;
			static const int32_t MAX_EXPONENT{ int32_t(127) };
			static const int32_t MIN_EXPONENT{ int32_t(-126) };
			static const int32_t SIZE{ int32_t(32) };
			static String* toString(float f);
			//static String* toHexString(float f);
			//static Float* valueOf(String* s);
			static Float* valueOf(float f);
			//static float parseFloat(String* s);
			static bool isNaN(float v);
			static bool isInfinite(float v);

		private:
			float value{};

		public:
			bool isNaN();
			bool isInfinite();
			std::string to_cppstring() override;
			String* toString() override;
			int8_t byteValue() override;
			int16_t shortValue() override;
			int32_t intValue() override;
			int64_t longValue() override;
			float floatValue() override;
			double doubleValue() override;
			//int32_t hashCode() override;
			//bool equals(Object* obj) override;
			static int32_t floatToIntBits(float value);
			static int32_t floatToRawIntBits(float value);
			static float intBitsToFloat(int32_t bits);
			int32_t compareTo(Float* anotherFloat);
			static int32_t compare(float f1, float f2);

		private:
			static constexpr int64_t serialVersionUID{ int64_t(-2671257302660747028LL) };

		public:
			Float(float value);
			Float(double value);
			Float(std::string s);

		public:
			//virtual int32_t compareTo(Object* o) override;

		};

	}
}