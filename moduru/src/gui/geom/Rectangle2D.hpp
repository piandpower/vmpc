#pragma once

#include <gui/geom/RectangularShape.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Line2D;
			class Point2D;
			class Dimension2D;

			class Rectangle2D
				: public RectangularShape
			{

			public:
				typedef RectangularShape super;
				static const int OUT_BOTTOM{ 8 };
				static const int OUT_LEFT{ 1 };
				static const int OUT_RIGHT{ 4 };
				static const int OUT_TOP{ 2 };

			public:
				virtual void add(Point2D* pt);
				virtual void add(Rectangle2D* r);
				virtual void add(double newx, double newy);
				bool contains(double x, double y) override;
				bool contains(double x, double y, double w, double h) override;
				virtual Rectangle2D* createIntersection(Rectangle2D* r) = 0;
				virtual Rectangle2D* createUnion(Rectangle2D* r) = 0;
				//bool equals(moduru::lang::Object* obj) override;
				Rectangle2D* getBounds2D() override;
				PathIterator* getPathIterator(AffineTransform* at) override;
				PathIterator* getPathIterator(AffineTransform* at, double flatness) override;
				//int hashCode() override;
				static void intersect(Rectangle2D* src1, Rectangle2D* src2, Rectangle2D* dest);
				bool intersects(double x, double y, double w, double h) override;
				virtual bool intersectsLine(Line2D* l);
				virtual bool intersectsLine(double x1, double y1, double x2, double y2);
				virtual int outcode(Point2D* p);
				virtual int outcode(double x, double y) = 0;
				void setFrame(double x, double y, double w, double h) override;
				virtual void setRect(Rectangle2D* r);
				virtual void setRect(double x, double y, double w, double h) = 0;
				static void union_(Rectangle2D* src1, Rectangle2D* src2, Rectangle2D* dest);

			public:
				Rectangle2D();

			};

		}
	}
}
