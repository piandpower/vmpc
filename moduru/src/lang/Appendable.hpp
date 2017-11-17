#pragma once
#include <lang/Object.hpp>

namespace moduru {
	namespace lang {

		class CharSequence;

		class Appendable
			: public virtual Object
		{
		protected:
			virtual Appendable* append(CharSequence* csq) = 0;
			virtual Appendable* append(CharSequence* csq, int32_t start, int32_t end) = 0;
			virtual Appendable* append(char16_t c) = 0;

		};

	}
}
