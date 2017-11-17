#pragma once
#include <lang/Object.hpp>

namespace moduru {
	namespace util {

		class Enumeration
			: public virtual moduru::lang::Object
		{
			virtual bool hasMoreElements() = 0;
			virtual moduru::lang::Object* nextElement() = 0;

		};

	}
}
