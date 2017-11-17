#include <gui/geom/Rectangle2D.hpp>

#include <gui/geom/Line2D.hpp>
#include <gui/geom/Point2D.hpp>
#include <gui/geom/RectIterator.hpp>

#include <cmath>

#include <math/Math.hpp>

using namespace moduru::gui::geom;

Rectangle2D::Rectangle2D()
{
}

const int Rectangle2D::OUT_LEFT;
const int Rectangle2D::OUT_TOP;
const int Rectangle2D::OUT_RIGHT;
const int Rectangle2D::OUT_BOTTOM;

void Rectangle2D::setRect(Rectangle2D* r)
{
	setRect(r->getX(), r->getY(), r->getWidth(), r->getHeight());
}

bool Rectangle2D::intersectsLine(double x1, double y1, double x2, double y2)
{
	int out1, out2;
	if ((out2 = outcode(x2, y2)) == 0) {
		return true;
	}
	while ((out1 = outcode(x1, y1)) != 0) {
		if ((out1 & out2) != 0) {
			return false;
		}
		if ((out1 & (OUT_LEFT | OUT_RIGHT)) != 0) {
			auto x = getX();
			if ((out1 & OUT_RIGHT) != 0) {
				x += getWidth();
			}
			y1 = y1 + (x - x1) * (y2 - y1) / (x2 - x1);
			x1 = x;
		}
		else {
			auto y = getY();
			if ((out1 & OUT_BOTTOM) != 0) {
				y += getHeight();
			}
			x1 = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
			y1 = y;
		}
	}
	return true;
}

bool Rectangle2D::intersectsLine(Line2D* l)
{
	return intersectsLine(l->getX1(), l->getY1(), l->getX2(), l->getY2());
}

int Rectangle2D::outcode(Point2D* p)
{
	return outcode(p->getX(), p->getY());
}

void Rectangle2D::setFrame(double x, double y, double w, double h)
{
	setRect(x, y, w, h);
}

Rectangle2D* Rectangle2D::getBounds2D()
{
	return dynamic_cast<Rectangle2D*>(clone());
}

bool Rectangle2D::contains(double x, double y)
{
	auto x0 = getX();
	auto y0 = getY();
	return (x >= x0 && y >= y0 && x < x0 + getWidth() && y < y0 + getHeight());
}

bool Rectangle2D::intersects(double x, double y, double w, double h)
{
	if (isEmpty() || w <= 0 || h <= 0) {
		return false;
	}
	auto x0 = getX();
	auto y0 = getY();
	return (x + w > x0 && y + h > y0 && x < x0 + getWidth() && y < y0 + getHeight());
}

bool Rectangle2D::contains(double x, double y, double w, double h)
{
	if (isEmpty() || w <= 0 || h <= 0) {
		return false;
	}
	auto x0 = getX();
	auto y0 = getY();
	return (x >= x0 && y >= y0 && (x + w) <= x0 + getWidth() && (y + h) <= y0 + getHeight());
}

void Rectangle2D::intersect(Rectangle2D* src1, Rectangle2D* src2, Rectangle2D* dest)
{
	auto x1 = moduru::math::Math::max(src1->getMinX(), src2->getMinX());
	auto y1 = moduru::math::Math::max(src1->getMinY(), src2->getMinY());
	auto x2 = moduru::math::Math::min(src1->getMaxX(), src2->getMaxX());
	auto y2 = moduru::math::Math::min(src1->getMaxY(), src2->getMaxY());
	dest->setFrame(x1, y1, x2 - x1, y2 - y1);
}

void Rectangle2D::union_(Rectangle2D* src1, Rectangle2D* src2, Rectangle2D* dest)
{
	auto x1 = moduru::math::Math::min(src1->getMinX(), src2->getMinX());
	auto y1 = moduru::math::Math::min(src1->getMinY(), src2->getMinY());
	auto x2 = moduru::math::Math::max(src1->getMaxX(), src2->getMaxX());
	auto y2 = moduru::math::Math::max(src1->getMaxY(), src2->getMaxY());
	dest->setFrameFromDiagonal(x1, y1, x2, y2);
}

void Rectangle2D::add(double newx, double newy)
{
	auto x1 = moduru::math::Math::min(getMinX(), newx);
	auto x2 = moduru::math::Math::max(getMaxX(), newx);
	auto y1 = moduru::math::Math::min(getMinY(), newy);
	auto y2 = moduru::math::Math::max(getMaxY(), newy);
	setRect(x1, y1, x2 - x1, y2 - y1);
}

void Rectangle2D::add(Point2D* pt)
{
	add(pt->getX(), pt->getY());
}

void Rectangle2D::add(Rectangle2D* r)
{
	auto x1 = moduru::math::Math::min(getMinX(), r->getMinX());
	auto x2 = moduru::math::Math::max(getMaxX(), r->getMaxX());
	auto y1 = moduru::math::Math::min(getMinY(), r->getMinY());
	auto y2 = moduru::math::Math::max(getMaxY(), r->getMaxY());
	setRect(x1, y1, x2 - x1, y2 - y1);
}

PathIterator* Rectangle2D::getPathIterator(AffineTransform* at)
{
	return new RectIterator(this, at);
}

PathIterator* Rectangle2D::getPathIterator(AffineTransform* at, double flatness)
{
	return new RectIterator(this, at);
}

/*
int Rectangle2D::hashCode()
{
	auto bits = ::Double::doubleToLongBits(getX());
	bits += ::Double::doubleToLongBits(getY()) * int32_t(37);
	bits += ::Double::doubleToLongBits(getWidth()) * int32_t(43);
	bits += ::Double::doubleToLongBits(getHeight()) * int32_t(47);
	return ((static_cast< int >(bits)) ^ (static_cast< int >((bits >> int32_t(32)))));
}

bool Rectangle2D::equals(moduru::lang::Object* obj)
{
	if (obj == static_cast< moduru::lang::Object* >(this)) {
		return true;
	}
	if (dynamic_cast< Rectangle2D* >(obj) != nullptr) {
		auto r2d = dynamic_cast< Rectangle2D* >(obj);
		return ((getX() == r2d->getX()) && (getY() == r2d->getY()) && (getWidth() == r2d->getWidth()) && (getHeight() == r2d->getHeight()));
	}
	return false;
}
*/