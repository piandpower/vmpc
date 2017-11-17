#pragma once

#include <gui/geom/QuadCurve2D.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class QuadCurve2D_Float
				: public QuadCurve2D
			{

			public:
				typedef QuadCurve2D super;
				float x1{};
				float y1{};
				float ctrlx{};
				float ctrly{};
				float x2{};
				float y2{};

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
				virtual void setCurve(float x1, float y1, float ctrlx, float ctrly, float x2, float y2);
				Rectangle2D* getBounds2D() override;

			public:
				QuadCurve2D_Float();
				QuadCurve2D_Float(float x1, float y1, float ctrlx, float ctrly, float x2, float y2);

			private:
				//friend class QuadCurve2D;
				//friend class QuadCurve2D_Double;
			};

		}
	}
}
