#include <control/AbstractLaw.hpp>

using namespace ctoot::control;
using namespace std;

AbstractLaw::AbstractLaw(float min, float max, string units) 
{
	this->min = min;
	this->max = max;
	this->units = units;
}

const int AbstractLaw::resolution;

int AbstractLaw::getResolution()
{
    return resolution;
}

float AbstractLaw::getMinimum()
{
    return min;
}

float AbstractLaw::getMaximum()
{
    return max;
}

string AbstractLaw::getUnits()
{
    return units;
}
