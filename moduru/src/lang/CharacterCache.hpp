#pragma once
#include <lang/Object.hpp>

#include <vector>

namespace moduru {
	namespace lang {

		class Character;

		class CharacterCache
			: public virtual Object
		{

		public:
			typedef Object super;

		private:
			static std::vector<Character*>* cache_;

			CharacterCache();

		public: /* package */
			static std::vector<Character*>*& cache();

		private:
			//friend class Character;
			//friend class Subset;
			//friend class UnicodeBlock;
		};

	}
}
