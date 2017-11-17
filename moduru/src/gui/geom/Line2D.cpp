#include <gui/geom/Line2D.hpp>

#include <gui/Rectangle.hpp>
#include <gui/geom/LineIterator.hpp>
#include <gui/geom/Point2D.hpp>
#include <gui/geom/Rectangle2D_Double.hpp>
#include <gui/geom/Rectangle2D.hpp>

#include <cmath>

using namespace moduru::gui::geom;

Line2D::Line2D() 
{
}

void Line2D::setLine(Point2D* p1, Point2D* p2)
{
    setLine(p1->getX(), p1->getY(), p2->getX(), p2->getY());
}

void Line2D::setLine(Line2D* l)
{
    setLine(l->getX1(), l->getY1(), l->getX2(), l->getY2());
}

int Line2D::relativeCCW(double x1, double y1, double x2, double y2, double px, double py)
{
    x2 -= x1;
    y2 -= y1;
    px -= x1;
    py -= y1;
    auto ccw = px * y2 - py * x2;
    if(ccw == 0.0) {
        ccw = px * x2 + py * y2;
        if(ccw > 0.0) {
            px -= x2;
            py -= y2;
            ccw = px * x2 + py * y2;
            if(ccw < 0.0) {
                ccw = 0.0;
            }
        }
    }
    return (ccw < 0.0) ? -1 : ((ccw > 0.0) ? 1 : 0);
}

int Line2D::relativeCCW(double px, double py)
{
    return relativeCCW(getX1(), getY1(), getX2(), getY2(), px, py);
}

int Line2D::relativeCCW(Point2D* p)
{
    return relativeCCW(getX1(), getY1(), getX2(), getY2(), p->getX(), p->getY());
}

bool Line2D::linesIntersect(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4)
{
    return ((relativeCCW(x1, y1, x2, y2, x3, y3) * relativeCCW(x1, y1, x2, y2, x4, y4) <= 0) && (relativeCCW(x3, y3, x4, y4, x1, y1) * relativeCCW(x3, y3, x4, y4, x2, y2) <= 0));
}

bool Line2D::intersectsLine(double x1, double y1, double x2, double y2)
{
    return linesIntersect(x1, y1, x2, y2, getX1(), getY1(), getX2(), getY2());
}

bool Line2D::intersectsLine(Line2D* l)
{
    return linesIntersect(l->getX1(), l->getY1(), l->getX2(), l->getY2(), getX1(), getY1(), getX2(), getY2());
}

double Line2D::ptSegDistSq(double x1, double y1, double x2, double y2, double px, double py)
{

    x2 -= x1;
    y2 -= y1;
    px -= x1;
    py -= y1;
    auto dotprod = px * x2 + py * y2;
    double projlenSq;
    if(dotprod <= 0.0) {
        projlenSq = 0.0;
    } else {
        px = x2 - px;
        py = y2 - py;
        dotprod = px * x2 + py * y2;
        if(dotprod <= 0.0) {
            projlenSq = 0.0;
        } else {
            projlenSq = dotprod * dotprod / (x2 * x2 + y2 * y2);
        }
    }
    auto lenSq = px * px + py * py - projlenSq;
    if(lenSq < 0) {
        lenSq = 0;
    }
    return lenSq;
}

double Line2D::ptSegDist(double x1, double y1, double x2, double y2, double px, double py)
{

    return sqrt(ptSegDistSq(x1, y1, x2, y2, px, py));
}

double Line2D::ptSegDistSq(double px, double py)
{
    return ptSegDistSq(getX1(), getY1(), getX2(), getY2(), px, py);
}

double Line2D::ptSegDistSq(Point2D* pt)
{
    return ptSegDistSq(getX1(), getY1(), getX2(), getY2(), pt->getX(), pt->getY());
}

double Line2D::ptSegDist(double px, double py)
{
    return ptSegDist(getX1(), getY1(), getX2(), getY2(), px, py);
}

double Line2D::ptSegDist(Point2D* pt)
{
    return ptSegDist(getX1(), getY1(), getX2(), getY2(), pt->getX(), pt->getY());
}

double Line2D::ptLineDistSq(double x1, double y1, double x2, double y2, double px, double py)
{

    x2 -= x1;
    y2 -= y1;
    px -= x1;
    py -= y1;
    auto dotprod = px * x2 + py * y2;
    auto projlenSq = dotprod * dotprod / (x2 * x2 + y2 * y2);
    auto lenSq = px * px + py * py - projlenSq;
    if(lenSq < 0) {
        lenSq = 0;
    }
    return lenSq;
}

double Line2D::ptLineDist(double x1, double y1, double x2, double y2, double px, double py)
{

    return sqrt(ptLineDistSq(x1, y1, x2, y2, px, py));
}

double Line2D::ptLineDistSq(double px, double py)
{
    return ptLineDistSq(getX1(), getY1(), getX2(), getY2(), px, py);
}

double Line2D::ptLineDistSq(Point2D* pt)
{
    return ptLineDistSq(getX1(), getY1(), getX2(), getY2(), pt->getX(), pt->getY());
}

double Line2D::ptLineDist(double px, double py)
{
    return ptLineDist(getX1(), getY1(), getX2(), getY2(), px, py);
}

double Line2D::ptLineDist(Point2D* pt)
{
    return ptLineDist(getX1(), getY1(), getX2(), getY2(), pt->getX(), pt->getY());
}

bool Line2D::contains(double x, double y)
{
    return false;
}

bool Line2D::contains(Point2D* p)
{
    return false;
}

bool Line2D::intersects(double x, double y, double w, double h)
{
    return intersects(static_cast< Rectangle2D* >(new Rectangle2D_Double(x, y, w, h)));
}

bool Line2D::intersects(Rectangle2D* r)
{
    return r->intersectsLine(getX1(), getY1(), getX2(), getY2());
}

bool Line2D::contains(double x, double y, double w, double h)
{
    return false;
}

bool Line2D::contains(Rectangle2D* r)
{
    return false;
}

moduru::gui::Rectangle* Line2D::getBounds()
{
    return getBounds2D()->getBounds();
}

PathIterator* Line2D::getPathIterator(AffineTransform* at)
{
    return new LineIterator(this, at);
}

PathIterator* Line2D::getPathIterator(AffineTransform* at, double flatness)
{
    return new LineIterator(this, at);
}
