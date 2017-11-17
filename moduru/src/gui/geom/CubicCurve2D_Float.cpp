#include <gui/geom/CubicCurve2D_Float.hpp>

#include <gui/geom/Point2D_Float.hpp>
#include <gui/geom/Rectangle2D_Float.hpp>

#include <math/Math.hpp>

using namespace moduru::gui::geom;

CubicCurve2D_Float::CubicCurve2D_Float() 
{
}

CubicCurve2D_Float::CubicCurve2D_Float(float x1, float y1, float ctrlx1, float ctrly1, float ctrlx2, float ctrly2, float x2, float y2) 
{
    setCurve(x1, y1, ctrlx1, ctrly1, ctrlx2, ctrly2, x2, y2);
}

double CubicCurve2D_Float::getX1()
{
    return static_cast< double >(x1);
}

double CubicCurve2D_Float::getY1()
{
    return static_cast< double >(y1);
}

Point2D* CubicCurve2D_Float::getP1()
{
    return new Point2D_Float(x1, y1);
}

double CubicCurve2D_Float::getCtrlX1()
{
    return static_cast< double >(ctrlx1);
}

double CubicCurve2D_Float::getCtrlY1()
{
    return static_cast< double >(ctrly1);
}

Point2D* CubicCurve2D_Float::getCtrlP1()
{
    return new Point2D_Float(ctrlx1, ctrly1);
}

double CubicCurve2D_Float::getCtrlX2()
{
    return static_cast< double >(ctrlx2);
}

double CubicCurve2D_Float::getCtrlY2()
{
    return static_cast< double >(ctrly2);
}

Point2D* CubicCurve2D_Float::getCtrlP2()
{
    return new Point2D_Float(ctrlx2, ctrly2);
}

double CubicCurve2D_Float::getX2()
{
    return static_cast< double >(x2);
}

double CubicCurve2D_Float::getY2()
{
    return static_cast< double >(y2);
}

Point2D* CubicCurve2D_Float::getP2()
{
    return new Point2D_Float(x2, y2);
}

void CubicCurve2D_Float::setCurve(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2)
{
    this->x1 = static_cast< float >(x1);
    this->y1 = static_cast< float >(y1);
    this->ctrlx1 = static_cast< float >(ctrlx1);
    this->ctrly1 = static_cast< float >(ctrly1);
    this->ctrlx2 = static_cast< float >(ctrlx2);
    this->ctrly2 = static_cast< float >(ctrly2);
    this->x2 = static_cast< float >(x2);
    this->y2 = static_cast< float >(y2);
}

void CubicCurve2D_Float::setCurve(float x1, float y1, float ctrlx1, float ctrly1, float ctrlx2, float ctrly2, float x2, float y2)
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

Rectangle2D* CubicCurve2D_Float::getBounds2D()
{
    auto left = moduru::math::Math::min(moduru::math::Math::min(x1, x2), moduru::math::Math::min(ctrlx1, ctrlx2));
    auto top = moduru::math::Math::min(moduru::math::Math::min(y1, y2), moduru::math::Math::min(ctrly1, ctrly2));
    auto right = moduru::math::Math::max(moduru::math::Math::max(x1, x2), moduru::math::Math::max(ctrlx1, ctrlx2));
    auto bottom = moduru::math::Math::max(moduru::math::Math::max(y1, y2), moduru::math::Math::max(ctrly1, ctrly2));
    return new Rectangle2D_Float(left, top, right - left, bottom - top);
}
