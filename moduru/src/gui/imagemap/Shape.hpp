#pragma once
#include <lang/Object.hpp>

#include <memory>
#include <cstdint>

namespace moduru {
	namespace gui {

		class Rectangle;
		class Point;
		class Polygon;

		namespace imagemap {

			class Shape
				: public virtual moduru::lang::Object
			{

			public:
				typedef moduru::lang::Object super;

			private:
				static const int TYPE_RECT_{ 1 };
				static const int TYPE_CIRCLE_{ 2 };
				static const int TYPE_POLY_ { 3 };
				static const int POINT_RECT_UPPERLEFT_ { -1 };
				static const int POINT_RECT_UPPERRIGHT_ { -2 };
				static const int POINT_RECT_LOWERLEFT_ { -3 };
				static const int POINT_RECT_LOWERRIGHT_ { -4 };
				static const int POINT_RECT_INSIDE_ { -5 };
				static const int POINT_CIRCLE_RADIUS_ { -6 };
				static const int POINT_CIRCLE_INSIDE_ { -7 };
				static const int POINT_POLY_INSIDE_ { -8 };

				int type{ 0 };
				std::shared_ptr<moduru::gui::Rectangle> rect{};
				std::shared_ptr<moduru::gui::Point> circle_center{};
				int circle_r{ 0 };
				std::shared_ptr<moduru::gui::Polygon> poly{};
				std::shared_ptr<moduru::gui::Point> poly_center{};
				std::shared_ptr<moduru::lang::String> href;
				std::shared_ptr<moduru::lang::String> alt{};
				std::shared_ptr<moduru::lang::String> onMouseOver{};
				std::shared_ptr<moduru::lang::String> onClick{};
				std::shared_ptr<moduru::lang::String> onMouseOut{};
				std::shared_ptr<moduru::gui::Point> mouseDragPosition{};

			public:
				virtual void set_href(moduru::lang::String* href);
				virtual void set_alt(moduru::lang::String* alt);
				virtual void set_onMouseOver(moduru::lang::String* s);
				virtual void set_onMouseOut(moduru::lang::String* s);
				virtual void set_onClick(moduru::lang::String* s);
				virtual std::weak_ptr<moduru::lang::String> get_href();
				virtual std::weak_ptr<moduru::lang::String> get_alt();
				virtual moduru::lang::String get_typeString();
				//virtual void draw(moduru::gui::Graphics2D* g2, int zoomFactor);
				virtual void set_rect(int x1, int y1, int x2, int y2);
				virtual void set_rect_ul(int mx, int my);
				virtual void set_rect_lr(int mx, int my);
				virtual void set_rect_ur(int mx, int my);
				virtual void set_rect_ll(int mx, int my);
				virtual void set_circle(moduru::gui::Point* m, int r);
				virtual void set_polypoint(int p, int x, int y);
				virtual int add_polypoint(int x, int y);
				virtual void remove_polypoint(int p);
				virtual void remove_lastPolyPoint();
				virtual int get_type();
				virtual std::weak_ptr<moduru::gui::Rectangle> getRect();
				virtual int isKeyPoint(int mx, int my);
				virtual void moveKeyPoint(int p, int mx, int my);
				virtual moduru::lang::String* get_html();
				virtual bool inside(int x, int y);
				virtual void convert();
				virtual void convert(int n);
				virtual moduru::gui::Point* minDistance(int x, int y, Shape* activeShape, int movingPoint);
				virtual int minXDistance(int x);
				virtual int minYDistance(int y);
				virtual void insertPoint(int n, int x, int y);
				virtual bool tryAddPoint(int x, int y);
				virtual bool removePointAt(int x, int y);
				virtual moduru::lang::String* getCoordString(int mouseX, int mouseY);


				Shape(int x, int y, int x2, int y2);
				Shape(std::shared_ptr<moduru::gui::Point> m, int r);
				Shape(std::shared_ptr<moduru::gui::Point> first);
				Shape(std::shared_ptr<moduru::gui::Polygon> p);
				~Shape();
			};

		}
	}
}
