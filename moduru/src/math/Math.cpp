#include <math/Math.hpp>

using namespace moduru::math;

long Math::min(long v1, long v2) {
	return v1 < v2 ? v1 : v2;
}

long Math::max(long v1, long v2) {
	return v1 > v2 ? v1 : v2;
}

float Math::minf(float v1, float v2) {
	return v1 < v2 ? v1 : v2;
}

float Math::maxf(float v1, float v2) {
	return v1 > v2 ? v1 : v2;
}

unsigned int Math::highestOneBit(unsigned int i)
{
	i |= (i >> 1);
	i |= (i >> 2);
	i |= (i >> 4);
	i |= (i >> 8);
	i |= (i >> 16);
	return i - (i >> 1);
}
