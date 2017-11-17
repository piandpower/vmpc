#pragma once
#include <lang/Object.hpp>

#include <memory>

namespace moduru {

	namespace lang {
		class String;
	}

	namespace gui {
		namespace imagemap {

			class ShapeList;
			class HTMLTag;

			class HTMLParser
				: public virtual moduru::lang::Object
			{

			public:
				typedef moduru::lang::Object super;

			public:
				moduru::lang::String* html{ nullptr };
				int parseIndex{ 0 };

			public:
				moduru::lang::String getImageSrc();
				moduru::lang::String getMapName();
				void createShapeList(std::weak_ptr<ShapeList> sl);
				HTMLTag* nextTag();

				HTMLParser(moduru::lang::String* html);
				~HTMLParser();
			};

		}
	}
}
