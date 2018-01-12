#include <gui/geom/FlatteningPathIterator.hpp>

#include <gui/geom/CubicCurve2D.hpp>
#include <gui/geom/PathIterator.hpp>
#include <gui/geom/QuadCurve2D.hpp>

#include <VecUtil.hpp>

#include <cmath>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::gui::geom;
using namespace std;

FlatteningPathIterator::FlatteningPathIterator(PathIterator* src, double flatness)
{
}

FlatteningPathIterator::FlatteningPathIterator(PathIterator* src, double flatness, int limit)
{
	if (flatness < 0.0) {
		throw std::invalid_argument("flatness must be >= 0");
	}
	if (limit < 0) {
		throw std::invalid_argument("limit must be >= 0");
	}
	this->src = src;
	this->squareflat = flatness * flatness;
	this->limit = limit;
	this->levels = vector<int>(limit + 1);
	next(false);
}

constexpr int FlatteningPathIterator::GROW_SIZE;


double FlatteningPathIterator::getFlatness()
{
    return sqrt(squareflat);
}

int FlatteningPathIterator::getRecursionLimit()
{
    return limit;
}

int FlatteningPathIterator::getWindingRule()
{
    return src->getWindingRule();
}

bool FlatteningPathIterator::isDone()
{
    return done;
}

void FlatteningPathIterator::ensureHoldCapacity(int want)
{
	if (holdIndex - want < 0) {
		auto have = hold.size() - holdIndex;
		auto newsize = hold.size() + GROW_SIZE;
		auto newhold = vector<double>(newsize);
		moduru::VecUtil::VecCopy(&hold, holdIndex, &newhold, holdIndex + GROW_SIZE, have);
		hold = newhold;
		holdIndex += GROW_SIZE;
		holdEnd += GROW_SIZE;
	}
}

void FlatteningPathIterator::next()
{
    next(true);
}

void FlatteningPathIterator::next(bool doNext)
{
    int level;
	if (holdIndex >= holdEnd) {
		if (doNext) {
			src->next();
		}
		if (src->isDone()) {
			done = true;
			return;
		}
		holdType = src->currentSegment(&hold);
		levelIndex = 0;
		levels[0] = 0;
	}
    switch (holdType) {
    case SEG_MOVETO:
    case SEG_LINETO:
        curx = hold[0];
        cury = hold[1];
        if(holdType == SEG_MOVETO) {
            movx = curx;
            movy = cury;
        }
        holdIndex = 0;
        holdEnd = 0;
        break;
    case SEG_CLOSE:
        curx = movx;
        cury = movy;
        holdIndex = 0;
        holdEnd = 0;
        break;
    case SEG_QUADTO:
        if(holdIndex >= holdEnd) {
            holdIndex = hold.size() - 6;
            holdEnd = hold.size() - 2;
            hold[holdIndex + 0] = curx;
            hold[holdIndex + 1] = cury;
            hold[holdIndex + 2] = hold[0];
            hold[holdIndex + 3] = hold[1];
            hold[holdIndex + 4] = curx = hold[2];
            hold[holdIndex + 5] = cury = hold[3];
        }
        level = levels[levelIndex];
        while (level < limit) {
            if(QuadCurve2D::getFlatnessSq(&hold, holdIndex) < squareflat) {
                break;
            }
            ensureHoldCapacity(4);
            QuadCurve2D::subdivide(&hold, holdIndex, &hold, holdIndex - 4, &hold, holdIndex);
            holdIndex -= 4;
            level++;
            levels[levelIndex] = level;
            levelIndex++;
            levels[levelIndex] = level;
        }
        holdIndex += 4;
        levelIndex--;
        break;
    case SEG_CUBICTO:
        if(holdIndex >= holdEnd) {
            holdIndex = hold.size() - 8;
            holdEnd = hold.size() - 2;
            hold[holdIndex + 0] = curx;
            hold[holdIndex + 1] = cury;
            hold[holdIndex + 2] = hold[0];
            hold[holdIndex + 3] = hold[1];
            hold[holdIndex + 4] = hold[2];
            hold[holdIndex + 5] = hold[3];
            hold[holdIndex + 6] = curx = hold[4];
            hold[holdIndex + 7] = cury = hold[5];
        }
        level = levels[levelIndex];
        while (level < limit) {
            if(CubicCurve2D::getFlatnessSq(&hold, holdIndex) < squareflat) {
               break;
            }
            ensureHoldCapacity(6);
            CubicCurve2D::subdivide(&hold, holdIndex, &hold, holdIndex - 6, &hold, holdIndex);
            holdIndex -= 6;
            level++;
            levels[levelIndex] = level;
            levelIndex++;
            levels[levelIndex] = level;
        }
        holdIndex += 6;
        levelIndex--;
        break;
    }

}

int FlatteningPathIterator::currentSegment(vector<float>* coords)
{
    if(isDone()) {
        throw std::invalid_argument("flattening iterator out of bounds");
    }
    auto type = holdType;
    if(type != SEG_CLOSE) {
        (*coords)[0] = static_cast< float >(hold[holdIndex + 0]);
        (*coords)[1] = static_cast< float >(hold[holdIndex + 1]);
        if(type != SEG_MOVETO) {
            type = SEG_LINETO;
        }
    }
    return type;
}

int FlatteningPathIterator::currentSegment(vector<double>* coords)
{
	if (isDone()) {
		throw std::invalid_argument("flattening iterator out of bounds");
	}
	auto type = holdType;
	if (type != SEG_CLOSE) {
		(*coords)[0] = hold[holdIndex + 0];
		(*coords)[1] = hold[holdIndex + 1];
		if (type != SEG_MOVETO) {
			type = SEG_LINETO;
		}
	}
	return type;
}

FlatteningPathIterator::~FlatteningPathIterator() {
	delete src;
	src = nullptr;
}
