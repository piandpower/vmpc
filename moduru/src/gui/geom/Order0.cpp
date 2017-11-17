#include <gui/geom/Order0.hpp>

#include <gui/geom/Crossings.hpp>
#include <gui/geom/Curve.hpp>
#include <gui/geom/PathIterator.hpp>
#include <gui/geom/Rectangle2D.hpp>

using namespace moduru::gui::geom;
using namespace std;

Order0::Order0(double x, double y) : Curve(INCREASING)
{
	this->x = x;
	this->y = y;
}

int Order0::getOrder()
{
    return 0;
}

double Order0::getXTop()
{
    return x;
}

double Order0::getYTop()
{
    return y;
}

double Order0::getXBot()
{
    return x;
}

double Order0::getYBot()
{
    return y;
}

double Order0::getXMin()
{
    return x;
}

double Order0::getXMax()
{
    return x;
}

double Order0::getX0()
{
    return x;
}

double Order0::getY0()
{
    return y;
}

double Order0::getX1()
{
    return x;
}

double Order0::getY1()
{
    return y;
}

double Order0::XforY(double y)
{
    return y;
}

double Order0::TforY(double y)
{
    return 0;
}

double Order0::XforT(double t)
{
    return x;
}

double Order0::YforT(double t)
{
    return y;
}

double Order0::dXforT(double t, int deriv)
{
    return 0;
}

double Order0::dYforT(double t, int deriv)
{
    return 0;
}

double Order0::nextVertical(double t0, double t1)
{
    return t1;
}

int Order0::crossingsFor(double x, double y)
{
    return 0;
}

bool Order0::accumulateCrossings(Crossings* c)
{
    return (x > c->getXLo() && x < c->getXHi() && y > c->getYLo() && y < c->getYHi());
}

void Order0::enlarge(Rectangle2D* r)
{
    r->add(x, y);
}

Curve* Order0::getSubCurve(double ystart, double yend, int dir)
{
    return this;
}

Curve* Order0::getReversedCurve()
{
    return this;
}

int Order0::getSegment(vector<double>* coords)
{
    (*coords)[0] = x;
    (*coords)[1] = y;
    return PathIterator::SEG_MOVETO;
}
