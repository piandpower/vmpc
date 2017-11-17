#pragma once
#include <lang/Object.hpp>

namespace moduru {
	namespace lang {

		class String;

		class Subset
			: public virtual Object
		{

		public:
			typedef Object super;

		private:
			String* name{};

		public:
			bool equals(Object* obj) override;
			int32_t hashCode() override;
			String* toString() override;

		public:
			Subset(String* name);

		private:
			//friend class Character;
			//friend class UnicodeBlock;
			//friend class CharacterCache;
		};

	}
}
