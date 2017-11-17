#pragma once

#include <gui/geom/Point2D.hpp>

namespace moduru {
	namespace gui {

		class Point
			: public moduru::gui::geom::Point2D
		{

		public:
			typedef gui::geom::Point2D super;

		public:
			int x{ 0 };
			int y{ 0 };

		public:
			//bool equals(moduru::lang::Object* obj) override;
			virtual Point* getLocation();
			double getX() override;
			double getY() override;
			virtual void move(int x, int y);
			virtual void setLocation(Point* p);
			virtual void setLocation(int x, int y);
			void setLocation(double x, double y) override;
			//::String* toString() override;
			virtual void translate(int dx, int dy);

			Point();
			Point(Point* p);
			Point(int x, int y);
		};

	}
}
