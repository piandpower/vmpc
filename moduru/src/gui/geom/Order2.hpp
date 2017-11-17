#pragma once
#include <gui/geom/Curve.hpp>

#include <vector>

namespace moduru {
	namespace gui {
		namespace geom {

			class Order2 final
				: public Curve
			{

			public:
				typedef Curve super;

			private:
				double x0{};
				double y0{};
				double cx0{};
				double cy0{};
				double x1{};
				double y1{};
				double xmin{};
				double xmax{};
				double xcoeff0{};
				double xcoeff1{};
				double xcoeff2{};
				double ycoeff0{};
				double ycoeff1{};
				double ycoeff2{};

			public:
				static void insert(std::vector<Curve*>* curves, std::vector<double>* tmp, double x0, double y0, double cx0, double cy0, double x1, double y1, int direction);
				static void addInstance(std::vector<Curve*>* curves, double x0, double y0, double cx0, double cy0, double x1, double y1, int direction);
				static int getHorizontalParams(double c0, double cp, double c1, std::vector<double>* ret);
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
				double getX1() override;
				double getY1() override;
				double XforY(double y) override;
				double TforY(double y) override;
				static double TforY(double y, double ycoeff0, double ycoeff1, double ycoeff2);
				double XforT(double t) override;
				double YforT(double t) override;
				double dXforT(double t, int deriv) override;
				double dYforT(double t, int deriv) override;
				double nextVertical(double t0, double t1) override;
				void enlarge(Rectangle2D* r) override;
				Curve* getSubCurve(double ystart, double yend, int dir) override;
				Curve* getReversedCurve() override;
				int getSegment(std::vector<double>* coords) override;
				//::String* controlPointString() override;

				Order2(double x0, double y0, double cx0, double cy0, double x1, double y1, int direction);
			};

		}
	}
}
