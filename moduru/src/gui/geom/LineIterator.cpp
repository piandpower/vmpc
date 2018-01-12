#include <gui/geom/LineIterator.hpp>

#include <gui/geom/AffineTransform.hpp>
#include <gui/geom/Line2D.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::gui::geom;
using namespace std;

LineIterator::LineIterator(Line2D* l, AffineTransform* at)
{
	this->line = l;
	this->affine = at;
}

int LineIterator::getWindingRule()
{
    return WIND_NON_ZERO;
}

bool LineIterator::isDone()
{
    return (index > 1);
}

void LineIterator::next()
{
    index++;
}

int LineIterator::currentSegment(vector<float>* coords)
{
    if(isDone()) {
        throw std::invalid_argument("line iterator out of bounds");
    }
    int type;
    if(index == 0) {
        (*coords)[0] = static_cast< float >(line->getX1());
        (*coords)[1] = static_cast< float >(line->getY1());
        type = SEG_MOVETO;
    } else {
        (*coords)[0] = static_cast< float >(line->getX2());
        (*coords)[1] = static_cast< float >(line->getY2());
        type = SEG_LINETO;
    }
    if(affine != nullptr) {
        affine->transform(*coords, 0, coords, 0, 1);
    }
    return type;
}

int LineIterator::currentSegment(vector<double>* coords)
{
    if(isDone()) {
        throw std::invalid_argument("line iterator out of bounds");
    }
    int type;
    if(index == 0) {
        (*coords)[0] = line->getX1();
        (*coords)[1] = line->getY1();
        type = SEG_MOVETO;
    } else {
        (*coords)[0] = line->getX2();
        (*coords)[1] = line->getY2();
        type = SEG_LINETO;
    }
    if(affine != nullptr) {
        affine->transform(*coords, 0, coords, 0, 1);
    }
    return type;
}

LineIterator::~LineIterator() {
	delete line;
	line = nullptr;
	delete affine;
	affine = nullptr;
}
