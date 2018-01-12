#include <gui/Polygon.hpp>

#include <gui/Point.hpp>
#include <gui/PolygonPathIterator.hpp>
#include <gui/Rectangle.hpp>
#include <gui/geom/PathIterator.hpp>
#include <gui/geom/Point2D.hpp>
#include <gui/geom/Rectangle2D.hpp>

#include <gui/geom/Crossings_EvenOdd.hpp>

#include <math/Math.hpp>

#include <VecUtil.hpp>

#ifdef __linux__
#include <climits>
#include <stdexcept>
#endif


using namespace moduru::gui;
using namespace std;

Polygon::Polygon()
{
	xpoints = vector<int>(MIN_LENGTH);
	ypoints = vector<int>(MIN_LENGTH);
}

Polygon::Polygon(vector<int> xpoints, vector<int> ypoints, int npoints)
{
	if (npoints > xpoints.size() || npoints > ypoints.size()) {
		throw std::invalid_argument("npoints > xpoints.length || npoints > ypoints.length");
	}
	if (npoints < 0) {
		throw std::invalid_argument("npoints < 0");
	}
	this->npoints = npoints;
	this->xpoints = moduru::VecUtil::CopyOf(xpoints, npoints);
	this->ypoints = moduru::VecUtil::CopyOf(ypoints, npoints);
}

void Polygon::reset()
{
    npoints = 0;
    bounds = nullptr;
}

void Polygon::invalidate()
{
    bounds = nullptr;
}

void Polygon::translate(int deltaX, int deltaY)
{
    for (auto i = 0; i < npoints; i++) {
        xpoints[i] += deltaX;
        ypoints[i] += deltaY;
    }
    if(bounds != nullptr) {
        bounds->translate(deltaX, deltaY);
    }
}

void Polygon::calculateBounds(vector<int> xpoints, vector<int> ypoints, int npoints)
{
    auto boundsMinX = INT_MAX;
    auto boundsMinY = INT_MAX;
    auto boundsMaxX = INT_MIN;
    auto boundsMaxY = INT_MIN;
    for (auto i = 0; i < npoints; i++) {
        auto x = xpoints[i];
        boundsMinX = moduru::math::Math::min(boundsMinX, x);
        boundsMaxX = moduru::math::Math::max(boundsMaxX, x);
        auto y = ypoints[i];
        boundsMinY = moduru::math::Math::min(boundsMinY, y);
        boundsMaxY = moduru::math::Math::max(boundsMaxY, y);
    }
    bounds = new Rectangle(boundsMinX, boundsMinY, boundsMaxX - boundsMinX, boundsMaxY - boundsMinY);
}

void Polygon::updateBounds(int x, int y)
{
    if(x < bounds->x) {
        bounds->width = bounds->width + (bounds->x - x);
        bounds->x = x;
    } else {
        bounds->width = moduru::math::Math::max(bounds->width, x - bounds->x);
    }
    if(y < bounds->y) {
        bounds->height = bounds->height + (bounds->y - y);
        bounds->y = y;
    } else {
        bounds->height = moduru::math::Math::max(bounds->height, y - bounds->y);
    }
}

void Polygon::addPoint(int x, int y)
{
	if (npoints >= xpoints.size() || npoints >= ypoints.size()) {
		auto newLength = npoints * 2;
		if (newLength < MIN_LENGTH) {
			newLength = MIN_LENGTH;
		}
		else if ((newLength & (newLength - 1)) != 0) {
			newLength = moduru::math::Math::highestOneBit(newLength);
		}
		xpoints = moduru::VecUtil::CopyOf(xpoints, newLength);
		ypoints = moduru::VecUtil::CopyOf(ypoints, newLength);
	}
	xpoints[npoints] = x;
	ypoints[npoints] = y;
	npoints++;
	if (bounds != nullptr) {
		updateBounds(x, y);
	}
}

Rectangle* Polygon::getBounds()
{
    return getBoundingBox();
}

Rectangle* Polygon::getBoundingBox()
{
    if(npoints == 0) {
        return new Rectangle();
    }
    if(bounds == nullptr) {
        calculateBounds(xpoints, ypoints, npoints);
    }
    return bounds->getBounds();
}

bool Polygon::contains(Point* p)
{
    return contains(p->x, p->y);
}

bool Polygon::contains(int x, int y)
{
    return contains(static_cast< double >(x), static_cast< double >(y));
}

bool Polygon::inside(int x, int y)
{
    return contains(static_cast< double >(x), static_cast< double >(y));
}

geom::Rectangle2D* Polygon::getBounds2D()
{
    return getBounds();
}

bool Polygon::contains(double x, double y)
{
	if (npoints <= 2 || !getBoundingBox()->contains((int) x, (int) y)) {
		return false;
	}
	auto hits = 0;
	auto lastx = xpoints[npoints - 1];
	auto lasty = ypoints[npoints - 1];
	int curx, cury;
	for (auto i = 0; i < npoints; lastx = curx, lasty = cury, i++) {
		curx = xpoints[i];
		cury = ypoints[i];
		if (cury == lasty) {
			continue;
		}
		int leftx;
		if (curx < lastx) {
			if (x >= lastx) {
				continue;
			}
			leftx = curx;
		}
		else {
			if (x >= curx) {
				continue;
			}
			leftx = lastx;
		}
		double test1, test2;
		if (cury < lasty) {
			if (y < cury || y >= lasty) {
				continue;
			}
			if (x < leftx) {
				hits++;
				continue;
			}
			test1 = x - curx;
			test2 = y - cury;
		}
		else {
			if (y < lasty || y >= cury) {
				continue;
			}
			if (x < leftx) {
				hits++;
				continue;
			}
			test1 = x - lastx;
			test2 = y - lasty;
		}
		if (test1 < (test2 / (lasty - cury) * (lastx - curx))) {
			hits++;
		}
	}
	return ((hits & 1) != 0);
}

moduru::gui::geom::Crossings* Polygon::getCrossings(double xlo, double ylo, double xhi, double yhi)
{
	moduru::gui::geom::Crossings* cross = new moduru::gui::geom::Crossings_EvenOdd(xlo, ylo, xhi, yhi);
	auto lastx = xpoints[npoints - 1];
	auto lasty = ypoints[npoints - 1];
	int curx, cury;
	for (auto i = 0; i < npoints; i++) {
		curx = xpoints[i];
		cury = ypoints[i];
		if (cross->accumulateLine(lastx, lasty, curx, cury)) {
			return nullptr;
		}
		lastx = curx;
		lasty = cury;
	}
	return cross;
}

bool Polygon::contains(::geom::Point2D* p)
{
    return contains(p->getX(), p->getY());
}

bool Polygon::intersects(double x, double y, double w, double h)
{
	if (npoints <= 0 || !dynamic_cast<moduru::gui::geom::Rectangle2D*>(getBounds())->intersects(x, y, w, h)) {
		return false;
	}
	auto cross = getCrossings(x, y, x + w, y + h);
	return (cross == nullptr || !cross->isEmpty());
}

bool Polygon::intersects(::geom::Rectangle2D* r)
{
    return intersects(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

bool Polygon::contains(double x, double y, double w, double h)
{
    if(npoints <= 0 || !dynamic_cast<moduru::gui::geom::Rectangle2D*>(getBounds())->intersects(x, y, w, h)) {
        return false;
    }
    auto cross = getCrossings(x, y, x + w, y + h);
    return (cross != nullptr && cross->covers(y, y + h));
}

bool Polygon::contains(::geom::Rectangle2D* r)
{
	return contains(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

geom::PathIterator* Polygon::getPathIterator(::geom::AffineTransform* at)
{
    return new PolygonPathIterator(this, at);
}

geom::PathIterator* Polygon::getPathIterator(::geom::AffineTransform* at, double flatness)
{
    return getPathIterator(at);
}

Polygon::~Polygon() {
	xpoints.clear();
	ypoints.clear();
	if (bounds != nullptr) {
		delete bounds;
		bounds = nullptr;
	}
}
