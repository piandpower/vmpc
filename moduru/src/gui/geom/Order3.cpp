#include <gui/geom/Order3.hpp>

#include <gui/geom/Curve.hpp>
#include <gui/geom/Order2.hpp>
#include <gui/geom/PathIterator.hpp>
//#include <gui/geom/QuadCurve2D.hpp>
#include <gui/geom/Rectangle2D.hpp>

#include <math/Math.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace moduru::gui::geom;
using namespace std;

Order3::Order3(double x0, double y0, double cx0, double cy0, double cx1, double cy1, double x1, double y1, int direction) : Curve(direction)
{
	if (cy0 < y0)
		cy0 = y0;

	if (cy1 > y1)
		cy1 = y1;

	this->x0 = x0;
	this->y0 = y0;
	this->cx0 = cx0;
	this->cy0 = cy0;
	this->cx1 = cx1;
	this->cy1 = cy1;
	this->x1 = x1;
	this->y1 = y1;
	xmin = moduru::math::Math::min(moduru::math::Math::min(x0, x1), moduru::math::Math::min(cx0, cx1));
	xmax = moduru::math::Math::max(moduru::math::Math::max(x0, x1), moduru::math::Math::max(cx0, cx1));
	xcoeff0 = x0;
	xcoeff1 = (cx0 - x0) * 3.0;
	xcoeff2 = (cx1 - cx0 - cx0 + x0) * 3.0;
	xcoeff3 = x1 - (cx1 - cx0) * 3.0 - x0;
	ycoeff0 = y0;
	ycoeff1 = (cy0 - y0) * 3.0;
	ycoeff2 = (cy1 - cy0 - cy0 + y0) * 3.0;
	ycoeff3 = y1 - (cy1 - cy0) * 3.0 - y0;
	YforT1 = YforT2 = YforT3 = y0;
}

void Order3::insert(vector<Curve*>* curves, vector<double>* tmp, double x0, double y0, double cx0, double cy0, double cx1, double cy1, double x1, double y1, int direction)
{
	auto numparams = getHorizontalParams(y0, cy0, cy1, y1, tmp);
	if (numparams == 0) {
		addInstance(curves, x0, y0, cx0, cy0, cx1, cy1, x1, y1, direction);
		return;
	}
	(*tmp)[3] = x0;
	(*tmp)[4] = y0;
	(*tmp)[5] = cx0;
	(*tmp)[6] = cy0;
	(*tmp)[7] = cx1;
	(*tmp)[8] = cy1;
	(*tmp)[9] = x1;
	(*tmp)[10] = y1;
	auto t = (*tmp)[0];
	if (numparams > 1 && t > (*tmp)[1]) {
		(*tmp)[0] = (*tmp)[1];
		(*tmp)[1] = t;
		t = (*tmp)[0];
	}
	split(tmp, 3, t);
	if (numparams > 1) {
		t = ((*tmp)[1] - t) / (1 - t);
		split(tmp, 9, t);
	}
	auto index = 3;
	if (direction == DECREASING) {
		index += numparams * 6;
	}
	while (numparams >= 0) {
		addInstance(curves, (*tmp)[index + 0], (*tmp)[index + 1], (*tmp)[index + 2], (*tmp)[index + 3], (*tmp)[index + 4], (*tmp)[index + 5], (*tmp)[index + 6], (*tmp)[index + 7], direction);
		numparams--;
		if (direction == INCREASING) {
			index += 6;
		}
		else {
			index -= 6;
		}
	}
}

void Order3::addInstance(vector<Curve*>* curves, double x0, double y0, double cx0, double cy0, double cx1, double cy1, double x1, double y1, int direction)
{

	if (y0 > y1) {
		curves->push_back(new Order3(x1, y1, cx1, cy1, cx0, cy0, x0, y0, -direction));
	}
	else if (y1 > y0) {
		curves->push_back(new Order3(x0, y0, cx0, cy0, cx1, cy1, x1, y1, direction));
	}
}

int Order3::getHorizontalParams(double c0, double cp0, double cp1, double c1, vector<double>* ret)
{
    /*
    if(c0 <= cp0 && cp0 <= cp1 && cp1 <= c1) {
        return 0;
    }
    c1 -= cp1;
    cp1 -= cp0;
    cp0 -= c0;
    (*ret)[0] = cp0;
    (*ret)[1] = (cp1 - cp0) * 2;
    (*ret)[2] = (c1 - cp1 - cp1 + cp0);
    auto numroots = QuadCurve2D::solveQuadratic(ret, ret);
    auto j = 0;
    for (auto i = 0; i < numroots; i++) {
        auto t = (*ret)[i];
        if(t > 0 && t < 1) {
            if(j < i) {
                (*ret)[j] = t;
            }
            j++;
        }
    }
    return j;
	*/
	return 0;
}

void Order3::split(vector<double>* coords, int pos, double t)
{
    
    double x0, y0, cx0, cy0, cx1, cy1, x1, y1;
    (*coords)[pos + 12] = x1 = (*coords)[pos + 6];
    (*coords)[pos + 13] = y1 = (*coords)[pos + 7];
    cx1 = (*coords)[pos + 4];
    cy1 = (*coords)[pos + 5];
    x1 = cx1 + (x1 - cx1) * t;
    y1 = cy1 + (y1 - cy1) * t;
    x0 = (*coords)[pos + 0];
    y0 = (*coords)[pos + 1];
    cx0 = (*coords)[pos + 2];
    cy0 = (*coords)[pos + 3];
    x0 = x0 + (cx0 - x0) * t;
    y0 = y0 + (cy0 - y0) * t;
    cx0 = cx0 + (cx1 - cx0) * t;
    cy0 = cy0 + (cy1 - cy0) * t;
    cx1 = cx0 + (x1 - cx0) * t;
    cy1 = cy0 + (y1 - cy0) * t;
    cx0 = x0 + (cx0 - x0) * t;
    cy0 = y0 + (cy0 - y0) * t;
    (*coords)[pos + 2] = x0;
    (*coords)[pos + 3] = y0;
    (*coords)[pos + 4] = cx0;
    (*coords)[pos + 5] = cy0;
    (*coords)[pos + 6] = cx0 + (cx1 - cx0) * t;
    (*coords)[pos + 7] = cy0 + (cy1 - cy0) * t;
    (*coords)[pos + 8] = cx1;
    (*coords)[pos + 9] = cy1;
    (*coords)[pos + 10] = x1;
    (*coords)[pos + 11] = y1;
}

int Order3::getOrder()
{
    return 3;
}

double Order3::getXTop()
{
    return x0;
}

double Order3::getYTop()
{
    return y0;
}

double Order3::getXBot()
{
    return x1;
}

double Order3::getYBot()
{
    return y1;
}

double Order3::getXMin()
{
    return xmin;
}

double Order3::getXMax()
{
    return xmax;
}

double Order3::getX0()
{
    return (direction == INCREASING) ? x0 : x1;
}

double Order3::getY0()
{
    return (direction == INCREASING) ? y0 : y1;
}

double Order3::getCX0()
{
    return (direction == INCREASING) ? cx0 : cx1;
}

double Order3::getCY0()
{
    return (direction == INCREASING) ? cy0 : cy1;
}

double Order3::getCX1()
{
    return (direction == DECREASING) ? cx0 : cx1;
}

double Order3::getCY1()
{
    return (direction == DECREASING) ? cy0 : cy1;
}

double Order3::getX1()
{
    return (direction == DECREASING) ? x0 : x1;
}

double Order3::getY1()
{
    return (direction == DECREASING) ? y0 : y1;
}

double Order3::TforY(double y)
{
    if(y <= y0)
        return 0;

    if(y >= y1)
        return 1;

    if(y == YforT1)
        return TforY1;

    if(y == YforT2)
        return TforY2;

    if(y == YforT3)
        return TforY3;

    if(ycoeff3 == 0.0) {
        return Order2::TforY(y, ycoeff0, ycoeff1, ycoeff2);
    }
    auto a = ycoeff2 / ycoeff3;
    auto b = ycoeff1 / ycoeff3;
    auto c = (ycoeff0 - y) / ycoeff3;
    auto roots = 0;
    auto Q = (a * a - 3.0 * b) / 9.0;
    auto R = (2.0 * a * a* a - 9.0 * a * b + 27.0 * c) / 54.0;
    auto R2 = R * R;
    auto Q3 = Q * Q * Q;
    auto a_3 = a / 3.0;
    double t;
    if(R2 < Q3) {
        auto theta = acos(R / sqrt(Q3));
        Q = -2.0 * sqrt(Q);
        t = refine(a, b, c, y, Q * cos(theta / 3.0) - a_3);
        if(t < 0) {
            t = refine(a, b, c, y, Q * cos((theta + M_PI * 2.0) / 3.0) - a_3);
        }
        if(t < 0) {
            t = refine(a, b, c, y, Q * cos((theta - M_PI * 2.0) / 3.0) - a_3);
        }
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
        t = refine(a, b, c, y, (A + B) - a_3);
    }
    if(t < 0) {
        double t0 = 0;
        double t1 = 1;
        while (true) {
            t = (t0 + t1) / 2;
            if(t == t0 || t == t1) {
                break;
            }
            auto yt = YforT(t);
            if(yt < y) {
                t0 = t;
            } else if(yt > y) {
                t1 = t;
            } else {
                break;
            }
        }
    }
    if(t >= 0) {
        TforY3 = TforY2;
        YforT3 = YforT2;
        TforY2 = TforY1;
        YforT2 = YforT1;
        TforY1 = t;
        YforT1 = y;
    }
    return t;
}

double Order3::refine(double a, double b, double c, double target, double t)
{
	if (t < -0.1 || t > 1.1) {
		return -1;
	}
	auto y = YforT(t);
	double t0, t1;
	if (y < target) {
		t0 = t;
		t1 = 1;
	}
	else {
		t0 = 0;
		t1 = t;
	}
	auto origt = t;
	auto origy = y;
	auto useslope = true;
	while (y != target) {
		if (!useslope) {
			auto t2 = (t0 + t1) / 2;
			if (t2 == t0 || t2 == t1) {
				break;
			}
			t = t2;
		}
		else {
			auto slope = dYforT(t, 1);
			if (slope == 0) {
				useslope = false;
				continue;
			}
			auto t2 = t + ((target - y) / slope);
			if (t2 == t || t2 <= t0 || t2 >= t1) {
				useslope = false;
				continue;
			}
			t = t2;
		}
		y = YforT(t);
		if (y < target) {
			t0 = t;
		}
		else if (y > target) {
			t1 = t;
		}
		else {
			break;
		}
	}
	auto verbose = false;
	if (false && t >= 0 && t <= 1) {
		y = YforT(t);
		auto tdiff = diffbits(t, origt);
		auto ydiff = diffbits(y, origy);
		auto yerr = diffbits(y, target);
		if (yerr > 0 || (verbose && tdiff > 0)) {
			/*
			npc(::System::out())->println(::StringBuilder().append(u"target was y = "_j)->append(target)->toString());
			npc(::System::out())->println(::StringBuilder().append(u"original was y = "_j)->append(origy)
				->append(u", t = "_j)
				->append(origt)->toString());
			npc(::System::out())->println(::StringBuilder().append(u"final was y = "_j)->append(y)
				->append(u", t = "_j)
				->append(t)->toString());
			npc(::System::out())->println(::StringBuilder().append(u"t diff is "_j)->append(tdiff)->toString());
			npc(::System::out())->println(::StringBuilder().append(u"y diff is "_j)->append(ydiff)->toString());
			npc(::System::out())->println(::StringBuilder().append(u"y error is "_j)->append(yerr)->toString());
			*/
			auto tlow = prev(t);
			auto ylow = YforT(tlow);
			auto thi = next(t);
			auto yhi = YforT(thi);
			if (fabs(target - ylow) < fabs(target - y) || fabs(target - yhi) < fabs(target - y)) {
				/*
				npc(::System::out())->println(::StringBuilder().append(u"adjacent y's = ["_j)->append(ylow)
					->append(u", "_j)
					->append(yhi)
					->append(u"]"_j)->toString());
					*/
			}
		}
	}
	return (t > 1) ? static_cast<double>(-1) : t;
}

double Order3::XforY(double y)
{
    if(y <= y0) {
        return x0;
    }
    if(y >= y1) {
        return x1;
    }
    return XforT(TforY(y));
}

double Order3::XforT(double t)
{
    return (((xcoeff3 * t) + xcoeff2) * t + xcoeff1) * t + xcoeff0;
}

double Order3::YforT(double t)
{
    return (((ycoeff3 * t) + ycoeff2) * t + ycoeff1) * t + ycoeff0;
}

double Order3::dXforT(double t, int deriv)
{
    switch (deriv) {
    case 0:
        return (((xcoeff3 * t) + xcoeff2) * t + xcoeff1) * t + xcoeff0;
    case 1:
        return ((3 * xcoeff3 * t) + 2 * xcoeff2) * t + xcoeff1;
    case 2:
        return (6 * xcoeff3 * t) + 2 * xcoeff2;
    case 3:
        return 6 * xcoeff3;
    default:
        return 0;
    }

}

double Order3::dYforT(double t, int deriv)
{
    switch (deriv) {
    case 0:
        return (((ycoeff3 * t) + ycoeff2) * t + ycoeff1) * t + ycoeff0;
    case 1:
        return ((3 * ycoeff3 * t) + 2 * ycoeff2) * t + ycoeff1;
    case 2:
        return (6 * ycoeff3 * t) + 2 * ycoeff2;
    case 3:
        return 6 * ycoeff3;
    default:
        return 0;
    }

}

double Order3::nextVertical(double t0, double t1)
{
    auto eqn = vector<double>({ xcoeff1 , 2 * xcoeff2 , 3 * xcoeff3 });
    //auto numroots = QuadCurve2D::solveQuadratic(eqn, eqn);
    //for (auto i = 0; i < numroots; i++) {
    //    if(eqn[i] > t0 && eqn[i] < t1) {
    //        t1 = eqn[i];
    //   }
    //}
    return t1;
}

void Order3::enlarge(Rectangle2D* r)
{
	r->add(x0, y0);
	auto eqn = vector<double>({ xcoeff1, 2 * xcoeff2, 3 * xcoeff3 });
	/*
	auto numroots = QuadCurve2D::solveQuadratic(eqn, eqn);
	for (auto i = 0; i < numroots; i++) {
		auto t = eqn[i];
		if(t > 0 && t < 1) {
			r->add(XforT(t), YforT(t));
		}
	}
	*/
	r->add(x1, y1);
}

Curve* Order3::getSubCurve(double ystart, double yend, int dir)
{
    if(ystart <= y0 && yend >= y1) {
        return getWithDirection(dir);
    }
    auto eqn = vector<double>(14);
    double t0, t1;
    t0 = TforY(ystart);
    t1 = TforY(yend);
    eqn[0] = x0;
    eqn[1] = y0;
    eqn[2] = cx0;
    eqn[3] = cy0;
    eqn[4] = cx1;
    eqn[5] = cy1;
    eqn[6] = x1;
    eqn[7] = y1;
    if(t0 > t1) {
        auto t = t0;
        t0 = t1;
        t1 = t;
    }
    if(t1 < 1) {
        split(&eqn, 0, t1);
    }
    int i;
    if(t0 <= 0) {
        i = 0;
    } else {
        split(&eqn, 0, t0 / t1);
        i = 6;
    }
    return new Order3(eqn[i + 0], ystart, eqn[i + 2], eqn[i + 3], eqn[i + 4], eqn[i + 5], eqn[i + 6], yend, dir);
}

Curve* Order3::getReversedCurve()
{
    return new Order3(x0, y0, cx0, cy0, cx1, cy1, x1, y1, -direction);
}

int Order3::getSegment(vector<double>* coords)
{
    if(direction == INCREASING) {
        (*coords)[0] = cx0;
        (*coords)[1] = cy0;
        (*coords)[2] = cx1;
        (*coords)[3] = cy1;
        (*coords)[4] = x1;
        (*coords)[5] = y1;
    } else {
        (*coords)[0] = cx1;
        (*coords)[1] = cy1;
        (*coords)[2] = cx0;
        (*coords)[3] = cy0;
        (*coords)[4] = x0;
        (*coords)[5] = y0;
    }
    return PathIterator::SEG_CUBICTO;
}
