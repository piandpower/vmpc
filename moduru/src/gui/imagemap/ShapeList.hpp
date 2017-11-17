#pragma once
#include <lang/Object.hpp>

#include <memory>
#include <vector>

namespace moduru {
	namespace gui {
		namespace imagemap {

			class Shape;

			class ShapeList
				: public virtual moduru::lang::Object
			{

			public:
				typedef moduru::lang::Object super;

			private:
				std::vector<std::shared_ptr<Shape>> vec{};
				std::weak_ptr<Shape> foundShape{};
				int foundKeyPoint{ 0 };

			public:
				moduru::lang::String* mapName{ nullptr };

			public:
				 void add_shape(std::shared_ptr<Shape> s);
				 std::weak_ptr<Shape> get_shape(int i);
				 void remove_shape(std::weak_ptr<Shape> s);
				 int size();
				 void clear();
				 bool isShapePointAt(int x, int y);
				 bool isPointInsideShape(int x, int y);
				 std::weak_ptr<Shape> getFoundShape();
				 int getFoundKeyPoint();
				 int getFoundShapeIndex();
				 void removeFoundShape();
				 void addPoint(int x, int y);
				 void removePointAt(int x, int y);
				 moduru::lang::String* get_html(moduru::lang::String* imgSrc, int w, int h);

				ShapeList();
				~ShapeList();

			};

		}
	}
}
