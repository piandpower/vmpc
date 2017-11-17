#pragma once
#include <lang/Object.hpp>

namespace moduru {

	namespace lang {
		class String;
	}

	namespace gui {
		namespace imagemap {

			class HTMLTag
				: public virtual moduru::lang::Object
			{

			public:
				typedef moduru::lang::Object super;

			public:
				moduru::lang::String* tag{};
				moduru::lang::String* tag_lc{};

			public:
				moduru::lang::String getArgumentValue(moduru::lang::String arg);
				moduru::lang::String getTagType();


				HTMLTag(moduru::lang::String* tag);
				~HTMLTag();

			};

		}
	}
}
