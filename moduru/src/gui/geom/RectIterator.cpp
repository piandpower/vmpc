#include <gui/geom/RectIterator.hpp>

#include <gui/geom/AffineTransform.hpp>
#include <gui/geom/Rectangle2D.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::gui::geom;
using namespace std;

RectIterator::RectIterator(Rectangle2D* r, AffineTransform* at)
{
    this->x = r->getX();
    this->y = r->getY();
    this->w = r->getWidth();
    this->h = r->getHeight();
    this->affine = at;
    if(w < 0 || h < 0) {
        index = 6;
    }
}

int RectIterator::getWindingRule()
{
    return WIND_NON_ZERO;
}

bool RectIterator::isDone()
{
    return index > 5;
}

void RectIterator::next()
{
    index++;
}

int RectIterator::currentSegment(vector<float>* coords)
{
    if(isDone()) {
        throw std::invalid_argument("rect iterator out of bounds");
    }
    if(index == 5) {
        return SEG_CLOSE;
    }
    (*coords)[0] = static_cast< float >(x);
    (*coords)[1] = static_cast< float >(y);
    if(index == 1 || index == 2) {
        (*coords)[0] += static_cast< float >(w);
    }
    if(index == 2 || index == 3) {
        (*coords)[1] += static_cast< float >(h);
    }
    if(affine != nullptr) {
        affine->transform(*coords, 0, coords, 0, 1);
    }
    return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

int RectIterator::currentSegment(vector<double>* coords)
{
	if (isDone()) {
		throw std::invalid_argument("rect iterator out of bounds");
	}
	if (index == 5) {
		return SEG_CLOSE;
	}
	(*coords)[0] = x;
	(*coords)[1] = y;
	if (index == 1 || index == 2) {
		(*coords)[0] += w;
	}
	if (index == 2 || index == 3) {
		(*coords)[1] += h;
	}
	if (affine != nullptr) {
		affine->transform(*coords, 0, coords, 0, 1);
	}
	return (index == 0 ? SEG_MOVETO : SEG_LINETO);
}

RectIterator::~RectIterator() {
	delete affine;
	affine = nullptr;
}
