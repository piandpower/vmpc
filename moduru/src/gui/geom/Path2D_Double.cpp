#include <gui/geom/Path2D_Double.hpp>

#include <gui/Shape.hpp>
#include <gui/geom/AffineTransform.hpp>
//#include <gui/geom/IllegalPathStateException.hpp>
//#include <gui/geom/Path2D_Double_CopyIterator.hpp>
//#include <gui/geom/Path2D_Double_TxIterator.hpp>
#include <gui/geom/Path2D.hpp>
#include <gui/geom/PathIterator.hpp>
#include <gui/geom/Point2D_Double.hpp>
#include <gui/geom/Rectangle2D_Double.hpp>
#include <gui/geom/Curve.hpp>

#include <VecUtil.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::gui::geom;
using namespace std;

Path2D_Double::Path2D_Double() : Path2D_Double(WIND_NON_ZERO, INIT_SIZE)
{
}

Path2D_Double::Path2D_Double(int rule) : Path2D_Double(rule, INIT_SIZE)
{
}

Path2D_Double::Path2D_Double(int rule, int initialCapacity) : Path2D(rule, initialCapacity)
{
	doubleCoords = vector<double>(initialCapacity * 2);
}

Path2D_Double::Path2D_Double(moduru::gui::Shape* s) : Path2D_Double(s, static_cast< AffineTransform* >(nullptr))
{
}

Path2D_Double::Path2D_Double(moduru::gui::Shape* s, AffineTransform* at)
{
	if (dynamic_cast<Path2D*>(s) != nullptr) {
		auto p2d = dynamic_cast<Path2D*>(s);
		setWindingRule(p2d->windingRule);
		this->numTypes = p2d->numTypes;
		this->pointTypes = moduru::VecUtil::CopyOf(p2d->pointTypes, p2d->pointTypes.size());
		this->numCoords = p2d->numCoords;
		this->doubleCoords = p2d->cloneCoordsDouble(at);
	}
	else {
		auto pi = s->getPathIterator(at);
		setWindingRule(pi->getWindingRule());
		this->pointTypes = vector<int8_t>(INIT_SIZE);
		this->doubleCoords = vector<double>(INIT_SIZE * 2);
		append(pi, false);
	}
}

vector<float> Path2D_Double::cloneCoordsFloat(AffineTransform* at)
{
	auto ret = vector<float>(doubleCoords.size());
	if (at == nullptr) {
		for (int i = 0; i < numCoords; i++) {
			ret[i] = static_cast<float>(doubleCoords[i]);
		}
	}
	else {
		at->transform(doubleCoords, 0, &ret, 0, numCoords / 2);
	}
	return ret;
}

vector<double> Path2D_Double::cloneCoordsDouble(AffineTransform* at)
{
	vector<double> ret;
	if (at == nullptr) {
		//ret = moduru::VecUtil::CopyOf(doubleCoords, doubleCoords.size());
		VecUtil::VecCopy(&doubleCoords, 0, &ret, 0, doubleCoords.size());
	}
	else {
		ret = vector<double>(doubleCoords.size());
		at->transform(doubleCoords, 0, &ret, 0, numCoords / 2);
	}
	return ret;
}

void Path2D_Double::append(float x, float y)
{
    doubleCoords[numCoords++] = x;
    doubleCoords[numCoords++] = y;
}

void Path2D_Double::append(double x, double y)
{
    doubleCoords[numCoords++] = x;
    doubleCoords[numCoords++] = y;
}

Point2D* Path2D_Double::getPoint(int coordindex)
{
    return new Point2D_Double(doubleCoords[coordindex], doubleCoords[coordindex + 1]);
}

void Path2D_Double::needRoom(bool needMove, int newCoords)
{
	if (needMove && numTypes == 0) {
		//throw new IllegalPathStateException(::StringBuilder().append("missing initial moveto ")->append("in path definition")->toString());
		throw std::invalid_argument("illegal path state exception");
	}
	auto size = pointTypes.size();
	if (numTypes >= size) {
		auto grow = size;
		if (grow > EXPAND_MAX) {
			grow = EXPAND_MAX;
		}
		pointTypes = moduru::VecUtil::CopyOf(pointTypes, size + grow);
	}
	size = doubleCoords.size();
	if (numCoords + newCoords > size) {
		auto grow = size;
		if (grow > EXPAND_MAX * 2) {
			grow = EXPAND_MAX * 2;
		}
		if (grow < newCoords) {
			grow = newCoords;
		}
		vector<double> copy;
		moduru::VecUtil::VecCopy(&doubleCoords, 0, &copy, 0, doubleCoords.size());
		while (copy.size() < size + grow)
			copy.push_back(0);
		//doubleCoords = moduru::VecUtil::CopyOf(doubleCoords, size + grow);
		doubleCoords = copy;
	}
}

void Path2D_Double::moveTo(double x, double y)
{
    if(numTypes > 0 && pointTypes[numTypes - 1] == SEG_MOVETO) {
        doubleCoords[numCoords - 2] = x;
        doubleCoords[numCoords - 1] = y;
    } else {
        needRoom(false, 2);
        pointTypes[numTypes++] = SEG_MOVETO;
        doubleCoords[numCoords++] = x;
        doubleCoords[numCoords++] = y;
    }
}

void Path2D_Double::lineTo(double x, double y)
{
    needRoom(true, 2);
    pointTypes[numTypes++] = SEG_LINETO;
    doubleCoords[numCoords++] = x;
    doubleCoords[numCoords++] = y;
}

void Path2D_Double::quadTo(double x1, double y1, double x2, double y2)
{
    needRoom(true, 4);
    pointTypes[numTypes++] = SEG_QUADTO;
    doubleCoords[numCoords++] = x1;
    doubleCoords[numCoords++] = y1;
    doubleCoords[numCoords++] = x2;
    doubleCoords[numCoords++] = y2;
}

void Path2D_Double::curveTo(double x1, double y1, double x2, double y2, double x3, double y3)
{
    needRoom(true, 6);
    pointTypes[numTypes++] = SEG_CUBICTO;
    doubleCoords[numCoords++] = x1;
    doubleCoords[numCoords++] = y1;
    doubleCoords[numCoords++] = x2;
    doubleCoords[numCoords++] = y2;
    doubleCoords[numCoords++] = x3;
    doubleCoords[numCoords++] = y3;
}

int Path2D_Double::pointCrossings(double px, double py)
{
    double movx, movy, curx, cury, endx, endy;
    auto coords = doubleCoords;
    curx = movx = coords[0];
    cury = movy = coords[1];
    int crossings = 0;
    auto ci = 2;
    for (int i = 1; i < numTypes; i++) {
        switch (pointTypes[i]) {
        case PathIterator::SEG_MOVETO:
            if(cury != movy) {
                crossings += Curve::pointCrossingsForLine(px, py, curx, cury, movx, movy);
            }
            movx = curx = coords[ci++];
            movy = cury = coords[ci++];
            break;
        case PathIterator::SEG_LINETO:
            crossings += Curve::pointCrossingsForLine(px, py, curx, cury, endx = coords[ci++], endy = coords[ci++]);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_QUADTO:
            crossings += Curve::pointCrossingsForQuad(px, py, curx, cury, coords[ci++], coords[ci++], endx = coords[ci++], endy = coords[ci++], 0);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_CUBICTO:
            crossings += Curve::pointCrossingsForCubic(px, py, curx, cury, coords[ci++], coords[ci++], coords[ci++], coords[ci++], endx = coords[ci++], endy = coords[ci++], 0);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_CLOSE:
            if(cury != movy) {
                crossings += Curve::pointCrossingsForLine(px, py, curx, cury, movx, movy);
            }
            curx = movx;
            cury = movy;
            break;
        }

    }
    if(cury != movy) {
        crossings += Curve::pointCrossingsForLine(px, py, curx, cury, movx, movy);
    }
    return crossings;
}

int Path2D_Double::rectCrossings(double rxmin, double rymin, double rxmax, double rymax)
{
    auto coords = doubleCoords;
    double curx, cury, movx, movy, endx, endy;
    curx = movx = coords[0];
    cury = movy = coords[1];
    auto crossings = 0;
    auto ci = 2;
    for (auto i = 1; crossings != Curve::RECT_INTERSECTS && i < numTypes; i++) {
        switch (pointTypes[i]) {
        case PathIterator::SEG_MOVETO:
            if(curx != movx || cury != movy) {
                crossings = Curve::rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, curx, cury, movx, movy);
            }
            movx = curx = coords[ci++];
            movy = cury = coords[ci++];
            break;
        case PathIterator::SEG_LINETO:
            endx = coords[ci++];
            endy = coords[ci++];
            crossings = Curve::rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, curx, cury, endx, endy);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_QUADTO:
            crossings = Curve::rectCrossingsForQuad(crossings, rxmin, rymin, rxmax, rymax, curx, cury, coords[ci++], coords[ci++], endx = coords[ci++], endy = coords[ci++], 0);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_CUBICTO:
            crossings = Curve::rectCrossingsForCubic(crossings, rxmin, rymin, rxmax, rymax, curx, cury, coords[ci++], coords[ci++], coords[ci++], coords[ci++], endx = coords[ci++], endy = coords[ci++], 0);
            curx = endx;
            cury = endy;
            break;
        case PathIterator::SEG_CLOSE:
            if(curx != movx || cury != movy) {
                crossings = Curve::rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, curx, cury, movx, movy);
            }
            curx = movx;
            cury = movy;
            break;
        }

    }
    if(crossings != Curve::RECT_INTERSECTS && (curx != movx || cury != movy)) {
        crossings = Curve::rectCrossingsForLine(crossings, rxmin, rymin, rxmax, rymax, curx, cury, movx, movy);
    }
    return crossings;
}

void Path2D_Double::append(PathIterator* pi, bool connect)
{
    auto coords = vector<double>(6);
    while (!pi->isDone()) {
        switch (pi->currentSegment(&coords)) {
        case SEG_MOVETO:
            if(!connect || numTypes < 1 || numCoords < 1) {
                moveTo(coords[0], coords[1]);
                break;
            }
            if(pointTypes[numTypes - 1] != SEG_CLOSE && doubleCoords[numCoords - 2] == coords[0] && doubleCoords[numCoords - 1] == coords[1]) {
                break;
            }
        case SEG_LINETO:
            lineTo(coords[0], coords[1]);
            break;
        case SEG_QUADTO:
            quadTo(coords[0], coords[1], coords[2], coords[3]);
            break;
        case SEG_CUBICTO:
            curveTo(coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
            break;
        case SEG_CLOSE:
            closePath();
            break;
        }

        pi->next();
        connect = false;
    }
}

void Path2D_Double::transform(AffineTransform* at)
{
	at->transform(doubleCoords, 0, &doubleCoords, 0, numCoords / 2);
}

Rectangle2D* Path2D_Double::getBounds2D()
{
    double x1, y1, x2, y2;
    auto i = super::numCoords;
    if(i > 0) {
        y1 = y2 = doubleCoords[--i];
        x1 = x2 = doubleCoords[--i];
        while (i > 0) {
            auto y = doubleCoords[--i];
            auto x = doubleCoords[--i];
            if(x < x1)
                x1 = x;

            if(y < y1)
                y1 = y;

            if(x > x2)
                x2 = x;

            if(y > y2)
                y2 = y;

        }
    } else {
        x1 = y1 = x2 = y2 = 0.0;
    }
    return new Rectangle2D_Double(x1, y1, x2 - x1, y2 - y1);
}

PathIterator* Path2D_Double::getPathIterator(AffineTransform* at)
{
    if(at == nullptr) {
        //return new Path2D_Double_CopyIterator(this);
		return nullptr;
    } else {
        //return new Path2D_Double_TxIterator(this, at);
		return nullptr;
    }
}

Path2D* Path2D_Double::clone()
{
    return new Path2D_Double(static_cast< moduru::gui::Shape* >(this));
}
