#include <gui/geom/Rectangle2D_Float.hpp>

#include <gui/geom/Rectangle2D_Double.hpp>
#include <gui/geom/Rectangle2D.hpp>

using namespace moduru::gui::geom;

Rectangle2D_Float::Rectangle2D_Float()
{
}

Rectangle2D_Float::Rectangle2D_Float(float x, float y, float w, float h)
{
	setRect(x, y, w, h);
}

double Rectangle2D_Float::getX()
{
	return static_cast< double >(x);
}

double Rectangle2D_Float::getY()
{
	return static_cast< double >(y);
}

double Rectangle2D_Float::getWidth()
{
	return static_cast< double >(width);
}

double Rectangle2D_Float::getHeight()
{
	return static_cast< double >(height);
}

bool Rectangle2D_Float::isEmpty()
{
	return (width <= 0.0f) || (height <= 0.0f);
}

void Rectangle2D_Float::setRect(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->width = w;
	this->height = h;
}

void Rectangle2D_Float::setRect(double x, double y, double w, double h)
{
	this->x = static_cast< float >(x);
	this->y = static_cast< float >(y);
	this->width = static_cast< float >(w);
	this->height = static_cast< float >(h);
}

void Rectangle2D_Float::setRect(Rectangle2D* r)
{
	this->x = static_cast< float >(r->getX());
	this->y = static_cast< float >(r->getY());
	this->width = static_cast< float >(r->getWidth());
	this->height = static_cast< float >(r->getHeight());
}

int Rectangle2D_Float::outcode(double x, double y)
{
	int out = 0;
	if (this->width <= 0) {
		out |= OUT_LEFT | OUT_RIGHT;
	}
	else if (x < this->x) {
		out |= OUT_LEFT;
	}
	else if (x > this->x + static_cast<double>(this->width)) {
		out |= OUT_RIGHT;
	}
	if (this->height <= 0) {
		out |= OUT_TOP | OUT_BOTTOM;
	}
	else if (y < this->y) {
		out |= OUT_TOP;
	}
	else if (y > this->y + static_cast<double>(this->height)) {
		out |= OUT_BOTTOM;
	}
	return out;
}

Rectangle2D* Rectangle2D_Float::getBounds2D()
{
	return new Rectangle2D_Float(x, y, width, height);
}

Rectangle2D* Rectangle2D_Float::createIntersection(Rectangle2D* r)
{
	Rectangle2D* dest;
	if (dynamic_cast< Rectangle2D_Float* >(r) != nullptr) {
		dest = new Rectangle2D_Float();
	}
	else {
		dest = new Rectangle2D_Double();
	}
	Rectangle2D::intersect(this, r, dest);
	return dest;
}

Rectangle2D* Rectangle2D_Float::createUnion(Rectangle2D* r)
{
	Rectangle2D* dest;
	if (dynamic_cast< Rectangle2D_Float* >(r) != nullptr) {
		dest = new Rectangle2D_Float();
	}
	else {
		dest = new Rectangle2D_Double();
	}
	Rectangle2D::union_(this, r, dest);
	return dest;
}
