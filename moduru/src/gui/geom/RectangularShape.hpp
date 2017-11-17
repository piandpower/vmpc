#pragma once
#include <gui/Shape.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Dimension2D;

			class RectangularShape
				: public virtual moduru::gui::Shape
			{

			public:
				bool contains(Point2D* p) override;
				bool contains(Rectangle2D* r) override;
				moduru::gui::Rectangle* getBounds() override;
				virtual double getCenterX();
				virtual double getCenterY();
				virtual Rectangle2D* getFrame();
				virtual double getHeight() = 0;
				virtual double getMaxX();
				virtual double getMaxY();
				virtual double getMinX();
				virtual double getMinY();
				PathIterator* getPathIterator(AffineTransform* at, double flatness) override;
				virtual double getWidth() = 0;
				virtual double getX() = 0;
				virtual double getY() = 0;
				bool intersects(Rectangle2D* r) override;
				virtual bool isEmpty() = 0;
				virtual void setFrame(Rectangle2D* r);
				virtual void setFrame(Point2D* loc, Dimension2D* size);
				virtual void setFrame(double x, double y, double w, double h) = 0;
				virtual void setFrameFromCenter(Point2D* center, Point2D* corner);
				virtual void setFrameFromCenter(double centerX, double centerY, double cornerX, double cornerY);
				virtual void setFrameFromDiagonal(Point2D* p1, Point2D* p2);
				virtual void setFrameFromDiagonal(double x1, double y1, double x2, double y2);

				
			public: 
				RectangularShape();

			public:
				virtual bool contains(double x, double y) = 0;
				virtual bool contains(double x, double y, double w, double h) = 0;
				virtual PathIterator* getPathIterator(AffineTransform* at) = 0;
				virtual bool intersects(double x, double y, double w, double h) = 0;
			};
		}
	}
}
