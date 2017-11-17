#pragma once
#include <gui/geom/Rectangle2D.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Rectangle2D_Double
				: public Rectangle2D
			{

			public:
				typedef Rectangle2D super;
				double height{};

			public:
				double width{};
				double x{};
				double y{};

			public:
				Rectangle2D* createIntersection(Rectangle2D* r) override;
				Rectangle2D* createUnion(Rectangle2D* r) override;
				Rectangle2D* getBounds2D() override;
				double getHeight() override;
				double getWidth() override;
				double getX() override;
				double getY() override;
				bool isEmpty() override;
				int outcode(double x, double y) override;
				void setRect(Rectangle2D* r) override;
				void setRect(double x, double y, double w, double h) override;
				//::String* toString() override;

				virtual Shape* clone() override { return nullptr; };

				Rectangle2D_Double();
				Rectangle2D_Double(double x, double y, double w, double h);
			};

		}
	}
}
