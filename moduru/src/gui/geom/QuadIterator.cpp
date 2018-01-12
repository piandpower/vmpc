#include <gui/geom/QuadIterator.hpp>

#include <gui/geom/AffineTransform.hpp>
#include <gui/geom/QuadCurve2D.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::gui::geom;
using namespace std;

QuadIterator::QuadIterator(QuadCurve2D* q, AffineTransform* at)
{
    this->quad = q;
    this->affine = at;
}

int QuadIterator::getWindingRule()
{
    return WIND_NON_ZERO;
}

bool QuadIterator::isDone()
{
    return (index > 1);
}

void QuadIterator::next()
{
    index++;
}

int QuadIterator::currentSegment(vector<float>* coords)
{
    if(isDone()) {
        throw std::invalid_argument("quad iterator iterator out of bounds");
    }
    int type;
    if(index == 0) {
        (*coords)[0] = static_cast< float >(quad->getX1());
        (*coords)[1] = static_cast< float >(quad->getY1());
        type = SEG_MOVETO;
    } else {
        (*coords)[0] = static_cast< float >(quad->getCtrlX());
        (*coords)[1] = static_cast< float >(quad->getCtrlY());
        (*coords)[2] = static_cast< float >(quad->getX2());
        (*coords)[3] = static_cast< float >(quad->getY2());
        type = SEG_QUADTO;
    }
    if(affine != nullptr) {
        affine->transform(*coords, 0, coords, 0, index == 0 ? 1 : 2);
    }
    return type;
}

int QuadIterator::currentSegment(vector<double>* coords)
{
	if (isDone()) {
		throw std::invalid_argument("quad iterator iterator out of bounds");
	}
	int type;
	if (index == 0) {
		(*coords)[0] = quad->getX1();
		(*coords)[1] = quad->getY1();
		type = SEG_MOVETO;
	}
	else {
		(*coords)[0] = quad->getCtrlX();
		(*coords)[1] = quad->getCtrlY();
		(*coords)[2] = quad->getX2();
		(*coords)[3] = quad->getY2();
		type = SEG_QUADTO;
	}
	if (affine != nullptr) {
		affine->transform(*coords, 0, coords, 0, index == 0 ? 1 : 2);
	}
	return type;
}

QuadIterator::~QuadIterator() {
	delete quad;
	quad = nullptr;
	delete affine;
	affine = nullptr;
}
