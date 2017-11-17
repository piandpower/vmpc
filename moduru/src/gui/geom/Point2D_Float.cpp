#include <gui/geom/Point2D_Float.hpp>

using namespace moduru::gui::geom;

Point2D_Float::Point2D_Float()
{
}

Point2D_Float::Point2D_Float(float x, float y)
{
	this->x = x;
	this->y = y;
}

double Point2D_Float::getX()
{
	return (double)x;
}

double Point2D_Float::getY()
{
	return (double)y;
}

void Point2D_Float::setLocation(double x, double y)
{
	this->x = (float)x;
	this->y = (float)y;
}

void Point2D_Float::setLocation(float x, float y)
{
	this->x = x;
	this->y = y;
}
