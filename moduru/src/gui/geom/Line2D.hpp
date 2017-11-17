#pragma once
#include <gui/Shape.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Line2D
				: public virtual moduru::gui::Shape {


			public:
				virtual double getX1() = 0;
				virtual double getY1() = 0;
				virtual Point2D* getP1() = 0;
				virtual double getX2() = 0;
				virtual double getY2() = 0;
				virtual Point2D* getP2() = 0;
				virtual void setLine(double x1, double y1, double x2, double y2) = 0;
				virtual void setLine(Point2D* p1, Point2D* p2);
				virtual void setLine(Line2D* l);
				static int relativeCCW(double x1, double y1, double x2, double y2, double px, double py);
				virtual int relativeCCW(double px, double py);
				virtual int relativeCCW(Point2D* p);
				static bool linesIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4);
				virtual bool intersectsLine(double x1, double y1, double x2, double y2);
				virtual bool intersectsLine(Line2D* l);
				static double ptSegDistSq(double x1, double y1, double x2, double y2, double px, double py);
				static double ptSegDist(double x1, double y1, double x2, double y2, double px, double py);
				virtual double ptSegDistSq(double px, double py);
				virtual double ptSegDistSq(Point2D* pt);
				virtual double ptSegDist(double px, double py);
				virtual double ptSegDist(Point2D* pt);
				static double ptLineDistSq(double x1, double y1, double x2, double y2, double px, double py);
				static double ptLineDist(double x1, double y1, double x2, double y2, double px, double py);
				virtual double ptLineDistSq(double px, double py);
				virtual double ptLineDistSq(Point2D* pt);
				virtual double ptLineDist(double px, double py);
				virtual double ptLineDist(Point2D* pt);
				bool contains(double x, double y) override;
				bool contains(Point2D* p) override;
				bool intersects(double x, double y, double w, double h) override;
				bool intersects(Rectangle2D* r) override;
				bool contains(double x, double y, double w, double h) override;
				bool contains(Rectangle2D* r) override;
				moduru::gui::Rectangle* getBounds() override;
				PathIterator* getPathIterator(AffineTransform* at) override;
				PathIterator* getPathIterator(AffineTransform* at, double flatness) override;
				
				Line2D();

			private:
				//friend class Line2D_Float;
				//friend class Line2D_Double;
			};

		}
	}
}
