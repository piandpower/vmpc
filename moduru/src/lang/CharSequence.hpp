#pragma once
#include <lang/Object.hpp>

#include <cstdint>

namespace moduru {
	namespace lang {

		class CharSequence
			: public virtual Object
		{
		public:
			virtual int32_t length() = 0;
			virtual char16_t charAt(int32_t index) = 0;
			virtual CharSequence* subSequence(int32_t start, int32_t end) = 0;
			/*virtual String* toString(); (already declared) */

		};

	}
}
