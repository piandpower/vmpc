#pragma once
#include <gui/geom/Curve.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Rectangle2D;

			class Order0 final
				: public Curve
			{

			public:
				typedef Curve super;

			private:
				double x {  };
				double y {  };

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
				int crossingsFor(double x, double y) override;
				bool accumulateCrossings(Crossings* c) override;
				void enlarge(Rectangle2D* r) override;
				Curve* getSubCurve(double ystart, double yend, int dir) override;
				Curve* getReversedCurve() override;
				int getSegment(std::vector<double>* coords) override;

				Order0(double x, double y);
			};

		}
	}
}
