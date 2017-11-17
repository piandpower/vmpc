#pragma once
#include <gui/geom/Curve.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class Order1 final
				: public Curve
			{

			public:
				typedef Curve super;

			private:
				double x0 {  };
				double y0 {  };
				double x1 {  };
				double y1 {  };
				double xmin {  };
				double xmax {  };

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
				double getX1() override;
				double getY1() override;
				double XforY(double y) override;
				double TforY(double y) override;
				double XforT(double t) override;
				double YforT(double t) override;
				double dXforT(double t, int deriv) override;
				double dYforT(double t, int deriv) override;
				double nextVertical(double t0, double t1) override;
				bool accumulateCrossings(Crossings* c) override;
				void enlarge(Rectangle2D* r) override;
				Curve* getSubCurve(double ystart, double yend, int dir) override;
				Curve* getReversedCurve() override;
				int compareTo(Curve* other, std::vector<double>* yrange) override;
				int getSegment(std::vector<double>* coords) override;

				Order1(double x0, double y0, double x1, double y1, int direction);
			};

		}
	}
}
