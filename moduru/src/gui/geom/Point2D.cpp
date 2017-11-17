#include <gui/geom/Point2D.hpp>

#include <cmath>

using namespace moduru::gui::geom;

Point2D::Point2D()
{
}

double Point2D::distance(Point2D* pt)
{ 
	double px = pt->getX() - getX();
	double py = pt->getY() - getY();
	return sqrt(px * px + py * py);
}

double Point2D::distance(double px, double py)
{ 
	px -= getX();
	py -= getY();
	return sqrt(px * px + py * py);
}

double Point2D::distance(double x1, double y1, double x2, double y2)
{ 
	x1 -= x2;
	y1 -= y2;
	return sqrt(x1 * x1 + y1 * y1);
}

double Point2D::distanceSq(Point2D* pt)
{ 
	double px = pt->getX() - getX();
	double py = pt->getY() - getY();
	return (px * px + py * py);
}

double Point2D::distanceSq(double px, double py)
{ 
	px -= getX();
	py -= getY();
	return (px * px + py * py);
}

double Point2D::distanceSq(double x1, double y1, double x2, double y2)
{ 
	x1 -= x2;
	y1 -= y2;
	return (x1 * x1 + y1 * y1);
}

void Point2D::setLocation(Point2D* p)
{ 
	setLocation(p->getX(), p->getY());
}
