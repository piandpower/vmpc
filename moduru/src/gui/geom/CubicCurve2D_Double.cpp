#include <gui/geom/CubicCurve2D_Double.hpp>

#include <gui/geom/Point2D_Double.hpp>
#include <gui/geom/Rectangle2D_Double.hpp>

#include <math/Math.hpp>

using namespace moduru::gui::geom;

CubicCurve2D_Double::CubicCurve2D_Double() 
{
}

CubicCurve2D_Double::CubicCurve2D_Double(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2) 
{
	setCurve(x1, y1, ctrlx1, ctrly1, ctrlx2, ctrly2, x2, y2);
}

double CubicCurve2D_Double::getX1()
{
    return x1;
}

double CubicCurve2D_Double::getY1()
{
    return y1;
}

Point2D* CubicCurve2D_Double::getP1()
{
    return new Point2D_Double(x1, y1);
}

double CubicCurve2D_Double::getCtrlX1()
{
    return ctrlx1;
}

double CubicCurve2D_Double::getCtrlY1()
{
    return ctrly1;
}

Point2D* CubicCurve2D_Double::getCtrlP1()
{
    return new Point2D_Double(ctrlx1, ctrly1);
}

double CubicCurve2D_Double::getCtrlX2()
{
    return ctrlx2;
}

double CubicCurve2D_Double::getCtrlY2()
{
    return ctrly2;
}

Point2D* CubicCurve2D_Double::getCtrlP2()
{
    return new Point2D_Double(ctrlx2, ctrly2);
}

double CubicCurve2D_Double::getX2()
{
    return x2;
}

double CubicCurve2D_Double::getY2()
{
    return y2;
}

Point2D* CubicCurve2D_Double::getP2()
{
    return new Point2D_Double(x2, y2);
}

void CubicCurve2D_Double::setCurve(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2)
{
    this->x1 = x1;
    this->y1 = y1;
    this->ctrlx1 = ctrlx1;
    this->ctrly1 = ctrly1;
    this->ctrlx2 = ctrlx2;
    this->ctrly2 = ctrly2;
    this->x2 = x2;
    this->y2 = y2;
}

Rectangle2D* CubicCurve2D_Double::getBounds2D()
{
    auto left = moduru::math::Math::min(moduru::math::Math::min(x1, x2), moduru::math::Math::min(ctrlx1, ctrlx2));
    auto top = moduru::math::Math::min(moduru::math::Math::min(y1, y2), moduru::math::Math::min(ctrly1, ctrly2));
    auto right = moduru::math::Math::max(moduru::math::Math::max(x1, x2), moduru::math::Math::max(ctrlx1, ctrlx2));
    auto bottom = moduru::math::Math::max(moduru::math::Math::max(y1, y2), moduru::math::Math::max(ctrly1, ctrly2));
    return new Rectangle2D_Double(left, top, right - left, bottom - top);
}
