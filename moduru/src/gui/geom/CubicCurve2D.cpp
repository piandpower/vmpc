#include <gui/geom/CubicCurve2D.hpp>

#include <gui/Rectangle.hpp>
#include <gui/geom/CubicIterator.hpp>
#include <gui/geom/FlatteningPathIterator.hpp>
#include <gui/geom/Line2D.hpp>
#include <gui/geom/Point2D.hpp>
#include <gui/geom/QuadCurve2D.hpp>
#include <gui/geom/Rectangle2D_Double.hpp>
#include <gui/geom/Rectangle2D.hpp>

#include <gui/geom/Curve.hpp>

#include <math/Math.hpp>
#include <VecUtil.hpp>

#include <algorithm>

#define _USE_MATH_DEFINES

#include <math.h>

#ifdef __APPLE__
#include <cstdlib>
#include <cfloat>
#endif

#ifdef __linux__
#include <cfloat>
#endif

using namespace moduru::gui::geom;
using namespace std;

CubicCurve2D::CubicCurve2D()
{
}

void CubicCurve2D::setCurve(vector<double>* coords, int offset)
{
    setCurve((*coords)[offset + 0], (*coords)[offset + 1], (*coords)[offset + 2], (*coords)[offset + 3], (*coords)[offset + 4], (*coords)[offset + 5], (*coords)[offset + 6], (*coords)[offset + 7]);
}

void CubicCurve2D::setCurve(Point2D* p1, Point2D* cp1, Point2D* cp2, Point2D* p2)
{
    setCurve(p1->getX(), p1->getY(), cp1->getX(), cp1->getY(), cp2->getX(), cp2->getY(), p2->getX(), p2->getY());
}

void CubicCurve2D::setCurve(vector<Point2D*>* pts, int offset)
{
    setCurve((*pts)[offset + 0]->getX(), (*pts)[offset + 0]->getY(), (*pts)[offset + 1]->getX(), (*pts)[offset + 1]->getY(), (*pts)[offset + 2]->getX(), (*pts)[offset + 2]->getY(), (*pts)[offset + 3]->getX(), (*pts)[offset + 3]->getY());
}

void CubicCurve2D::setCurve(CubicCurve2D* c)
{
    setCurve(c->getX1(), c->getY1(), c->getCtrlX1(), c->getCtrlY1(), c->getCtrlX2(), c->getCtrlY2(), c->getX2(), c->getY2());
}

double CubicCurve2D::getFlatnessSq(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2)
{
    return moduru::math::Math::max(Line2D::ptSegDistSq(x1, y1, x2, y2, ctrlx1, ctrly1), Line2D::ptSegDistSq(x1, y1, x2, y2, ctrlx2, ctrly2));
}

double CubicCurve2D::getFlatness(double x1, double y1, double ctrlx1, double ctrly1, double ctrlx2, double ctrly2, double x2, double y2)
{
    return sqrt(getFlatnessSq(x1, y1, ctrlx1, ctrly1, ctrlx2, ctrly2, x2, y2));
}

double CubicCurve2D::getFlatnessSq(vector<double>* coords, int offset)
{
    return getFlatnessSq((*coords)[offset + 0], (*coords)[offset + 1], (*coords)[offset + 2], (*coords)[offset + 3], (*coords)[offset + 4], (*coords)[offset + 5], (*coords)[offset + 6], (*coords)[offset + 7]);
}

double CubicCurve2D::getFlatness(vector<double>* coords, int offset)
{
    return getFlatness((*coords)[offset + 0], (*coords)[offset + 1], (*coords)[offset + 2], (*coords)[offset + 3], (*coords)[offset + 4], (*coords)[offset + 5], (*coords)[offset + 6], (*coords)[offset + 7]);
}

double CubicCurve2D::getFlatnessSq()
{
    return getFlatnessSq(getX1(), getY1(), getCtrlX1(), getCtrlY1(), getCtrlX2(), getCtrlY2(), getX2(), getY2());
}

double CubicCurve2D::getFlatness()
{
    return getFlatness(getX1(), getY1(), getCtrlX1(), getCtrlY1(), getCtrlX2(), getCtrlY2(), getX2(), getY2());
}

void CubicCurve2D::subdivide(CubicCurve2D* left, CubicCurve2D* right)
{
    subdivide(this, left, right);
}

void CubicCurve2D::subdivide(CubicCurve2D* src, CubicCurve2D* left, CubicCurve2D* right)
{
    auto x1 = src->getX1();
    auto y1 = src->getY1();
    auto ctrlx1 = src->getCtrlX1();
    auto ctrly1 = src->getCtrlY1();
    auto ctrlx2 = src->getCtrlX2();
    auto ctrly2 = src->getCtrlY2();
    auto x2 = src->getX2();
    auto y2 = src->getY2();
    auto centerx = (ctrlx1 + ctrlx2) / 2.0;
    auto centery = (ctrly1 + ctrly2) / 2.0;
    ctrlx1 = (x1 + ctrlx1) / 2.0;
    ctrly1 = (y1 + ctrly1) / 2.0;
    ctrlx2 = (x2 + ctrlx2) / 2.0;
    ctrly2 = (y2 + ctrly2) / 2.0;
    auto ctrlx12 = (ctrlx1 + centerx) / 2.0;
    auto ctrly12 = (ctrly1 + centery) / 2.0;
    auto ctrlx21 = (ctrlx2 + centerx) / 2.0;
    auto ctrly21 = (ctrly2 + centery) / 2.0;
    centerx = (ctrlx12 + ctrlx21) / 2.0;
    centery = (ctrly12 + ctrly21) / 2.0;
    if(left != nullptr) {
        left->setCurve(x1, y1, ctrlx1, ctrly1, ctrlx12, ctrly12, centerx, centery);
    }
    if(right != nullptr) {
        right->setCurve(centerx, centery, ctrlx21, ctrly21, ctrlx2, ctrly2, x2, y2);
    }
}

void CubicCurve2D::subdivide(vector<double>* src, int srcoff, vector<double>* left, int leftoff, vector<double>* right, int rightoff)
{
    auto x1 = (*src)[srcoff + 0];
    auto y1 = (*src)[srcoff + 1];
    auto ctrlx1 = (*src)[srcoff + 2];
    auto ctrly1 = (*src)[srcoff + 3];
    auto ctrlx2 = (*src)[srcoff + 4];
    auto ctrly2 = (*src)[srcoff + 5];
    auto x2 = (*src)[srcoff + 6];
    auto y2 = (*src)[srcoff + 7];
    if(left != nullptr) {
        (*left)[leftoff + 0] = x1;
        (*left)[leftoff + 1] = y1;
    }
    if(right != nullptr) {
        (*right)[rightoff + 6] = x2;
        (*right)[rightoff + 7] = y2;
    }
    x1 = (x1 + ctrlx1) / 2.0;
    y1 = (y1 + ctrly1) / 2.0;
    x2 = (x2 + ctrlx2) / 2.0;
    y2 = (y2 + ctrly2) / 2.0;
    auto centerx = (ctrlx1 + ctrlx2) / 2.0;
    auto centery = (ctrly1 + ctrly2) / 2.0;
    ctrlx1 = (x1 + centerx) / 2.0;
    ctrly1 = (y1 + centery) / 2.0;
    ctrlx2 = (x2 + centerx) / 2.0;
    ctrly2 = (y2 + centery) / 2.0;
    centerx = (ctrlx1 + ctrlx2) / 2.0;
    centery = (ctrly1 + ctrly2) / 2.0;
    if(left != nullptr) {
        (*left)[leftoff + 2] = x1;
        (*left)[leftoff + 3] = y1;
        (*left)[leftoff + 4] = ctrlx1;
        (*left)[leftoff + 5] = ctrly1;
        (*left)[leftoff + 6] = centerx;
        (*left)[leftoff + 7] = centery;
    }
    if(right != nullptr) {
        (*right)[rightoff + 0] = centerx;
        (*right)[rightoff + 1] = centery;
        (*right)[rightoff + 2] = ctrlx2;
        (*right)[rightoff + 3] = ctrly2;
        (*right)[rightoff + 4] = x2;
        (*right)[rightoff + 5] = y2;
    }
}

int CubicCurve2D::solveCubic(vector<double>* eqn)
{
    return solveCubic(eqn, eqn);
}

int CubicCurve2D::solveCubic(vector<double>* eqn, vector<double>* res)
{
    auto d = (*eqn)[3];
    if(d == 0.0) {
        return QuadCurve2D::solveQuadratic(eqn, res);
    }
    auto a = (*eqn)[2] / d;
    auto b = (*eqn)[1] / d;
    auto c = (*eqn)[0] / d;
    auto roots = 0;
    auto Q = (a * a - 3.0 * b) / 9.0;
    auto R = (2.0 * a * a* a - 9.0 * a * b + 27.0 * c) / 54.0;
    auto R2 = R * R;
    auto Q3 = Q * Q * Q;
    a = a / 3.0;
    if(R2 < Q3) {
        auto theta = acos(R / sqrt(Q3));
        Q = -2.0 * sqrt(Q);
        if(res == eqn) {
            eqn = new vector<double>(4);
            moduru::VecUtil::VecCopy(res, 0, eqn, 0, 4);
        }
        (*res)[roots++] = Q * cos(theta / 3.0) - a;
        (*res)[roots++] = Q * cos((theta + M_PI * 2.0) / 3.0) - a;
        (*res)[roots++] = Q * cos((theta - M_PI * 2.0) / 3.0) - a;
        fixRoots(res, eqn);
    } else {
        auto neg = (R < 0.0);
        auto S = sqrt(R2 - Q3);
        if(neg) {
            R = -R;
        }
        auto A = pow(R + S, 1.0 / 3.0);
        if(!neg) {
            A = -A;
        }
        auto B = (A == 0.0) ? 0.0 : (Q / A);
        (*res)[roots++] = (A + B) - a;
    }
    return roots;
}

void CubicCurve2D::fixRoots(vector<double>* res, vector<double>* eqn)
{
    auto const EPSILON = 1.0E-5;
    for (auto i = 0; i < 3; i++) {
        auto t = (*res)[i];
        if(fabs(t) < EPSILON) {
            (*res)[i] = findZero(t, 0, eqn);
        } else if(fabs(t - 1) < EPSILON) {
            (*res)[i] = findZero(t, 1, eqn);
        }
    }
}

double CubicCurve2D::solveEqn(vector<double>* eqn, int order, double t)
{
    auto v = (*eqn)[order];
    while (--order >= 0) {
        v = v * t + (*eqn)[order];
    }
    return v;
}

double CubicCurve2D::findZero(double t, double target, vector<double>* eqn)
{
    auto slopeqn = (new vector<double>({
        (*eqn)[1]
        , 2 * (*eqn)[2]
        , 3 * (*eqn)[3]
    }));
    double slope;
    double origdelta = 0;
    auto origt = t;
    while (true) {
        slope = solveEqn(slopeqn, 2, t);
        if(slope == 0) {
            return t;
        }
        auto y = solveEqn(eqn, 3, t);
        if(y == 0) {
            return t;
        }
        auto delta = -(y / slope);
        if(origdelta == 0) {
            origdelta = delta;
        }
        if(t < target) {
            if(delta < 0)
                return t;

        } else if(t > target) {
            if(delta > 0)
                return t;

        } else {
            return (delta > 0 ? (target + DBL_MIN) : (target - DBL_MIN));
        }
        auto newt = t + delta;
        if(t == newt) {
            return t;
        }
        if(delta * origdelta < 0) {
            auto tag = (origt < t ? getTag(target, origt, t) : getTag(target, t, origt));
            if(tag != INSIDE) {
                return (origt + t) / 2;
            }
            t = target;
        } else {
            t = newt;
        }
    }
}

bool CubicCurve2D::contains(double x, double y)
{
    if(!(x * 0.0 + y * 0.0 == 0.0)) {
        return false;
    }
    auto x1 = getX1();
    auto y1 = getY1();
    auto x2 = getX2();
    auto y2 = getY2();
    auto crossings = (moduru::gui::geom::Curve::pointCrossingsForLine(x, y, x1, y1, x2, y2) + moduru::gui::geom::Curve::pointCrossingsForCubic(x, y, x1, y1, getCtrlX1(), getCtrlY1(), getCtrlX2(), getCtrlY2(), x2, y2, 0));
    return ((crossings & 1) == 1);
}

bool CubicCurve2D::contains(Point2D* p)
{
    return contains(p->getX(), p->getY());
}

void CubicCurve2D::fillEqn(vector<double>* eqn, double val, double c1, double cp1, double cp2, double c2)
{
    (*eqn)[0] = c1 - val;
    (*eqn)[1] = (cp1 - c1) * 3.0;
    (*eqn)[2] = (cp2 - cp1 - cp1 + c1) * 3.0;
    (*eqn)[3] = c2 + (cp1 - cp2) * 3.0 - c1;
    return;
}

int CubicCurve2D::evalCubic(vector<double>* vals, int num, bool include0, bool include1, vector<double>* inflect, double c1, double cp1, double cp2, double c2)
{
    auto j = 0;
    for (auto i = 0; i < num; i++) {
        auto t = (*vals)[i];
        if((include0 ? t >= 0 : t > 0) && (include1 ? t <= 1 : t < 1) && (inflect == nullptr || (*inflect)[1] + (2 * (*inflect)[2] + 3 * (*inflect)[3] * t) * t != 0)) {
            auto u = 1 - t;
            (*vals)[j++] = c1 * u * u* u + 3 * cp1 * t* u* u + 3 * cp2 * t* t* u + c2 * t * t* t;
        }
    }
    return j;
}

constexpr int CubicCurve2D::BELOW;
constexpr int CubicCurve2D::LOWEDGE;
constexpr int CubicCurve2D::INSIDE;
constexpr int CubicCurve2D::HIGHEDGE;
constexpr int CubicCurve2D::ABOVE;

int CubicCurve2D::getTag(double coord, double low, double high)
{
    if(coord <= low) {
        return (coord < low ? BELOW : LOWEDGE);
    }
    if(coord >= high) {
        return (coord > high ? ABOVE : HIGHEDGE);
    }
    return INSIDE;
}

bool CubicCurve2D::inwards(int pttag, int opt1tag, int opt2tag)
{
    switch (pttag) {
    case BELOW:
    case ABOVE:
    default:
        return false;
    case LOWEDGE:
        return (opt1tag >= INSIDE || opt2tag >= INSIDE);
    case INSIDE:
        return true;
    case HIGHEDGE:
        return (opt1tag <= INSIDE || opt2tag <= INSIDE);
    }

}

bool CubicCurve2D::intersects(double x, double y, double w, double h)
{
	if (w <= 0 || h <= 0) {
		return false;
	}
	auto x1 = getX1();
	auto y1 = getY1();
	auto x1tag = getTag(x1, x, x + w);
	auto y1tag = getTag(y1, y, y + h);
	if (x1tag == INSIDE && y1tag == INSIDE) {
		return true;
	}
	auto x2 = getX2();
	auto y2 = getY2();
	auto x2tag = getTag(x2, x, x + w);
	auto y2tag = getTag(y2, y, y + h);
	if (x2tag == INSIDE && y2tag == INSIDE) {
		return true;
	}
	auto ctrlx1 = getCtrlX1();
	auto ctrly1 = getCtrlY1();
	auto ctrlx2 = getCtrlX2();
	auto ctrly2 = getCtrlY2();
	auto ctrlx1tag = getTag(ctrlx1, x, x + w);
	auto ctrly1tag = getTag(ctrly1, y, y + h);
	auto ctrlx2tag = getTag(ctrlx2, x, x + w);
	auto ctrly2tag = getTag(ctrly2, y, y + h);
	if (x1tag < INSIDE && x2tag < INSIDE && ctrlx1tag < INSIDE && ctrlx2tag < INSIDE) {
		return false;
	}
	if (y1tag < INSIDE && y2tag < INSIDE && ctrly1tag < INSIDE && ctrly2tag < INSIDE) {
		return false;
	}
	if (x1tag > INSIDE && x2tag > INSIDE && ctrlx1tag > INSIDE && ctrlx2tag > INSIDE) {
		return false;
	}
	if (y1tag > INSIDE && y2tag > INSIDE && ctrly1tag > INSIDE && ctrly2tag > INSIDE) {
		return false;
	}
	if (inwards(x1tag, x2tag, ctrlx1tag) && inwards(y1tag, y2tag, ctrly1tag)) {
		return true;
	}
	if (inwards(x2tag, x1tag, ctrlx2tag) && inwards(y2tag, y1tag, ctrly2tag)) {
		return true;
	}
	auto xoverlap = (x1tag * x2tag <= 0);
	auto yoverlap = (y1tag * y2tag <= 0);
	if (x1tag == INSIDE && x2tag == INSIDE && yoverlap) {
		return true;
	}
	if (y1tag == INSIDE && y2tag == INSIDE && xoverlap) {
		return true;
	}
	auto eqn = new vector<double>(4);
	auto res = new vector<double>(4);
	if (!yoverlap) {
		fillEqn(eqn, (y1tag < INSIDE ? y : y + h), y1, ctrly1, ctrly2, y2);
		auto num = solveCubic(eqn, res);
		num = evalCubic(res, num, true, true, nullptr, x1, ctrlx1, ctrlx2, x2);
		return (num == 2 && getTag((*res)[0], x, x + w) * getTag((*res)[1], x, x + w) <= 0);
	}
	if (!xoverlap) {
		fillEqn(eqn, (x1tag < INSIDE ? x : x + w), x1, ctrlx1, ctrlx2, x2);
		auto num = solveCubic(eqn, res);
		num = evalCubic(res, num, true, true, nullptr, y1, ctrly1, ctrly2, y2);
		return (num == 2 && getTag((*res)[0], y, y + h) * getTag((*res)[1], y, y + h) <= 0);
	}
	auto dx = x2 - x1;
	auto dy = y2 - y1;
	auto k = y2 * x1 - x2 * y1;
	int c1tag, c2tag;
	if (y1tag == INSIDE) {
		c1tag = x1tag;
	}
	else {
		c1tag = getTag((k + dx * (y1tag < INSIDE ? y : y + h)) / dy, x, x + w);
	}
	if (y2tag == INSIDE) {
		c2tag = x2tag;
	}
	else {
		c2tag = getTag((k + dx * (y2tag < INSIDE ? y : y + h)) / dy, x, x + w);
	}
	if (c1tag * c2tag <= 0) {
		return true;
	}
	c1tag = ((c1tag * x1tag <= 0) ? y1tag : y2tag);
	fillEqn(eqn, (c2tag < INSIDE ? x : x + w), x1, ctrlx1, ctrlx2, x2);
	auto num = solveCubic(eqn, res);
	num = evalCubic(res, num, true, true, nullptr, y1, ctrly1, ctrly2, y2);
	auto tags = new vector<int>(num + 1);
	for (auto i = 0; i < num; i++) {
		(*tags)[i] = getTag((*res)[i], y, y + h);
	}
	(*tags)[num] = c1tag;
	sort(tags->begin(), tags->end());
	return ((num >= 1 && (*tags)[0] * (*tags)[1] <= 0) || (num >= 3 && (*tags)[2] * (*tags)[3] <= 0));
}

bool CubicCurve2D::intersects(Rectangle2D* r)
{
    return intersects(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

bool CubicCurve2D::contains(double x, double y, double w, double h)
{
    if(w <= 0 || h <= 0) {
        return false;
    }
    if(!(contains(x, y) && contains(x + w, y) && contains(x + w, y + h)&& contains(x, y + h))) {
        return false;
    }
    Rectangle2D* rect = new Rectangle2D_Double(x, y, w, h);
    return !rect->intersectsLine(getX1(), getY1(), getX2(), getY2());
}

bool CubicCurve2D::contains(Rectangle2D* r)
{
    return contains(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

moduru::gui::Rectangle* CubicCurve2D::getBounds()
{
    return getBounds2D()->getBounds();
}

PathIterator* CubicCurve2D::getPathIterator(AffineTransform* at)
{
    return new CubicIterator(this, at);
}

PathIterator* CubicCurve2D::getPathIterator(AffineTransform* at, double flatness)
{
    return new FlatteningPathIterator(getPathIterator(at), flatness);
}
