#include <gui/PolygonPathIterator.hpp>

#include <gui/Polygon.hpp>
#include <gui/geom/AffineTransform.hpp>

using namespace moduru::gui;
using namespace std;

PolygonPathIterator::PolygonPathIterator(Polygon* polygon, moduru::gui::geom::AffineTransform* at) 
{
	poly = polygon;
    transform = at;
    if(poly->npoints == 0) {
        index = 1;
    }
}

int PolygonPathIterator::getWindingRule()
{
    return WIND_EVEN_ODD;
}

bool PolygonPathIterator::isDone()
{
    return index > poly->npoints;
}

void PolygonPathIterator::next()
{
    index++;
}

int PolygonPathIterator::currentSegment(vector<float>* coords)
{
	if (index >= poly->npoints) {
		return SEG_CLOSE;
	}	
	(*coords)[0] = (float) poly->xpoints[index];
	(*coords)[1] = (float) poly->ypoints[index];
	if (transform != nullptr) {
		transform->transform(*coords, 0, coords, 0, 1);
	}
	return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

int PolygonPathIterator::currentSegment(vector<double>* coords)
{
	if (index >= poly->npoints) {
		return SEG_CLOSE;
	}
	(*coords)[0] = poly->xpoints[index];
	(*coords)[1] = poly->ypoints[index];
	if (transform != nullptr) {
		transform->transform(*coords, 0, coords, 0, 1);
	}
	return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

PolygonPathIterator::~PolygonPathIterator() {
	if (poly != nullptr) {
		delete poly;
		poly = nullptr;
	}
	if (transform != nullptr) {
		delete transform;
		transform = nullptr;
	}
}
