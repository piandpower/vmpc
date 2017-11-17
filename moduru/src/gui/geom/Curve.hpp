#pragma once

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class Rectangle2D;
			class PathIterator;
			class Crossings;

			class Curve
			{

			public:
				static const int INCREASING{ 1 };
				static const int DECREASING{ -1 };

			public:
				int direction{};

			public:
				static void insertMove(std::vector<Curve*>* curves, double x, double y);
				static void insertLine(std::vector<Curve*>* curves, double x0, double y0, double x1, double y1);
				static void insertQuad(std::vector<Curve*>* curves, double x0, double y0, std::vector<double>* coords);
				static void insertCubic(std::vector<Curve*>* curves, double x0, double y0, std::vector<double>* coords);
				static int pointCrossingsForPath(PathIterator* pi, double px, double py);
				static int pointCrossingsForLine(double px, double py, double x0, double y0, double x1, double y1);
				static int pointCrossingsForQuad(double px, double py, double x0, double y0, double xc, double yc, double x1, double y1, int level);
				static int pointCrossingsForCubic(double px, double py, double x0, double y0, double xc0, double yc0, double xc1, double yc1, double x1, double y1, int level);
				static constexpr int RECT_INTERSECTS{ -0x7fffffff - 1 };
				static int rectCrossingsForPath(PathIterator* pi, double rxmin, double rymin, double rxmax, double rymax);
				static int rectCrossingsForLine(int crossings, double rxmin, double rymin, double rxmax, double rymax, double x0, double y0, double x1, double y1);
				static int rectCrossingsForQuad(int crossings, double rxmin, double rymin, double rxmax, double rymax, double x0, double y0, double xc, double yc, double x1, double y1, int level);
				static int rectCrossingsForCubic(int crossings, double rxmin, double rymin, double rxmax, double rymax, double x0, double y0, double xc0, double yc0, double xc1, double yc1, double x1, double y1, int level);

			public:
				int getDirection();
				Curve* getWithDirection(int direction);
				static double round(double v);
				static int orderof(double x1, double x2);
				static int64_t signeddiffbits(double y1, double y2);
				static int64_t diffbits(double y1, double y2);
				static double prev(double v);
				static double next(double v);
				//::String* toString() override;
				//virtual ::String* controlPointString();
				virtual int getOrder() = 0;
				virtual double getXTop() = 0;
				virtual double getYTop() = 0;
				virtual double getXBot() = 0;
				virtual double getYBot() = 0;
				virtual double getXMin() = 0;
				virtual double getXMax() = 0;
				virtual double getX0() = 0;
				virtual double getY0() = 0;
				virtual double getX1() = 0;
				virtual double getY1() = 0;
				virtual double XforY(double y) = 0;
				virtual double TforY(double y) = 0;
				virtual double XforT(double t) = 0;
				virtual double YforT(double t) = 0;
				virtual double dXforT(double t, int deriv) = 0;
				virtual double dYforT(double t, int deriv) = 0;
				virtual double nextVertical(double t0, double t1) = 0;
				virtual int crossingsFor(double x, double y);
				virtual bool accumulateCrossings(Crossings* c);
				virtual void enlarge(Rectangle2D* r) = 0;
				virtual Curve* getSubCurve(double ystart, double yend);
				virtual Curve* getReversedCurve() = 0;
				virtual Curve* getSubCurve(double ystart, double yend, int dir) = 0;
				virtual int compareTo(Curve* that, std::vector<double>* yrange);
				static constexpr double TMIN{ 0.001 };
				virtual bool findIntersect(Curve* that, std::vector<double>* yrange, double ymin, int slevel, int tlevel, double s0, double xs0, double ys0, double s1, double xs1, double ys1, double t0, double xt0, double yt0, double t1, double xt1, double yt1);
				virtual double refineTforY(double t0, double yt0, double y0);
				virtual bool fairlyClose(double v1, double v2);
				virtual int getSegment(std::vector<double>* coords) = 0;

				Curve(int direction);
			};

		}
	}
}
