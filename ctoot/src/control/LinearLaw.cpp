#include <control/LinearLaw.hpp>

using namespace ctoot::control;
using namespace std;

LinearLaw::LinearLaw(float min, float max, string units)
	: AbstractLaw(min, max, units)
{
}

LinearLaw*& LinearLaw::UNITY()
{
	return UNITY_;
}
LinearLaw* LinearLaw::UNITY_ =  new LinearLaw(0, 1, "");

int LinearLaw::intValue(float v)
{
    return static_cast< int >((resolution - 1) * (v - min) / (max - min));
}

float LinearLaw::userValue(int v)
{
    return min + (max - min) * (static_cast< float >(v) / (resolution - 1));
}
