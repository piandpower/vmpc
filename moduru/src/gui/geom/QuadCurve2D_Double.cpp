#include <gui/geom/QuadCurve2D_Double.hpp>

#include <gui/geom/Point2D_Double.hpp>
#include <gui/geom/Rectangle2D_Double.hpp>

#include <math/Math.hpp>

using namespace moduru::gui::geom;

QuadCurve2D_Double::QuadCurve2D_Double() 
{
}

QuadCurve2D_Double::QuadCurve2D_Double(double x1, double y1, double ctrlx, double ctrly, double x2, double y2) 
{
    setCurve(x1, y1, ctrlx, ctrly, x2, y2);
}

double QuadCurve2D_Double::getX1()
{
    return x1;
}

double QuadCurve2D_Double::getY1()
{
    return y1;
}

Point2D* QuadCurve2D_Double::getP1()
{
    return new Point2D_Double(x1, y1);
}

double QuadCurve2D_Double::getCtrlX()
{
    return ctrlx;
}

double QuadCurve2D_Double::getCtrlY()
{
    return ctrly;
}

Point2D* QuadCurve2D_Double::getCtrlPt()
{
    return new Point2D_Double(ctrlx, ctrly);
}

double QuadCurve2D_Double::getX2()
{
    return x2;
}

double QuadCurve2D_Double::getY2()
{
    return y2;
}

Point2D* QuadCurve2D_Double::getP2()
{
    return new Point2D_Double(x2, y2);
}

void QuadCurve2D_Double::setCurve(double x1, double y1, double ctrlx, double ctrly, double x2, double y2)
{
    this->x1 = x1;
    this->y1 = y1;
    this->ctrlx = ctrlx;
    this->ctrly = ctrly;
    this->x2 = x2;
    this->y2 = y2;
}

Rectangle2D* QuadCurve2D_Double::getBounds2D()
{
    auto left = moduru::math::Math::min(moduru::math::Math::min(x1, x2), ctrlx);
    auto top = moduru::math::Math::min(moduru::math::Math::min(y1, y2), ctrly);
    auto right = moduru::math::Math::max(moduru::math::Math::max(x1, x2), ctrlx);
    auto bottom = moduru::math::Math::max(moduru::math::Math::max(y1, y2), ctrly);
    return new Rectangle2D_Double(left, top, right - left, bottom - top);
}
