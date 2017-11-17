#pragma once
#include <gui/geom/QuadCurve2D.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class QuadCurve2D_Double
				: public QuadCurve2D
			{

			public:
				typedef QuadCurve2D super;
				double x1{};
				double y1{};
				double ctrlx{};
				double ctrly{};
				double x2{};
				double y2{};

			public:
				double getX1() override;
				double getY1() override;
				Point2D* getP1() override;
				double getCtrlX() override;
				double getCtrlY() override;
				Point2D* getCtrlPt() override;
				double getX2() override;
				double getY2() override;
				Point2D* getP2() override;
				void setCurve(double x1, double y1, double ctrlx, double ctrly, double x2, double y2) override;
				Rectangle2D* getBounds2D() override;

			public:
				QuadCurve2D_Double();
				QuadCurve2D_Double(double x1, double y1, double ctrlx, double ctrly, double x2, double y2);

			public:
				//virtual void setCurve(vector<double>* coords, int32_t offset);
				//virtual void setCurve(Point2D* p1, Point2D* cp, Point2D* p2);
				//virtual void setCurve(vector<Point2D*>* pts, int32_t offset);
				//virtual void setCurve(QuadCurve2D* c);

			private:
				//friend class QuadCurve2D;
				//friend class QuadCurve2D_Float;
			};

		}
	}
}
