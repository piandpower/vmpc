#include <gui/geom/CubicIterator.hpp>

#include <gui/geom/AffineTransform.hpp>
#include <gui/geom/CubicCurve2D.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::gui::geom;
using namespace std;

CubicIterator::CubicIterator(CubicCurve2D* q, AffineTransform* at)
{
    this->cubic = q;
    this->affine = at;
}

int CubicIterator::getWindingRule()
{
    return WIND_NON_ZERO;
}

bool CubicIterator::isDone()
{
    return (index > 1);
}

void CubicIterator::next()
{
    index++;
}

int CubicIterator::currentSegment(vector<float>* coords)
{
	if (isDone()) {
		throw std::invalid_argument("cubic iterator iterator out of bounds");
	}
	int type;
	if (index == 0) {
		(*coords)[0] = static_cast<float>(cubic->getX1());
		(*coords)[1] = static_cast<float>(cubic->getY1());
		type = SEG_MOVETO;
	}
	else {
		(*coords)[0] = static_cast<float>(cubic->getCtrlX1());
		(*coords)[1] = static_cast<float>(cubic->getCtrlY1());
		(*coords)[2] = static_cast<float>(cubic->getCtrlX2());
		(*coords)[3] = static_cast<float>(cubic->getCtrlY2());
		(*coords)[4] = static_cast<float>(cubic->getX2());
		(*coords)[5] = static_cast<float>(cubic->getY2());
		type = SEG_CUBICTO;
	}
	if (affine != nullptr) {
		affine->transform(*coords, 0, coords, 0, index == 0 ? 1 : 3);
	}
	return type;
}

int CubicIterator::currentSegment(vector<double>* coords)
{
	if (isDone()) {
		throw std::invalid_argument("cubic iterator iterator out of bounds");
	}
	int type;
	if (index == 0) {
		(*coords)[0] = cubic->getX1();
		(*coords)[1] = cubic->getY1();
		type = SEG_MOVETO;
	}
	else {
		(*coords)[0] = cubic->getCtrlX1();
		(*coords)[1] = cubic->getCtrlY1();
		(*coords)[2] = cubic->getCtrlX2();
		(*coords)[3] = cubic->getCtrlY2();
		(*coords)[4] = cubic->getX2();
		(*coords)[5] = cubic->getY2();
		type = SEG_CUBICTO;
	}
	if (affine != nullptr) {
		affine->transform(*coords, 0, coords, 0, index == 0 ? 1 : 3);
	}
	return type;
}

CubicIterator::~CubicIterator() {
	delete affine;
	affine = nullptr;
	delete cubic;
	cubic = nullptr;
}
