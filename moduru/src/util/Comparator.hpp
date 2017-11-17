#pragma once
#include <lang/Object.hpp>

namespace moduru {
	namespace util {

		class Comparator
			: public virtual moduru::lang::Object
		{
		public:
			virtual int32_t compare(moduru::lang::Object* o1, moduru::lang::Object* o2) = 0;
			/*virtual bool equals(moduru::lang::Object* obj); (already declared) */

		};

	}
}
