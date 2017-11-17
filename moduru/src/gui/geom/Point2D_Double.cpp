#include <gui/geom/Point2D_Double.hpp>

using namespace moduru::gui::geom;

Point2D_Double::Point2D_Double()
{
}

Point2D_Double::Point2D_Double(double x, double y)
{
	this->x = x;
	this->y = y;
}

double Point2D_Double::getX()
{ 
	return x;
}

double Point2D_Double::getY()
{
	return y;
}

void Point2D_Double::setLocation(double x, double y)
{ 
	this->x = x;
	this->y = y;
}
