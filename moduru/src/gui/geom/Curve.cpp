#include <gui/geom/Curve.hpp>

#include <gui/geom/Crossings.hpp>
//#include <gui/geom/IllegalPathStateException.hpp>
#include <gui/geom/Order0.hpp>
#include <gui/geom/Order1.hpp>
#include <gui/geom/Order2.hpp>
#include <gui/geom/Order3.hpp>
#include <gui/geom/PathIterator.hpp>

#include <lang/Double.hpp>

#include <math/Math.hpp>

#ifdef __APPLE__
#include <cstdlib>
#include <math.h>
#endif

#ifdef __linux__
#include <stdexcept>
#include <math.h>
#endif

using namespace moduru::gui::geom;
using namespace std;

Curve::Curve(int direction)
{
	this->direction = direction;
}

const int Curve::INCREASING;
const int Curve::DECREASING;

void Curve::insertMove(vector<Curve*>* curves, double x, double y)
{
	curves->push_back(new Order0(x, y));
}

void Curve::insertLine(vector<Curve*>* curves, double x0, double y0, double x1, double y1)
{
    if(y0 < y1) {
        curves->push_back(new Order1(x0, y0, x1, y1, INCREASING));
    } else if(y0 > y1) {
        curves->push_back(new Order1(x1, y1, x0, y0, DECREASING));
    } else {
    }
}

void Curve::insertQuad(vector<Curve*>* curves, double x0, double y0, vector<double>* coords)
{

    auto y1 = (*coords)[3];
    if(y0 > y1) {
        Order2::insert(curves, coords, (*coords)[2], y1, (*coords)[0], (*coords)[1], x0, y0, DECREASING);
    } else if(y0 == y1 && y0 == (*coords)[1]) {
        return;
    } else {
        Order2::insert(curves, coords, x0, y0, (*coords)[0], (*coords)[1], (*coords)[2], y1, INCREASING);
    }
}

void Curve::insertCubic(vector<Curve*>* curves, double x0, double y0, vector<double>* coords)
{

    auto y1 = (*coords)[5];
    if(y0 > y1) {
        Order3::insert(curves, coords, (*coords)[4], y1, (*coords)[2], (*coords)[3], (*coords)[0], (*coords)[1], x0, y0, DECREASING);
    } else if(y0 == y1 && y0 == (*coords)[1] && y0 == (*coords)[3]) {
        return;
    } else {
        Order3::insert(curves, coords, x0, y0, (*coords)[0], (*coords)[1], (*coords)[2], (*coords)[3], (*coords)[4], y1, INCREASING);
    }
}

int Curve::pointCrossingsForPath(PathIterator* pi, double px, double py)
{

	if (pi->isDone()) {
		return 0;
	}
	auto coords = new vector<double>(6);
	if (pi->currentSegment(coords) != PathIterator::SEG_MOVETO) {
		throw std::invalid_argument("missing initial moveto in path definition");
	}
	pi->next();
	auto movx = (*coords)[0];
	auto movy = (*coords)[1];
	auto curx = movx;
	auto cury = movy;
	double endx, endy;
	auto crossings = 0;
	while (!pi->isDone()) {
		switch (pi->currentSegment(coords)) {
		case PathIterator::SEG_MOVETO:
			if (cury != movy) {
				crossings += pointCrossingsForLine(px, py, curx, cury, movx, movy);
			}
			movx = curx = (*coords)[0];
			movy = cury = (*coords)[1];
			break;
		case PathIterator::SEG_LINETO:
			endx = (*coords)[0];
			endy = (*coords)[1];
			crossings += pointCrossingsForLine(px, py, curx, cury, endx, endy);
			curx = endx;
			cury = endy;
			break;
		case PathIterator::SEG_QUADTO:
			endx = (*coords)[2];
			endy = (*coords)[3];
			crossings += pointCrossingsForQuad(px, py, curx, cury, (*coords)[0], (*coords)[1], endx, endy, 0);
			curx = endx;
			cury = endy;
			break;
		case PathIterator::SEG_CUBICTO:
			endx = (*coords)[4];
			endy = (*coords)[5];
			crossings += pointCrossingsForCubic(px, py, curx, cury, (*coords)[0], (*coords)[1], (*coords)[2], (*coords)[3], endx, endy, 0);
			curx = endx;
			cury = endy;
			break;
		case PathIterator::SEG_CLOSE:
			if (cury != movy) {
				crossings += pointCrossingsForLine(px, py, curx, cury, movx, movy);
			}
			curx = movx;
			cury = movy;
			break;
		}

		pi->next();
	}
	if (cury != movy) {
		crossings += pointCrossingsForLine(px, py, curx, cury, movx, movy);
	}
	return crossings;
}

int Curve::pointCrossingsForLine(double px, double py, double x0, double y0, double x1, double y1)
{

	if (py < y0 && py < y1)
		return 0;

	if (py >= y0 && py >= y1)
		return 0;

	if (px >= x0 && px >= x1)
		return 0;

	if (px < x0 && px < x1)
		return (y0 < y1) ? 1 : -1;

	auto xintercept = x0 + (py - y0) * (x1 - x0) / (y1 - y0);
	if (px >= xintercept)
		return 0;

	return (y0 < y1) ? 1 : -1;
}

int Curve::pointCrossingsForQuad(double px, double py, double x0, double y0, double xc, double yc, double x1, double y1, int level)
{

	if (py < y0 && py < yc && py < y1)
		return 0;

	if (py >= y0 && py >= yc && py >= y1)
		return 0;

	if (px >= x0 && px >= xc && px >= x1)
		return 0;

	if (px < x0 && px < xc && px < x1) {
		if (py >= y0) {
			if (py < y1)
				return 1;

		}
		else {
			if (py >= y1)
				return -1;

		}
		return 0;
	}
	if (level > 52)
		return pointCrossingsForLine(px, py, x0, y0, x1, y1);

	auto x0c = (x0 + xc) / 2;
	auto y0c = (y0 + yc) / 2;
	auto xc1 = (xc + x1) / 2;
	auto yc1 = (yc + y1) / 2;
	xc = (x0c + xc1) / 2;
	yc = (y0c + yc1) / 2;
	if (moduru::lang::Double::isNaN(xc) || moduru::lang::Double::isNaN(yc)) {
		return 0;
	}
	return (pointCrossingsForQuad(px, py, x0, y0, x0c, y0c, xc, yc, level + 1) + pointCrossingsForQuad(px, py, xc, yc, xc1, yc1, x1, y1, level + 1));
}

int Curve::pointCrossingsForCubic(double px, double py, double x0, double y0, double xc0, double yc0, double xc1, double yc1, double x1, double y1, int level)
{

    if(py < y0 && py < yc0 && py < yc1 && py < y1)
        return 0;

    if(py >= y0 && py >= yc0 && py >= yc1 && py >= y1)
        return 0;

    if(px >= x0 && px >= xc0 && px >= xc1 && px >= x1)
        return 0;

    if(px < x0 && px < xc0 && px < xc1 && px < x1) {
        if(py >= y0) {
            if(py < y1)
                return 1;

        } else {
            if(py >= y1)
                return -1;

        }
        return 0;
    }
    if(level > 52)
        return pointCrossingsForLine(px, py, x0, y0, x1, y1);

    auto xmid = (xc0 + xc1) / 2;
    auto ymid = (yc0 + yc1) / 2;
    xc0 = (x0 + xc0) / 2;
    yc0 = (y0 + yc0) / 2;
    xc1 = (xc1 + x1) / 2;
    yc1 = (yc1 + y1) / 2;
    auto xc0m = (xc0 + xmid) / 2;
    auto yc0m = (yc0 + ymid) / 2;
    auto xmc1 = (xmid + xc1) / 2;
    auto ymc1 = (ymid + yc1) / 2;
    xmid = (xc0m + xmc1) / 2;
    ymid = (yc0m + ymc1) / 2;
    if(moduru::lang::Double::isNaN(xmid) || moduru::lang::Double::isNaN(ymid)) {
        return 0;
    }
    return (pointCrossingsForCubic(px, py, x0, y0, xc0, yc0, xc0m, yc0m, xmid, ymid, level + 1) + pointCrossingsForCubic(px, py, xmid, ymid, xmc1, ymc1, xc1, yc1, x1, y1, level + 1));
}

const int Curve::RECT_INTERSECTS;

int Curve::rectCrossingsForPath(PathIterator* pi, double rxmin, double rymin, double rxmax, double rymax)
{

    if(rxmax <= rxmin || rymax <= rymin) {
        return 0;
    }
    if(pi->isDone()) {
        return 0;
    }
    auto coords = vector<double>(6);
    if(pi->currentSegment(&coords) != PathIterator::SEG_MOVETO) {
        throw std::invalid_argument("missing initial moveto in path definition");
    }
    pi->next();
    double curx, cury, movx, movy, endx, endy;
    curx = movx = coords[0];
    cury = movy = coords[1];
    auto crossings = 0;
    while (crossings != RECT_INTERSECTS && !pi->isDone()) {
        switch (pi->currentSegment(&coords)) {
        case PathIterator::SEG_MOVETO:
            if(curx != movx || cury != movy) {
                crossings = rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, curx, cury, movx, movy);
            }
            movx = curx = coords[0];
            movy = cury = coords[1];
            break;
        case PathIterator::SEG_LINETO:
            endx = coords[0];
            endy = coords[1];
            crossings = rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, curx, cury, endx, endy);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_QUADTO:
            endx = coords[2];
            endy = coords[3];
            crossings = rectCrossingsForQuad(crossings, rxmin, rymin, rxmax, rymax, curx, cury, coords[0], coords[1], endx, endy, 0);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_CUBICTO:
            endx = coords[4];
            endy = coords[5];
            crossings = rectCrossingsForCubic(crossings, rxmin, rymin, rxmax, rymax, curx, cury, coords[0], coords[1], coords[2], coords[3], endx, endy, 0);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_CLOSE:
            if(curx != movx || cury != movy) {
                crossings = rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, curx, cury, movx, movy);
            }
            curx = movx;
            cury = movy;
            break;
        }

        pi->next();
    }
    if(crossings != RECT_INTERSECTS && (curx != movx || cury != movy)) {
        crossings = rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, curx, cury, movx, movy);
    }
    return crossings;
}

int Curve::rectCrossingsForLine(int crossings, double rxmin, double rymin, double rxmax, double rymax, double x0, double y0, double x1, double y1)
{

    if(y0 >= rymax && y1 >= rymax)
        return crossings;

    if(y0 <= rymin && y1 <= rymin)
        return crossings;

    if(x0 <= rxmin && x1 <= rxmin)
        return crossings;

    if(x0 >= rxmax && x1 >= rxmax) {
        if(y0 < y1) {
            if(y0 <= rymin)
                crossings++;

            if(y1 >= rymax)
                crossings++;

        } else if(y1 < y0) {
            if(y1 <= rymin)
                crossings--;

            if(y0 >= rymax)
                crossings--;

        }
        return crossings;
    }
    if((x0 > rxmin && x0 < rxmax && y0 > rymin && y0 < rymax) || (x1 > rxmin && x1 < rxmax && y1 > rymin && y1 < rymax)) {
        return RECT_INTERSECTS;
    }
    auto xi0 = x0;
    if(y0 < rymin) {
        xi0 += ((rymin - y0) * (x1 - x0) / (y1 - y0));
    } else if(y0 > rymax) {
        xi0 += ((rymax - y0) * (x1 - x0) / (y1 - y0));
    }
    auto xi1 = x1;
    if(y1 < rymin) {
        xi1 += ((rymin - y1) * (x0 - x1) / (y0 - y1));
    } else if(y1 > rymax) {
        xi1 += ((rymax - y1) * (x0 - x1) / (y0 - y1));
    }
    if(xi0 <= rxmin && xi1 <= rxmin)
        return crossings;

    if(xi0 >= rxmax && xi1 >= rxmax) {
        if(y0 < y1) {
            if(y0 <= rymin)
                crossings++;

            if(y1 >= rymax)
                crossings++;

        } else if(y1 < y0) {
            if(y1 <= rymin)
                crossings--;

            if(y0 >= rymax)
                crossings--;

        }
        return crossings;
    }
    return RECT_INTERSECTS;
}

int Curve::rectCrossingsForQuad(int crossings, double rxmin, double rymin, double rxmax, double rymax, double x0, double y0, double xc, double yc, double x1, double y1, int level)
{

    if(y0 >= rymax && yc >= rymax && y1 >= rymax)
        return crossings;

    if(y0 <= rymin && yc <= rymin && y1 <= rymin)
        return crossings;

    if(x0 <= rxmin && xc <= rxmin && x1 <= rxmin)
        return crossings;

    if(x0 >= rxmax && xc >= rxmax && x1 >= rxmax) {
        if(y0 < y1) {
            if(y0 <= rymin && y1 > rymin)
                crossings++;

            if(y0 < rymax && y1 >= rymax)
                crossings++;

        } else if(y1 < y0) {
            if(y1 <= rymin && y0 > rymin)
                crossings--;

            if(y1 < rymax && y0 >= rymax)
                crossings--;

        }
        return crossings;
    }
    if((x0 < rxmax && x0 > rxmin && y0 < rymax && y0 > rymin) || (x1 < rxmax && x1 > rxmin && y1 < rymax && y1 > rymin)) {
        return RECT_INTERSECTS;
    }
    if(level > 52) {
        return rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, x0, y0, x1, y1);
    }
    auto x0c = (x0 + xc) / 2;
    auto y0c = (y0 + yc) / 2;
    auto xc1 = (xc + x1) / 2;
    auto yc1 = (yc + y1) / 2;
    xc = (x0c + xc1) / 2;
    yc = (y0c + yc1) / 2;
    if(moduru::lang::Double::isNaN(xc) || moduru::lang::Double::isNaN(yc)) {
        return 0;
    }
    crossings = rectCrossingsForQuad(crossings, rxmin, rymin, rxmax, rymax, x0, y0, x0c, y0c, xc, yc, level + 1);
    if(crossings != RECT_INTERSECTS) {
        crossings = rectCrossingsForQuad(crossings, rxmin, rymin, rxmax, rymax, xc, yc, xc1, yc1, x1, y1, level + 1);
    }
    return crossings;
}

int Curve::rectCrossingsForCubic(int crossings, double rxmin, double rymin, double rxmax, double rymax, double x0, double y0, double xc0, double yc0, double xc1, double yc1, double x1, double y1, int level)
{

    if(y0 >= rymax && yc0 >= rymax && yc1 >= rymax && y1 >= rymax) {
        return crossings;
    }
    if(y0 <= rymin && yc0 <= rymin && yc1 <= rymin && y1 <= rymin) {
        return crossings;
    }
    if(x0 <= rxmin && xc0 <= rxmin && xc1 <= rxmin && x1 <= rxmin) {
        return crossings;
    }
    if(x0 >= rxmax && xc0 >= rxmax && xc1 >= rxmax && x1 >= rxmax) {
        if(y0 < y1) {
            if(y0 <= rymin && y1 > rymin)
                crossings++;

            if(y0 < rymax && y1 >= rymax)
                crossings++;

        } else if(y1 < y0) {
            if(y1 <= rymin && y0 > rymin)
                crossings--;

            if(y1 < rymax && y0 >= rymax)
                crossings--;

        }
        return crossings;
    }
    if((x0 > rxmin && x0 < rxmax && y0 > rymin && y0 < rymax) || (x1 > rxmin && x1 < rxmax && y1 > rymin && y1 < rymax)) {
        return RECT_INTERSECTS;
    }
    if(level > 52) {
        return rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, x0, y0, x1, y1);
    }
    auto xmid = (xc0 + xc1) / 2;
    auto ymid = (yc0 + yc1) / 2;
    xc0 = (x0 + xc0) / 2;
    yc0 = (y0 + yc0) / 2;
    xc1 = (xc1 + x1) / 2;
    yc1 = (yc1 + y1) / 2;
    auto xc0m = (xc0 + xmid) / 2;
    auto yc0m = (yc0 + ymid) / 2;
    auto xmc1 = (xmid + xc1) / 2;
    auto ymc1 = (ymid + yc1) / 2;
    xmid = (xc0m + xmc1) / 2;
    ymid = (yc0m + ymc1) / 2;
    if(moduru::lang::Double::isNaN(xmid) || moduru::lang::Double::isNaN(ymid)) {
        return 0;
    }
    crossings = rectCrossingsForCubic(crossings, rxmin, rymin, rxmax, rymax, x0, y0, xc0, yc0, xc0m, yc0m, xmid, ymid, level + 1);
    if(crossings != RECT_INTERSECTS) {
        crossings = rectCrossingsForCubic(crossings, rxmin, rymin, rxmax, rymax, xmid, ymid, xmc1, ymc1, xc1, yc1, x1, y1, level + 1);
    }
    return crossings;
}

int Curve::getDirection()
{
    return direction;
}

Curve* Curve::getWithDirection(int direction)
{
    return (this->direction == direction ? this : getReversedCurve());
}

double Curve::round(double v)
{

    return v;
}

int Curve::orderof(double x1, double x2)
{

    if(x1 < x2) {
        return -1;
    }
    if(x1 > x2) {
        return 1;
    }
    return 0;
}

int64_t Curve::signeddiffbits(double y1, double y2)
{

    return (moduru::lang::Double::doubleToLongBits(y1) - moduru::lang::Double::doubleToLongBits(y2));
}

int64_t Curve::diffbits(double y1, double y2)
{

    return abs(moduru::lang::Double::doubleToLongBits(y1) - moduru::lang::Double::doubleToLongBits(y2));
}

double Curve::prev(double v)
{

    return moduru::lang::Double::longBitsToDouble(moduru::lang::Double::doubleToLongBits(v) - 1);
}

double Curve::next(double v)
{

    return moduru::lang::Double::longBitsToDouble(moduru::lang::Double::doubleToLongBits(v) + 1);
}

/*
string Curve::toString()
{
    return (::StringBuilder().append(u"Curve["_j)->append(getOrder())
        ->append(u", "_j)
        ->append((::StringBuilder().append(u"("_j)->append(round(getX0()))
            ->append(u", "_j)
            ->append(round(getY0()))
            ->append(u"), "_j)->toString()))
        ->append(controlPointString())
        ->append((::StringBuilder().append(u"("_j)->append(round(getX1()))
            ->append(u", "_j)
            ->append(round(getY1()))
            ->append(u"), "_j)->toString()))
        ->append((direction == INCREASING ? u"D"_j : u"U"_j))
        ->append(u"]"_j)->toString());
}

String* Curve::controlPointString()
{
    return u""_j;
}
*/

int Curve::crossingsFor(double x, double y)
{
    if(y >= getYTop() && y < getYBot()) {
        if(x < getXMax() && (x < getXMin() || x < XforY(y))) {
            return 1;
        }
    }
    return 0;
}

bool Curve::accumulateCrossings(Crossings* c)
{
    auto xhi = c->getXHi();
    if(getXMin() >= xhi) {
        return false;
    }
    auto xlo = c->getXLo();
    auto ylo = c->getYLo();
    auto yhi = c->getYHi();
    auto y0 = getYTop();
    auto y1 = getYBot();
    double tstart, ystart, tend, yend;
    if(y0 < ylo) {
        if(y1 <= ylo) {
            return false;
        }
        ystart = ylo;
        tstart = TforY(ylo);
    } else {
        if(y0 >= yhi) {
            return false;
        }
        ystart = y0;
        tstart = 0;
    }
    if(y1 > yhi) {
        yend = yhi;
        tend = TforY(yhi);
    } else {
        yend = y1;
        tend = 1;
    }
    auto hitLo = false;
    auto hitHi = false;
    while (true) {
        auto x = XforT(tstart);
        if(x < xhi) {
            if(hitHi || x > xlo) {
                return true;
            }
            hitLo = true;
        } else {
            if(hitLo) {
                return true;
            }
            hitHi = true;
        }
        if(tstart >= tend) {
            break;
        }
        tstart = nextVertical(tstart, tend);
    }
    if(hitLo) {
        c->record(ystart, yend, direction);
    }
    return false;
}

Curve* Curve::getSubCurve(double ystart, double yend)
{
    return getSubCurve(ystart, yend, direction);
}

int Curve::compareTo(Curve* that, vector<double>* yrange)
{
    auto y0 = (*yrange)[0];
    auto y1 = (*yrange)[1];
    y1 = moduru::math::Math::min(moduru::math::Math::min(y1, this->getYBot()), that->getYBot());
    if(y1 <= (*yrange)[0]) {
        //npc(::System::err())->println(::StringBuilder().append(u"this == "_j)->append(static_cast< moduru::lang::Object* >(this))->toString());
        //npc(::System::err())->println(::StringBuilder().append(u"that == "_j)->append(static_cast< moduru::lang::Object* >(that))->toString());
        //npc(::System::out())->println(::StringBuilder().append(u"target range = "_j)->append((*yrange)[0])
            //->append(u"=>"_j)
            //->append((*yrange)[1])->toString());
		throw std::invalid_argument("backstepping from ..."); //_j)->append((*yrange)[0])->append(u" to "_j)->append(y1)->toString());
    }
    (*yrange)[1] = y1;
    if(this->getXMax() <= that->getXMin()) {
        if(this->getXMin() == that->getXMax()) {
            return 0;
        }
        return -1;
    }
    if(this->getXMin() >= that->getXMax()) {
        return 1;
    }
    auto s0 = this->TforY(y0);
    auto ys0 = this->YforT(s0);
    if(ys0 < y0) {
        s0 = refineTforY(s0, ys0, y0);
        ys0 = this->YforT(s0);
    }
    auto s1 = this->TforY(y1);
    if(this->YforT(s1) < y0) {
        s1 = refineTforY(s1, this->YforT(s1), y0);
    }
    auto t0 = that->TforY(y0);
    auto yt0 = that->YforT(t0);
    if(yt0 < y0) {
        t0 = that->refineTforY(t0, yt0, y0);
        yt0 = that->YforT(t0);
    }
    auto t1 = that->TforY(y1);
    if(that->YforT(t1) < y0) {
        t1 = that->refineTforY(t1, that->YforT(t1), y0);
    }
    auto xs0 = this->XforT(s0);
    auto xt0 = that->XforT(t0);
    auto scale = moduru::math::Math::max(fabs(y0), fabs(y1));
    auto ymin = moduru::math::Math::max(scale * 1.0E-14, 1.0E-300);
    if(fairlyClose(xs0, xt0)) {
        auto bump = ymin;
        auto maxbump = moduru::math::Math::min(ymin * 1.0E13, (y1 - y0) * 0.1);
        auto y = y0 + bump;
        while (y <= y1) {
            if(fairlyClose(this->XforY(y), that->XforY(y))) {
                if((bump *= 2) > maxbump) {
                    bump = maxbump;
                }
            } else {
                y -= bump;
                while (true) {
                    bump /= 2;
                    auto newy = y + bump;
                    if(newy <= y) {
                        break;
                    }
                    if(fairlyClose(this->XforY(newy), that->XforY(newy))) {
                        y = newy;
                    }
                }
                break;
            }
            y += bump;
        }
        if(y > y0) {
            if(y < y1) {
                (*yrange)[1] = y;
            }
            return 0;
        }
    }
    if(ymin <= 0) {
        //npc(::System::out())->println(::StringBuilder().append(u"ymin = "_j)->append(ymin)->toString());
    }
    while (s0 < s1 && t0 < t1) {
        auto sh = this->nextVertical(s0, s1);
        auto xsh = this->XforT(sh);
        auto ysh = this->YforT(sh);
        auto th = that->nextVertical(t0, t1);
        auto xth = that->XforT(th);
        auto yth = that->YforT(th);
		try {
			if (findIntersect(that, yrange, ymin, 0, 0, s0, xs0, ys0, sh, xsh, ysh, t0, xt0, yt0, th, xth, yth)) {
				break;
			}
			//} catch (::Throwable* t) {
		} catch (exception* e) {
            /*
			npc(::System::err())->println(::StringBuilder().append(u"Error: "_j)->append(static_cast< moduru::lang::Object* >(t))->toString());
            npc(::System::err())->println(::StringBuilder().append(u"y range was "_j)->append((*yrange)[0])
                ->append(u"=>"_j)
                ->append((*yrange)[1])->toString());
            npc(::System::err())->println(::StringBuilder().append(u"s y range is "_j)->append(ys0)
                ->append(u"=>"_j)
                ->append(ysh)->toString());
            npc(::System::err())->println(::StringBuilder().append(u"t y range is "_j)->append(yt0)
                ->append(u"=>"_j)
                ->append(yth)->toString());
            npc(::System::err())->println(::StringBuilder().append(u"ymin is "_j)->append(ymin)->toString());
			*/
			throw e;
            //return 0;
        }
        if(ysh < yth) {
            if(ysh > (*yrange)[0]) {
                if(ysh < (*yrange)[1]) {
                    (*yrange)[1] = ysh;
                }
                break;
            }
            s0 = sh;
            xs0 = xsh;
            ys0 = ysh;
        } else {
            if(yth > (*yrange)[0]) {
                if(yth < (*yrange)[1]) {
                    (*yrange)[1] = yth;
                }
                break;
            }
            t0 = th;
            xt0 = xth;
            yt0 = yth;
        }
    }
    auto ymid = ((*yrange)[0] + (*yrange)[1]) / 2;
    return orderof(this->XforY(ymid), that->XforY(ymid));
}

bool Curve::findIntersect(Curve* that, vector<double>* yrange, double ymin, int slevel, int tlevel, double s0, double xs0, double ys0, double s1, double xs1, double ys1, double t0, double xt0, double yt0, double t1, double xt1, double yt1)
{
	if (ys0 > yt1 || yt0 > ys1) {
		return false;
	}
	if (moduru::math::Math::min(xs0, xs1) > moduru::math::Math::max(xt0, xt1) || moduru::math::Math::max(xs0, xs1) < moduru::math::Math::min(xt0, xt1)) {
		return false;
	}
	if (s1 - s0 > TMIN) {
		auto s = (s0 + s1) / 2;
		auto xs = this->XforT(s);
		auto ys = this->YforT(s);
		if (s == s0 || s == s1) {
			//npc(::System::out())->println(::StringBuilder().append(u"s0 = "_j)->append(s0)->toString());
			//npc(::System::out())->println(::StringBuilder().append(u"s1 = "_j)->append(s1)->toString());
			throw std::invalid_argument("no s progress!");
		}
		if (t1 - t0 > TMIN) {
			auto t = (t0 + t1) / 2;
			auto xt = that->XforT(t);
			auto yt = that->YforT(t);
			if (t == t0 || t == t1) {
				//npc(::System::out())->println(::StringBuilder().append(u"t0 = "_j)->append(t0)->toString());
				//npc(::System::out())->println(::StringBuilder().append(u"t1 = "_j)->append(t1)->toString());
				throw std::invalid_argument("no t progress!");
			}
			if (ys >= yt0 && yt >= ys0) {
				if (findIntersect(that, yrange, ymin, slevel + 1, tlevel + 1, s0, xs0, ys0, s, xs, ys, t0, xt0, yt0, t, xt, yt)) {
					return true;
				}
			}
			if (ys >= yt) {
				if (findIntersect(that, yrange, ymin, slevel + 1, tlevel + 1, s0, xs0, ys0, s, xs, ys, t, xt, yt, t1, xt1, yt1)) {
					return true;
				}
			}
			if (yt >= ys) {
				if (findIntersect(that, yrange, ymin, slevel + 1, tlevel + 1, s, xs, ys, s1, xs1, ys1, t0, xt0, yt0, t, xt, yt)) {
					return true;
				}
			}
			if (ys1 >= yt && yt1 >= ys) {
				if (findIntersect(that, yrange, ymin, slevel + 1, tlevel + 1, s, xs, ys, s1, xs1, ys1, t, xt, yt, t1, xt1, yt1)) {
					return true;
				}
			}
		}
		else {
			if (ys >= yt0) {
				if (findIntersect(that, yrange, ymin, slevel + 1, tlevel, s0, xs0, ys0, s, xs, ys, t0, xt0, yt0, t1, xt1, yt1)) {
					return true;
				}
			}
			if (yt1 >= ys) {
				if (findIntersect(that, yrange, ymin, slevel + 1, tlevel, s, xs, ys, s1, xs1, ys1, t0, xt0, yt0, t1, xt1, yt1)) {
					return true;
				}
			}
		}
	}
	else if (t1 - t0 > TMIN) {
		auto t = (t0 + t1) / 2;
		auto xt = that->XforT(t);
		auto yt = that->YforT(t);
		if (t == t0 || t == t1) {
			//npc(::System::out())->println(::StringBuilder().append(u"t0 = "_j)->append(t0)->toString());
			//npc(::System::out())->println(::StringBuilder().append(u"t1 = "_j)->append(t1)->toString());
			throw std::invalid_argument("no t progress!");
		}
		if (yt >= ys0) {
			if (findIntersect(that, yrange, ymin, slevel, tlevel + 1, s0, xs0, ys0, s1, xs1, ys1, t0, xt0, yt0, t, xt, yt)) {
				return true;
			}
		}
		if (ys1 >= yt) {
			if (findIntersect(that, yrange, ymin, slevel, tlevel + 1, s0, xs0, ys0, s1, xs1, ys1, t, xt, yt, t1, xt1, yt1)) {
				return true;
			}
		}
	}
	else {
		auto xlk = xs1 - xs0;
		auto ylk = ys1 - ys0;
		auto xnm = xt1 - xt0;
		auto ynm = yt1 - yt0;
		auto xmk = xt0 - xs0;
		auto ymk = yt0 - ys0;
		auto det = xnm * ylk - ynm * xlk;
		if (det != 0) {
			auto detinv = 1 / det;
			auto s = (xnm * ymk - ynm * xmk) * detinv;
			auto t = (xlk * ymk - ylk * xmk) * detinv;
			if (s >= 0 && s <= 1 && t >= 0 && t <= 1) {
				s = s0 + s * (s1 - s0);
				t = t0 + t * (t1 - t0);
				if (s < 0 || s > 1 || t < 0 || t > 1) {
					throw std::invalid_argument("Uh oh!");
				}
				auto y = (this->YforT(s) + that->YforT(t)) / 2;
				if (y <= (*yrange)[1] && y > (*yrange)[0]) {
					(*yrange)[1] = y;
					return true;
				}
			}
		}
	}
	return false;
}

double Curve::refineTforY(double t0, double yt0, double y0)
{
	double t1 = 1;
	while (true) {
		auto th = (t0 + t1) / 2;
		if (th == t0 || th == t1) {
			return t1;
		}
		auto y = YforT(th);
		if (y < y0) {
			t0 = th;
			yt0 = y;
		}
		else if (y > y0) {
			t1 = th;
		}
		else {
			return t1;
		}
	}
}

bool Curve::fairlyClose(double v1, double v2)
{
    return (fabs(v1 - v2) < moduru::math::Math::max(fabs(v1), fabs(v2)) * 1.0E-10);
}
