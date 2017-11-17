#include <gui/geom/QuadCurve2D_Float.hpp>

#include <gui/geom/Point2D_Float.hpp>
#include <gui/geom/Rectangle2D.hpp>
#include <gui/geom/Rectangle2D_Float.hpp>

#include <math/Math.hpp>

using namespace moduru::gui::geom;

QuadCurve2D_Float::QuadCurve2D_Float() 
{
}

QuadCurve2D_Float::QuadCurve2D_Float(float x1, float y1, float ctrlx, float ctrly, float x2, float y2) 
{
    setCurve(x1, y1, ctrlx, ctrly, x2, y2);
}

double QuadCurve2D_Float::getX1()
{
    return static_cast< double >(x1);
}

double QuadCurve2D_Float::getY1()
{
    return static_cast< double >(y1);
}

Point2D* QuadCurve2D_Float::getP1()
{
    return new Point2D_Float(x1, y1);
}

double QuadCurve2D_Float::getCtrlX()
{
    return static_cast< double >(ctrlx);
}

double QuadCurve2D_Float::getCtrlY()
{
    return static_cast< double >(ctrly);
}

Point2D* QuadCurve2D_Float::getCtrlPt()
{
    return new Point2D_Float(ctrlx, ctrly);
}

double QuadCurve2D_Float::getX2()
{
    return static_cast< double >(x2);
}

double QuadCurve2D_Float::getY2()
{
    return static_cast< double >(y2);
}

Point2D* QuadCurve2D_Float::getP2()
{
    return new Point2D_Float(x2, y2);
}

void QuadCurve2D_Float::setCurve(double x1, double y1, double ctrlx, double ctrly, double x2, double y2)
{
    this->x1 = static_cast< float >(x1);
    this->y1 = static_cast< float >(y1);
    this->ctrlx = static_cast< float >(ctrlx);
    this->ctrly = static_cast< float >(ctrly);
    this->x2 = static_cast< float >(x2);
    this->y2 = static_cast< float >(y2);
}

void QuadCurve2D_Float::setCurve(float x1, float y1, float ctrlx, float ctrly, float x2, float y2)
{
    this->x1 = x1;
    this->y1 = y1;
    this->ctrlx = ctrlx;
    this->ctrly = ctrly;
    this->x2 = x2;
    this->y2 = y2;
}

Rectangle2D* QuadCurve2D_Float::getBounds2D()
{
	auto left = moduru::math::Math::min(moduru::math::Math::min(x1, x2), ctrlx);
	auto top = moduru::math::Math::min(moduru::math::Math::min(y1, y2), ctrly);
	auto right = moduru::math::Math::max(moduru::math::Math::max(x1, x2), ctrlx);
	auto bottom = moduru::math::Math::max(moduru::math::Math::max(y1, y2), ctrly);
	return new Rectangle2D_Float(left, top, right - left, bottom - top);
}
