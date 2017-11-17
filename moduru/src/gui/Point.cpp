#include <gui/Point.hpp>

#include <cmath>

using namespace moduru::gui;

Point::Point() : Point(0, 0)
{
}

Point::Point(Point* p) : Point(p->x, p->y)
{
}

Point::Point(int x, int y)
{
	this->x = x;
	this->y = y;
}

Point* Point::getLocation()
{
	return new Point(x, y);
}

double Point::getX()
{
	return x;
}

double Point::getY()
{
	return y;
}

void Point::move(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Point::setLocation(Point* p)
{
	setLocation(p->x, p->y);
}

void Point::setLocation(int x, int y)
{
	move(x, y);
}

void Point::setLocation(double x, double y)
{
	this->x = (int)floor(x + 0.5);
	this->y = (int)floor(y + 0.5);
}

void Point::translate(int dx, int dy)
{
	this->x += dx;
	this->y += dy;
}
