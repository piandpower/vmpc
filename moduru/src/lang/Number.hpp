#pragma once

#include <cstdint>

#include <string>

namespace moduru {
	namespace lang {

		class String;

		class Number
		{

		public:
			virtual int32_t intValue() = 0;
			virtual int64_t longValue() = 0;
			virtual float floatValue() = 0;
			virtual double doubleValue() = 0;
			virtual int8_t byteValue();
			virtual int16_t shortValue();
			virtual String* toString() = 0;
			virtual std::string to_cppstring() = 0;

		public:
			Number();
		};

	}
}
