#pragma once

#include <gui/geom/Curve.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class Order3 final
				: public Curve
			{

			public:
				typedef Curve super;

			private:
				double x0 {  };
				double y0 {  };
				double cx0 {  };
				double cy0 {  };
				double cx1 {  };
				double cy1 {  };
				double x1 {  };
				double y1 {  };
				double xmin {  };
				double xmax {  };
				double xcoeff0 {  };
				double xcoeff1 {  };
				double xcoeff2 {  };
				double xcoeff3 {  };
				double ycoeff0 {  };
				double ycoeff1 {  };
				double ycoeff2 {  };
				double ycoeff3 {  };

			public:
				static void insert(std::vector<Curve*>* curves, std::vector<double>* tmp, double x0, double y0, double cx0, double cy0, double cx1, double cy1, double x1, double y1, int direction);
				static void addInstance(std::vector<Curve*>* curves, double x0, double y0, double cx0, double cy0, double cx1, double cy1, double x1, double y1, int direction);
				static int getHorizontalParams(double c0, double cp0, double cp1, double c1, std::vector<double>* ret);
				static void split(std::vector<double>* coords, int pos, double t);

			public:
				int getOrder() override;
				double getXTop() override;
				double getYTop() override;
				double getXBot() override;
				double getYBot() override;
				double getXMin() override;
				double getXMax() override;
				double getX0() override;
				double getY0() override;
				double getCX0();
				double getCY0();
				double getCX1();
				double getCY1();
				double getX1() override;
				double getY1() override;

			private:
				double TforY1 {  };
				double YforT1 {  };
				double TforY2 {  };
				double YforT2 {  };
				double TforY3 {  };
				double YforT3 {  };

			public:
				double TforY(double y) override;
				double refine(double a, double b, double c, double target, double t);
				double XforY(double y) override;
				double XforT(double t) override;
				double YforT(double t) override;
				double dXforT(double t, int deriv) override;
				double dYforT(double t, int deriv) override;
				double nextVertical(double t0, double t1) override;
				void enlarge(Rectangle2D* r) override;
				Curve* getSubCurve(double ystart, double yend, int dir) override;
				Curve* getReversedCurve() override;
				int getSegment(std::vector<double>* coords) override;

				Order3(double x0, double y0, double cx0, double cy0, double cx1, double cy1, double x1, double y1, int direction);
			};
		}
	}
}
