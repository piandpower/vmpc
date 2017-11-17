#include <gui/geom/Rectangle2D_Double.hpp>

#include <gui/geom/Rectangle2D.hpp>

using namespace moduru::gui::geom;

Rectangle2D_Double::Rectangle2D_Double()
{
}

Rectangle2D_Double::Rectangle2D_Double(double x, double y, double w, double h)
{
	setRect(x, y, w, h);
}

double Rectangle2D_Double::getX()
{
	return x;
}

double Rectangle2D_Double::getY()
{
	return y;
}

double Rectangle2D_Double::getWidth()
{
	return width;
}

double Rectangle2D_Double::getHeight()
{
	return height;
}

bool Rectangle2D_Double::isEmpty()
{
	return (width <= 0.0) || (height <= 0.0);
}

void Rectangle2D_Double::setRect(double x, double y, double w, double h)
{
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
}

void Rectangle2D_Double::setRect(Rectangle2D* r)
{
	this->x = r->getX();
	this->y = r->getY();
	this->width = r->getWidth();
	this->height = r->getHeight();
}

int Rectangle2D_Double::outcode(double x, double y)
{
	auto out = 0;
	if (this->width <= 0) {
		out |= OUT_LEFT | OUT_RIGHT;
	}
	else if (x < this->x) {
		out |= OUT_LEFT;
	}
	else if (x > this->x + this->width) {
		out |= OUT_RIGHT;
	}
	if (this->height <= 0) {
		out |= OUT_TOP | OUT_BOTTOM;
	}
	else if (y < this->y) {
		out |= OUT_TOP;
	}
	else if (y > this->y + this->height) {
		out |= OUT_BOTTOM;
	}
	return out;
}

Rectangle2D* Rectangle2D_Double::getBounds2D()
{
	return new Rectangle2D_Double(x, y, width, height);
}

Rectangle2D* Rectangle2D_Double::createIntersection(Rectangle2D* r)
{
	Rectangle2D* dest = new Rectangle2D_Double();
	Rectangle2D::intersect(this, r, dest);
	return dest;
}

Rectangle2D* Rectangle2D_Double::createUnion(Rectangle2D* r)
{
	Rectangle2D* dest = new Rectangle2D_Double();
	Rectangle2D::union_(this, r, dest);
	return dest;
}
