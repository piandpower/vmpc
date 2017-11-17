#include <gui/geom/QuadCurve2D.hpp>

#include <gui/Rectangle.hpp>
#include <gui/geom/FlatteningPathIterator.hpp>
#include <gui/geom/Line2D.hpp>
#include <gui/geom/Point2D.hpp>
#include <gui/geom/QuadIterator.hpp>
#include <gui/geom/Rectangle2D.hpp>

#include <cmath>

using namespace moduru::gui::geom;
using namespace std;

QuadCurve2D::QuadCurve2D() 
{
}

void QuadCurve2D::setCurve(vector<double>* coords, int offset)
{
    setCurve((*coords)[offset + 0], (*coords)[offset + 1], (*coords)[offset + 2], (*coords)[offset + 3], (*coords)[offset + 4], (*coords)[offset + 5]);
}

void QuadCurve2D::setCurve(Point2D* p1, Point2D* cp, Point2D* p2)
{
    setCurve(p1->getX(), p1->getY(), cp->getX(), cp->getY(), p2->getX(), p2->getY());
}

void QuadCurve2D::setCurve(vector<Point2D*>* pts, int offset)
{
    setCurve((*pts)[offset + 0]->getX(), (*pts)[offset + 0]->getY(), (*pts)[offset + 1]->getX(), (*pts)[offset + 1]->getY(), (*pts)[offset + 2]->getX(), (*pts)[offset + 2]->getY());
}

void QuadCurve2D::setCurve(QuadCurve2D* c)
{
    setCurve(c->getX1(), c->getY1(), c->getCtrlX(), c->getCtrlY(), c->getX2(), c->getY2());
}

double QuadCurve2D::getFlatnessSq(double x1, double y1, double ctrlx, double ctrly, double x2, double y2)
{
    
    return Line2D::ptSegDistSq(x1, y1, x2, y2, ctrlx, ctrly);
}

double QuadCurve2D::getFlatness(double x1, double y1, double ctrlx, double ctrly, double x2, double y2)
{
    
    return Line2D::ptSegDist(x1, y1, x2, y2, ctrlx, ctrly);
}

double QuadCurve2D::getFlatnessSq(vector<double>* coords, int offset)
{
    
    return Line2D::ptSegDistSq((*coords)[offset + 0], (*coords)[offset + 1], (*coords)[offset + 4], (*coords)[offset + 5], (*coords)[offset + 2], (*coords)[offset + 3]);
}

double QuadCurve2D::getFlatness(vector<double>* coords, int offset)
{
    
    return Line2D::ptSegDist((*coords)[offset + 0], (*coords)[offset + 1], (*coords)[offset + 4], (*coords)[offset + 5], (*coords)[offset + 2], (*coords)[offset + 3]);
}

double QuadCurve2D::getFlatnessSq()
{
    return Line2D::ptSegDistSq(getX1(), getY1(), getX2(), getY2(), getCtrlX(), getCtrlY());
}

double QuadCurve2D::getFlatness()
{
    return Line2D::ptSegDist(getX1(), getY1(), getX2(), getY2(), getCtrlX(), getCtrlY());
}

void QuadCurve2D::subdivide(QuadCurve2D* left, QuadCurve2D* right)
{
    subdivide(this, left, right);
}

void QuadCurve2D::subdivide(QuadCurve2D* src, QuadCurve2D* left, QuadCurve2D* right)
{

	auto x1 = src->getX1();
	auto y1 = src->getY1();
	auto ctrlx = src->getCtrlX();
	auto ctrly = src->getCtrlY();
	auto x2 = src->getX2();
	auto y2 = src->getY2();
	auto ctrlx1 = (x1 + ctrlx) / 2.0;
	auto ctrly1 = (y1 + ctrly) / 2.0;
	auto ctrlx2 = (x2 + ctrlx) / 2.0;
	auto ctrly2 = (y2 + ctrly) / 2.0;
	ctrlx = (ctrlx1 + ctrlx2) / 2.0;
	ctrly = (ctrly1 + ctrly2) / 2.0;
	if (left != nullptr) {
		left->setCurve(x1, y1, ctrlx1, ctrly1, ctrlx, ctrly);
	}
	if (right != nullptr) {
		right->setCurve(ctrlx, ctrly, ctrlx2, ctrly2, x2, y2);
	}
}

void QuadCurve2D::subdivide(vector<double>* src, int srcoff, vector<double>* left, int leftoff, vector<double>* right, int rightoff)
{
    
    auto x1 = (*src)[srcoff + 0];
    auto y1 = (*src)[srcoff + 1];
    auto ctrlx = (*src)[srcoff + 2];
    auto ctrly = (*src)[srcoff + 3];
    auto x2 = (*src)[srcoff + 4];
    auto y2 = (*src)[srcoff + 5];
    if(left != nullptr) {
        (*left)[leftoff + 0] = x1;
        (*left)[leftoff + 1] = y1;
    }
    if(right != nullptr) {
        (*right)[rightoff + 4] = x2;
        (*right)[rightoff + 5] = y2;
    }
    x1 = (x1 + ctrlx) / 2.0;
    y1 = (y1 + ctrly) / 2.0;
    x2 = (x2 + ctrlx) / 2.0;
    y2 = (y2 + ctrly) / 2.0;
    ctrlx = (x1 + x2) / 2.0;
    ctrly = (y1 + y2) / 2.0;
    if(left != nullptr) {
        (*left)[leftoff + 2] = x1;
        (*left)[leftoff + 3] = y1;
        (*left)[leftoff + 4] = ctrlx;
        (*left)[leftoff + 5] = ctrly;
    }
    if(right != nullptr) {
        (*right)[rightoff + 0] = ctrlx;
        (*right)[rightoff + 1] = ctrly;
        (*right)[rightoff + 2] = x2;
        (*right)[rightoff + 3] = y2;
    }
}

int QuadCurve2D::solveQuadratic(vector<double>* eqn)
{
    
    return solveQuadratic(eqn, eqn);
}

int QuadCurve2D::solveQuadratic(vector<double>* eqn, vector<double>* res)
{
    
    auto a = (*eqn)[2];
    auto b = (*eqn)[1];
    auto c = (*eqn)[0];
    auto roots = 0;
    if(a == 0.0) {
        if(b == 0.0) {
            return -1;
        }
        (*res)[roots++] = -c / b;
    } else {
        auto d = b * b - 4.0 * a * c;
        if(d < 0.0) {
            return 0;
        }
        d = sqrt(d);
        if(b < 0.0) {
            d = -d;
        }
        auto q = (b + d) / -2.0;
        (*res)[roots++] = q / a;
        if(q != 0.0) {
            (*res)[roots++] = c / q;
        }
    }
    return roots;
}

bool QuadCurve2D::contains(double x, double y)
{
    auto x1 = getX1();
    auto y1 = getY1();
    auto xc = getCtrlX();
    auto yc = getCtrlY();
    auto x2 = getX2();
    auto y2 = getY2();
    auto kx = x1 - 2 * xc + x2;
    auto ky = y1 - 2 * yc + y2;
    auto dx = x - x1;
    auto dy = y - y1;
    auto dxl = x2 - x1;
    auto dyl = y2 - y1;
    auto t0 = (dx * ky - dy * kx) / (dxl * ky - dyl * kx);
    if(t0 < 0 || t0 > 1 || t0 != t0) {
        return false;
    }
    auto xb = kx * t0 * t0 + 2 * (xc - x1) * t0 + x1;
    auto yb = ky * t0 * t0 + 2 * (yc - y1) * t0 + y1;
    auto xl = dxl * t0 + x1;
    auto yl = dyl * t0 + y1;
    return (x >= xb && x < xl) || (x >= xl && x < xb) || (y >= yb && y < yl)|| (y >= yl && y < yb);
}

bool QuadCurve2D::contains(Point2D* p)
{
    return contains(p->getX(), p->getY());
}

void QuadCurve2D::fillEqn(vector<double>* eqn, double val, double c1, double cp, double c2)
{
    
    (*eqn)[0] = c1 - val;
    (*eqn)[1] = cp + cp - c1 - c1;
    (*eqn)[2] = c1 - cp - cp + c2;
    return;
}

int QuadCurve2D::evalQuadratic(vector<double>* vals, int num, bool include0, bool include1, vector<double>* inflect, double c1, double ctrl, double c2)
{
    
    auto j = 0;
    for (auto i = 0; i < num; i++) {
        auto t = (*vals)[i];
        if((include0 ? t >= 0 : t > 0) && (include1 ? t <= 1 : t < 1) && (inflect == nullptr || (*inflect)[1] + 2 * (*inflect)[2] * t != 0)) {
            auto u = 1 - t;
            (*vals)[j++] = c1 * u * u + 2 * ctrl * t* u + c2 * t * t;
        }
    }
    return j;
}

constexpr int QuadCurve2D::BELOW;

constexpr int QuadCurve2D::LOWEDGE;

constexpr int QuadCurve2D::INSIDE;

constexpr int QuadCurve2D::HIGHEDGE;

constexpr int QuadCurve2D::ABOVE;

int QuadCurve2D::getTag(double coord, double low, double high)
{
    
    if(coord <= low) {
        return (coord < low ? BELOW : LOWEDGE);
    }
    if(coord >= high) {
        return (coord > high ? ABOVE : HIGHEDGE);
    }
    return INSIDE;
}

bool QuadCurve2D::inwards(int pttag, int opt1tag, int opt2tag)
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

bool QuadCurve2D::intersects(double x, double y, double w, double h)
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
	auto ctrlx = getCtrlX();
	auto ctrly = getCtrlY();
	auto ctrlxtag = getTag(ctrlx, x, x + w);
	auto ctrlytag = getTag(ctrly, y, y + h);
	if (x1tag < INSIDE && x2tag < INSIDE && ctrlxtag < INSIDE) {
		return false;
	}
	if (y1tag < INSIDE && y2tag < INSIDE && ctrlytag < INSIDE) {
		return false;
	}
	if (x1tag > INSIDE && x2tag > INSIDE && ctrlxtag > INSIDE) {
		return false;
	}
	if (y1tag > INSIDE && y2tag > INSIDE && ctrlytag > INSIDE) {
		return false;
	}
	if (inwards(x1tag, x2tag, ctrlxtag) && inwards(y1tag, y2tag, ctrlytag)) {
		return true;
	}
	if (inwards(x2tag, x1tag, ctrlxtag) && inwards(y2tag, y1tag, ctrlytag)) {
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
	auto eqn = vector<double>(3);
	auto res = vector<double>(3);
	if (!yoverlap) {
		fillEqn(&eqn, (y1tag < INSIDE ? y : y + h), y1, ctrly, y2);
		return (solveQuadratic(&eqn, &res) == 2 && evalQuadratic(&res, 2, true, true, nullptr, x1, ctrlx, x2) == 2 && getTag(res[0], x, x + w) * getTag(res[1], x, x + w) <= 0);
	}
	if (!xoverlap) {
		fillEqn(&eqn, (x1tag < INSIDE ? x : x + w), x1, ctrlx, x2);
		return (solveQuadratic(&eqn, &res) == 2 && evalQuadratic(&res, 2, true, true, nullptr, y1, ctrly, y2) == 2 && getTag(res[0], y, y + h) * getTag(res[1], y, y + h) <= 0);
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
	fillEqn(&eqn, (c2tag < INSIDE ? x : x + w), x1, ctrlx, x2);
	auto num = solveQuadratic(&eqn, &res);
	evalQuadratic(&res, num, true, true, nullptr, y1, ctrly, y2);
	c2tag = getTag(res[0], y, y + h);
	return (c1tag * c2tag <= 0);
}

bool QuadCurve2D::intersects(Rectangle2D* r)
{
    return intersects(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

bool QuadCurve2D::contains(double x, double y, double w, double h)
{
    if(w <= 0 || h <= 0) {
        return false;
    }
    return (contains(x, y) && contains(x + w, y) && contains(x + w, y + h)&& contains(x, y + h));
}

bool QuadCurve2D::contains(Rectangle2D* r)
{
    return contains(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

moduru::gui::Rectangle* QuadCurve2D::getBounds()
{
    return getBounds2D()->getBounds();
}

PathIterator* QuadCurve2D::getPathIterator(AffineTransform* at)
{
	return new QuadIterator(this, at);
}

PathIterator* QuadCurve2D::getPathIterator(AffineTransform* at, double flatness)
{
	return new FlatteningPathIterator(getPathIterator(at), flatness);
}
