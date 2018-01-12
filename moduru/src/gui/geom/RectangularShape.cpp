#include <gui/geom/RectangularShape.hpp>

#include <gui/Rectangle.hpp>
#include <gui/geom/Dimension2D.hpp>
#include <gui/geom/FlatteningPathIterator.hpp>
#include <gui/geom/Point2D.hpp>
#include <gui/geom/Rectangle2D_Double.hpp>
#include <gui/geom/Rectangle2D.hpp>

#include <cmath>

#ifdef __linux__
#include <cstdint>
#endif

using namespace moduru::gui::geom;

RectangularShape::RectangularShape()
{
}

double RectangularShape::getMinX()
{
    return getX();
}

double RectangularShape::getMinY()
{
    return getY();
}

double RectangularShape::getMaxX()
{
    return getX() + getWidth();
}

double RectangularShape::getMaxY()
{
    return getY() + getHeight();
}

double RectangularShape::getCenterX()
{
    return getX() + getWidth() / 2.0;
}

double RectangularShape::getCenterY()
{
    return getY() + getHeight() / 2.0;
}

Rectangle2D* RectangularShape::getFrame()
{
    return new Rectangle2D_Double(getX(), getY(), getWidth(), getHeight());
}

void RectangularShape::setFrame(Point2D* loc, Dimension2D* size)
{
    setFrame(loc->getX(), loc->getY(), size->getWidth(), size->getHeight());
}

void RectangularShape::setFrame(Rectangle2D* r)
{
    setFrame(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

void RectangularShape::setFrameFromDiagonal(double x1, double y1, double x2, double y2)
{
    if(x2 < x1) {
        auto t = x1;
        x1 = x2;
        x2 = t;
    }
    if(y2 < y1) {
        auto t = y1;
        y1 = y2;
        y2 = t;
    }
    setFrame(x1, y1, x2 - x1, y2 - y1);
}

void RectangularShape::setFrameFromDiagonal(Point2D* p1, Point2D* p2)
{
    setFrameFromDiagonal(p1->getX(), p1->getY(), p2->getX(), p2->getY());
}

void RectangularShape::setFrameFromCenter(double centerX, double centerY, double cornerX, double cornerY)
{
    auto halfW = fabs(cornerX - centerX);
    auto halfH = fabs(cornerY - centerY);
    setFrame(centerX - halfW, centerY - halfH, halfW * 2.0, halfH * 2.0);
}

void RectangularShape::setFrameFromCenter(Point2D* center, Point2D* corner)
{
    setFrameFromCenter(center->getX(), center->getY(), corner->getX(), corner->getY());
}

bool RectangularShape::contains(Point2D* p)
{
    return contains(p->getX(), p->getY());
}

bool RectangularShape::intersects(Rectangle2D* r)
{
    return intersects(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

bool RectangularShape::contains(Rectangle2D* r)
{
    return contains(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

moduru::gui::Rectangle* RectangularShape::getBounds()
{
    auto width = getWidth();
    auto height = getHeight();
    if(width < 0 || height < 0) {
        return new moduru::gui::Rectangle();
    }
    auto x = getX();
    auto y = getY();
    auto x1 = floor(x);
    auto y1 = floor(y);
    auto x2 = ceil(x + width);
    auto y2 = ceil(y + height);
    return new moduru::gui::Rectangle(static_cast< int32_t >(x1), static_cast< int32_t >(y1), static_cast< int32_t >((x2 - x1)), static_cast< int32_t >((y2 - y1)));
}

moduru::gui::geom::PathIterator* RectangularShape::getPathIterator(AffineTransform* at, double flatness)
{
    return new FlatteningPathIterator(getPathIterator(at), flatness);
}
