#include <gui/Rectangle.hpp>

#include <gui/Dimension.hpp>
//#include <gui/GraphicsEnvironment.hpp>
#include <gui/Point.hpp>
//#include <gui/Toolkit.hpp>
#include <gui/geom/Rectangle2D_Double.hpp>
#include <gui/geom/Rectangle2D.hpp>

#include <cmath>

#ifdef __APPLE__
#include <cstdlib>
#include <limits.h>
#endif

#ifdef __linux__
#include <climits>
#endif

using namespace moduru::gui;

Rectangle::Rectangle() : Rectangle(0, 0, 0, 0)
{
}

Rectangle::Rectangle(Rectangle* r) : Rectangle(r->x, r->y, r->width, r->height)
{
}

Rectangle::Rectangle(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

Rectangle::Rectangle(int width, int height) : Rectangle(0, 0, width, height)
{
}

Rectangle::Rectangle(Point* p, Dimension* d) : Rectangle(p->x, p->y, d->width, d->height)
{
}

Rectangle::Rectangle(Point* p) : Rectangle(p->x, p->y, 0, 0)
{
}

Rectangle::Rectangle(Dimension* d) : Rectangle(0, 0, d->width, d->height)
{
}

double Rectangle::getX()
{
	return x;
}

double Rectangle::getY()
{
	return y;
}

double Rectangle::getWidth()
{
	return width;
}

double Rectangle::getHeight()
{
	return height;
}

Rectangle* Rectangle::getBounds()
{
	return new Rectangle(x, y, width, height);
}

moduru::gui::geom::Rectangle2D* Rectangle::getBounds2D()
{
	return new Rectangle(x, y, width, height);
}

void Rectangle::setBounds(Rectangle* r)
{
	setBounds(r->x, r->y, r->width, r->height);
}

void Rectangle::setBounds(int x, int y, int width, int height)
{
	reshape(x, y, width, height);
}

void Rectangle::setRect(double x, double y, double width, double height)
{
	int newx, newy, neww, newh;
	if (x > 2.0 * INT_MAX) {
		newx = INT_MAX;
		neww = -1;
	}
	else {
		newx = clip(x, false);
		if (width >= 0)
			width += x - newx;

		neww = clip(width, width >= 0);
	}
	if (y > 2.0 * INT_MAX) {
		newy = INT_MAX;
		newh = -1;
	}
	else {
		newy = clip(y, false);
		if (height >= 0)
			height += y - newy;

		newh = clip(height, height >= 0);
	}
	reshape(newx, newy, neww, newh);
}

int Rectangle::clip(double v, bool doceil)
{
	if (v <= INT_MIN) {
		return INT_MIN;
	}
	if (v >= INT_MAX) {
		return INT_MAX;
	}
	return static_cast< int >((doceil ? ceil(v) : floor(v)));
}

void Rectangle::reshape(int x, int y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

moduru::gui::Point* Rectangle::getLocation()
{
	return new Point(x, y);
}

void Rectangle::setLocation(Point* p)
{
	setLocation(p->x, p->y);
}

void Rectangle::setLocation(int x, int y)
{
	move(x, y);
}

void Rectangle::move(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Rectangle::translate(int dx, int dy)
{
	auto oldv = this->x;
	auto newv = oldv + dx;
	if (dx < 0) {
		if (newv > oldv) {
			if (width >= 0) {
				width += newv - INT_MIN;
			}
			newv = INT_MIN;
		}
	}
	else {
		if (newv < oldv) {
			if (width >= 0) {
				width += newv - INT_MAX;
				if (width < 0)
					width = INT_MAX;

			}
			newv = INT_MAX;
		}
	}
	this->x = newv;
	oldv = this->y;
	newv = oldv + dy;
	if (dy < 0) {
		if (newv > oldv) {
			if (height >= 0) {
				height += newv - INT_MIN;
			}
			newv = INT_MIN;
		}
	}
	else {
		if (newv < oldv) {
			if (height >= 0) {
				height += newv - INT_MAX;
				if (height < 0)
					height = INT_MAX;

			}
			newv = INT_MAX;
		}
	}
	this->y = newv;
}

moduru::gui::Dimension* Rectangle::getSize()
{
	return new Dimension(width, height);
}

void Rectangle::setSize(Dimension* d)
{
	setSize(d->width, d->height);
}

void Rectangle::setSize(int width, int height)
{
	resize(width, height);
}

void Rectangle::resize(int width, int height)
{
	this->width = width;
	this->height = height;
}

bool Rectangle::contains(Point* p)
{
	return contains(p->x, p->y);
}

bool Rectangle::contains(int x, int y)
{
	return inside(x, y);
}

bool Rectangle::contains(Rectangle* r)
{
	return contains(r->x, r->y, r->width, r->height);
}

bool Rectangle::contains(int X, int Y, int W, int H)
{
	auto w = this->width;
	auto h = this->height;
	if ((w | h | W | H) < 0) {
		return false;
	}
	auto x = this->x;
	auto y = this->y;
	if (X < x || Y < y) {
		return false;
	}
	w += x;
	W += X;
	if (W <= X) {
		if (w >= x || W > w)
			return false;

	}
	else {
		if (w >= x && W > w)
			return false;

	}
	h += y;
	H += Y;
	if (H <= Y) {
		if (h >= y || H > h)
			return false;

	}
	else {
		if (h >= y && H > h)
			return false;

	}
	return true;
}

bool Rectangle::inside(int X, int Y)
{
	auto w = this->width;
	auto h = this->height;
	if ((w | h) < 0) {
		return false;
	}
	auto x = this->x;
	auto y = this->y;
	if (X < x || Y < y) {
		return false;
	}
	w += x;
	h += y;
	return ((w < x || w > X) && (h < y || h > Y));
}

bool Rectangle::intersects(Rectangle* r)
{
	auto tw = this->width;
	auto th = this->height;
	auto rw = r->width;
	auto rh = r->height;
	if (rw <= 0 || rh <= 0 || tw <= 0 || th <= 0) {
		return false;
	}
	auto tx = this->x;
	auto ty = this->y;
	auto rx = r->x;
	auto ry = r->y;
	rw += rx;
	rh += ry;
	tw += tx;
	th += ty;
	return ((rw < rx || rw > tx) && (rh < ry || rh > ty) && (tw < tx || tw > rx) && (th < ty || th > ry));
}

Rectangle* Rectangle::intersection(Rectangle* r)
{
	auto tx1 = this->x;
	auto ty1 = this->y;
	auto rx1 = r->x;
	auto ry1 = r->y;
	int tx2 = tx1;
	tx2 += this->width;
	int ty2 = ty1;
	ty2 += this->height;
	int rx2 = rx1;
	rx2 += r->width;
	int ry2 = ry1;
	ry2 += r->height;
	if (tx1 < rx1)
		tx1 = rx1;

	if (ty1 < ry1)
		ty1 = ry1;

	if (tx2 > rx2)
		tx2 = rx2;

	if (ty2 > ry2)
		ty2 = ry2;

	tx2 -= tx1;
	ty2 -= ty1;
	if (tx2 < INT_MIN)
		tx2 = INT_MIN;

	if (ty2 < INT_MIN)
		ty2 = INT_MIN;

	return new Rectangle(tx1, ty1, static_cast< int >(tx2), static_cast< int >(ty2));
}

Rectangle* Rectangle::union_(Rectangle* r)
{
	int tx2 = this->width;
	int ty2 = this->height;
	if ((tx2 | ty2) < 0) {
		return new Rectangle(r);
	}
	int rx2 = r->width;
	int ry2 = r->height;
	if ((rx2 | ry2) < 0) {
		return new Rectangle(this);
	}
	auto tx1 = this->x;
	auto ty1 = this->y;
	tx2 += tx1;
	ty2 += ty1;
	auto rx1 = r->x;
	auto ry1 = r->y;
	rx2 += rx1;
	ry2 += ry1;
	if (tx1 > rx1)
		tx1 = rx1;

	if (ty1 > ry1)
		ty1 = ry1;

	if (tx2 < rx2)
		tx2 = rx2;

	if (ty2 < ry2)
		ty2 = ry2;

	tx2 -= tx1;
	ty2 -= ty1;
	if (tx2 > INT_MAX)
		tx2 = INT_MAX;

	if (ty2 > INT_MAX)
		ty2 = INT_MAX;

	return new Rectangle(tx1, ty1, static_cast< int >(tx2), static_cast< int >(ty2));
}

void Rectangle::add(int newx, int newy)
{
	if ((width | height) < 0) {
		this->x = newx;
		this->y = newy;
		this->width = this->height = 0;
		return;
	}
	auto x1 = this->x;
	auto y1 = this->y;
	int x2 = this->width;
	int y2 = this->height;
	x2 += x1;
	y2 += y1;
	if (x1 > newx)
		x1 = newx;

	if (y1 > newy)
		y1 = newy;

	if (x2 < newx)
		x2 = newx;

	if (y2 < newy)
		y2 = newy;

	x2 -= x1;
	y2 -= y1;
	if (x2 > INT_MAX)
		x2 = INT_MAX;

	if (y2 > INT_MAX)
		y2 = INT_MAX;

	reshape(x1, y1, static_cast< int >(x2), static_cast< int >(y2));
}

void Rectangle::add(Point* pt)
{
	add(pt->x, pt->y);
}

void Rectangle::add(Rectangle* r)
{
	int tx2 = this->width;
	int ty2 = this->height;
	if ((tx2 | ty2) < 0) {
		reshape(r->x, r->y, r->width, r->height);
	}
	int rx2 = r->width;
	int ry2 = r->height;
	if ((rx2 | ry2) < 0) {
		return;
	}
	auto tx1 = this->x;
	auto ty1 = this->y;
	tx2 += tx1;
	ty2 += ty1;
	auto rx1 = r->x;
	auto ry1 = r->y;
	rx2 += rx1;
	ry2 += ry1;
	if (tx1 > rx1)
		tx1 = rx1;

	if (ty1 > ry1)
		ty1 = ry1;

	if (tx2 < rx2)
		tx2 = rx2;

	if (ty2 < ry2)
		ty2 = ry2;

	tx2 -= tx1;
	ty2 -= ty1;
	if (tx2 > INT_MAX)
		tx2 = INT_MAX;

	if (ty2 > INT_MAX)
		ty2 = INT_MAX;

	reshape(tx1, ty1, static_cast< int >(tx2), static_cast< int >(ty2));
}

void Rectangle::grow(int h, int v)
{
	int x0 = this->x;
	int y0 = this->y;
	int x1 = this->width;
	int y1 = this->height;
	x1 += x0;
	y1 += y0;
	x0 -= h;
	y0 -= v;
	x1 += h;
	y1 += v;
	if (x1 < x0) {
		x1 -= x0;
		if (x1 < INT_MIN)
			x1 = INT_MIN;

		if (x0 < INT_MIN)
			x0 = INT_MIN;
		else if (x0 > INT_MAX)
			x0 = INT_MAX;

	}
	else {
		if (x0 < INT_MIN)
			x0 = INT_MIN;
		else if (x0 > INT_MAX)
			x0 = INT_MAX;

		x1 -= x0;
		if (x1 < INT_MIN)
			x1 = INT_MIN;
		else if (x1 > INT_MAX)
			x1 = INT_MAX;

	}
	if (y1 < y0) {
		y1 -= y0;
		if (y1 < INT_MIN)
			y1 = INT_MIN;

		if (y0 < INT_MIN)
			y0 = INT_MIN;
		else if (y0 > INT_MAX)
			y0 = INT_MAX;

	}
	else {
		if (y0 < INT_MIN)
			y0 = INT_MIN;
		else if (y0 > INT_MAX)
			y0 = INT_MAX;

		y1 -= y0;
		if (y1 < INT_MIN)
			y1 = INT_MIN;
		else if (y1 > INT_MAX)
			y1 = INT_MAX;

	}
	reshape(static_cast< int >(x0), static_cast< int >(y0), static_cast< int >(x1), static_cast< int >(y1));
}

bool Rectangle::isEmpty()
{
	return (width <= 0) || (height <= 0);
}

int Rectangle::outcode(double x, double y)
{
	auto out = 0;
	if (this->width <= 0) {
		out |= OUT_LEFT | OUT_RIGHT;
	}
	else if (x < this->x) {
		out |= OUT_LEFT;
	}
	else if (x > this->x + static_cast< double >(this->width)) {
		out |= OUT_RIGHT;
	}
	if (this->height <= 0) {
		out |= OUT_TOP | OUT_BOTTOM;
	}
	else if (y < this->y) {
		out |= OUT_TOP;
	}
	else if (y > this->y + static_cast< double >(this->height)) {
		out |= OUT_BOTTOM;
	}
	return out;
}

moduru::gui::geom::Rectangle2D* Rectangle::createIntersection(moduru::gui::geom::Rectangle2D* r)
{
	if (dynamic_cast< Rectangle* >(r) != nullptr) {
		return intersection(dynamic_cast< Rectangle* >(r));
	}
	moduru::gui::geom::Rectangle2D* dest = new moduru::gui::geom::Rectangle2D_Double();
	moduru::gui::geom::Rectangle2D::intersect(this, r, dest);
	return dest;
}

moduru::gui::geom::Rectangle2D* Rectangle::createUnion(moduru::gui::geom::Rectangle2D* r)
{
	if (dynamic_cast< Rectangle* >(r) != nullptr) {
		return union_(dynamic_cast< Rectangle* >(r));
	}
	moduru::gui::geom::Rectangle2D* dest = new moduru::gui::geom::Rectangle2D_Double();
	moduru::gui::geom::Rectangle2D::union_(this, r, dest);
	return dest;
}

/*

bool Rectangle::equals(moduru::lang::Object* obj)
{
	if (dynamic_cast< Rectangle* >(obj) != nullptr) {
		auto r = dynamic_cast< Rectangle* >(obj);
		return ((x == r->x) && (y == r->y) && (width == r->width) && (height == r->height));
	}
	return super::equals(obj);
}

String* Rectangle::toString()
{
	return ::StringBuilder().append(npc(getClass())->getName())->append(u"[x="_j)
		->append(x)
		->append(u",y="_j)
		->append(y)
		->append(u",width="_j)
		->append(width)
		->append(u",height="_j)
		->append(height)
		->append(u"]"_j)->toString();
}
*/
