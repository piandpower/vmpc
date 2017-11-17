#include <gui/geom/Crossings.hpp>

#include <gui/geom/Crossings_EvenOdd.hpp>
#include <gui/geom/Crossings_NonZero.hpp>
#include <gui/geom/Curve.hpp>
#include <gui/geom/PathIterator.hpp>

#include <math/Math.hpp>

#include <cmath>

using namespace moduru::gui::geom;
using namespace std;

Crossings::Crossings(double xlo, double ylo, double xhi, double yhi) 
{
	yranges = vector<double>(10);
	this->xlo = xlo;
	this->ylo = ylo;
	this->xhi = xhi;
	this->yhi = yhi;
}

double Crossings::getXLo()
{
    return xlo;
}

double Crossings::getYLo()
{
    return ylo;
}

double Crossings::getXHi()
{
    return xhi;
}

double Crossings::getYHi()
{
    return yhi;
}

bool Crossings::isEmpty()
{
    return (limit == 0);
}

Crossings* Crossings::findCrossings(vector<Curve*> curves, double xlo, double ylo, double xhi, double yhi)
{
    Crossings* cross = new Crossings_EvenOdd(xlo, ylo, xhi, yhi);
    for (auto& c : curves) {
        if(c->accumulateCrossings(cross)) {
			delete cross;
            return nullptr;
        }
    }
    return cross;
}

Crossings* Crossings::findCrossings(PathIterator* pi, double xlo, double ylo, double xhi, double yhi)
{
    Crossings* cross;
    if(pi->getWindingRule() == pi->WIND_EVEN_ODD) {
        cross = new Crossings_EvenOdd(xlo, ylo, xhi, yhi);
    } else {
        cross = new Crossings_NonZero(xlo, ylo, xhi, yhi);
    }
    auto coords = vector<double>(23);
    double movx = 0;
    double movy = 0;
    double curx = 0;
    double cury = 0;
    double newx, newy;
    while (!pi->isDone()) {
        auto type = pi->currentSegment(&coords);
        switch (type) {
        case PathIterator::SEG_MOVETO:
            if(movy != cury && cross->accumulateLine(curx, cury, movx, movy)) {
				delete cross;
                return nullptr;
            }
            movx = curx = coords[0];
            movy = cury = coords[1];
            break;
        case PathIterator::SEG_LINETO:
            newx = coords[0];
            newy = coords[1];
            if(cross->accumulateLine(curx, cury, newx, newy)) {
				delete cross;
				return nullptr;
            }
            curx = newx;
            cury = newy;
            break;
        case PathIterator::SEG_QUADTO:
            newx = coords[2];
            newy = coords[3];
            if(cross->accumulateQuad(curx, cury, coords)) {
				delete cross; 
				return nullptr;
            }
            curx = newx;
            cury = newy;
            break;
        case PathIterator::SEG_CUBICTO:
            newx = coords[4];
            newy = coords[5];
            if(cross->accumulateCubic(curx, cury, coords)) {
				delete cross; 
				return nullptr;
            }
            curx = newx;
            cury = newy;
            break;
        case PathIterator::SEG_CLOSE:
            if(movy != cury && cross->accumulateLine(curx, cury, movx, movy)) {
				delete cross; 
				return nullptr;
            }
            curx = movx;
            cury = movy;
            break;
        }

        pi->next();
    }
    if(movy != cury) {
        if(cross->accumulateLine(curx, cury, movx, movy)) {
			delete cross; 
			return nullptr;
        }
    }
    return cross;
}

bool Crossings::accumulateLine(double x0, double y0, double x1, double y1)
{
    if(y0 <= y1) {
        return accumulateLine(x0, y0, x1, y1, 1);
    } else {
        return accumulateLine(x1, y1, x0, y0, -1);
    }
}

bool Crossings::accumulateLine(double x0, double y0, double x1, double y1, int direction)
{
    if(yhi <= y0 || ylo >= y1) {
        return false;
    }
    if(x0 >= xhi && x1 >= xhi) {
        return false;
    }
    if(y0 == y1) {
        return (x0 >= xlo || x1 >= xlo);
    }
    double xstart, ystart, xend, yend;
    auto dx = (x1 - x0);
    auto dy = (y1 - y0);
    if(y0 < ylo) {
        xstart = x0 + (ylo - y0) * dx / dy;
        ystart = ylo;
    } else {
        xstart = x0;
        ystart = y0;
    }
    if(yhi < y1) {
        xend = x0 + (yhi - y0) * dx / dy;
        yend = yhi;
    } else {
        xend = x1;
        yend = y1;
    }
    if(xstart >= xhi && xend >= xhi) {
        return false;
    }
    if(xstart > xlo || xend > xlo) {
        return true;
    }
    record(ystart, yend, direction);
    return false;
}

bool Crossings::accumulateQuad(double x0, double y0, vector<double> coords)
{
    if(y0 < ylo && coords[1] < ylo && coords[3] < ylo) {
        return false;
    }
    if(y0 > yhi && coords[1] > yhi && coords[3] > yhi) {
        return false;
    }
    if(x0 > xhi && coords[0] > xhi && coords[2] > xhi) {
        return false;
    }
    if(x0 < xlo && coords[0] < xlo && coords[2] < xlo) {
        if(y0 < coords[3]) {
            record(moduru::math::Math::max(y0, ylo), moduru::math::Math::min((long) coords[3], yhi), 1);
        } else if(y0 > coords[3]) {
            record(moduru::math::Math::max(coords[3], ylo), moduru::math::Math::min((long) y0, yhi), -1);
        }
        return false;
    }
    Curve::insertQuad(&tmp, x0, y0, &coords);
    for (auto& c : tmp) {
        if(c->accumulateCrossings(this)) {
            return true;
        }
    }
    tmp.clear();
    return false;
}

bool Crossings::accumulateCubic(double x0, double y0, vector<double> coords)
{
	if (y0 < ylo && coords[1] < ylo && coords[3] < ylo && coords[5] < ylo) {
		return false;
	}
	if (y0 > yhi && coords[1] > yhi && coords[3] > yhi && coords[5] > yhi) {
		return false;
	}
	if (x0 > xhi && coords[0] > xhi && coords[2] > xhi && coords[4] > xhi) {
		return false;
	}
	if (x0 < xlo && coords[0] < xlo && coords[2] < xlo && coords[4] < xlo) {
		if (y0 <= coords[5]) {
			record(moduru::math::Math::max(y0, ylo), moduru::math::Math::min((long) coords[5], yhi), 1);
		}
		else {
			record(moduru::math::Math::max(coords[5], ylo), moduru::math::Math::min((long) y0, yhi), -1);
		}
		return false;
	}
	Curve::insertCubic(&tmp, x0, y0, &coords);
	for (auto& c : tmp) {
		if (c->accumulateCrossings(this)) {
			return true;
		}
	}
	tmp.clear();
	return false;
}

Crossings::~Crossings() {
}
