#pragma once

#include <cstdint>

namespace moduru {
	namespace lang {

		class Object;

		class Comparable {

		public:
			virtual int32_t compareTo(Object* o) = 0;

		};

	}
}
