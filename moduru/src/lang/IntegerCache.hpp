#pragma once

#include <vector>

namespace moduru {
	namespace lang {

		class Integer;

		class IntegerCache
		{

		private:
			static std::vector<Integer*> cache_;

		public:
			static std::vector<Integer*> cache();

			IntegerCache();

		};

	}
}
