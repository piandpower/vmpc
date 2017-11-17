#pragma once

#include <gui/geom/CubicCurve2D.hpp>

namespace moduru {
	namespace gui {
		namespace geom {

			class Rectangle2D;
			class Point2D;

			class CubicCurve2D_Float
				: public CubicCurve2D
			{

			public:
				typedef CubicCurve2D super;
				float x1{};
				float y1{};
				float ctrlx1{};
				float ctrly1{};
				float ctrlx2{};
				float ctrly2{};
				float x2{};
				float y2{};

			public:
				double getX1() override;
				double getY1() override;
				Point2D* getP1() override;
				double getCtrlX1() override;
				double getCtrlY1() override;
				Point2D* getCtrlP1() override;
				double getCtrlX2() override;
				double getCtrlY2() override;
				Point2D* getCtrlP2() override;
				double getX2() override;
				double getY2() override;
				Point2D* getP2() override;
				void setCurve(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2) override;
				virtual void setCurve(float x1, float y1, float ctrlx1, float ctrly1, float ctrlx2, float ctrly2, float x2, float y2);
				Rectangle2D* getBounds2D() override;

			public:
				CubicCurve2D_Float();
				CubicCurve2D_Float(float x1, float y1, float ctrlx1, float ctrly1, float ctrlx2, float ctrly2, float x2, float y2);

			private:
				//friend class CubicCurve2D;
				//friend class CubicCurve2D_Double;
			};

		}
	}
}
