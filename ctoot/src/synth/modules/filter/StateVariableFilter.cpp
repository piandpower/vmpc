#include <synth/modules/filter/StateVariableFilter.hpp>
//#include <dsp/FastMath.hpp>
#include <synth/modules/filter/FilterVariables.hpp>
#include <synth/modules/filter/StateVariableFilterElement.hpp>
#include <synth/modules/filter/StateVariableFilterVariables.hpp>

#define _USE_MATH_DEFINES
//#include <cmath>
#include <math.h>

using namespace ctoot::synth::modules::filter;

StateVariableFilter::StateVariableFilter(StateVariableFilterVariables* variables) : AbstractFilter(variables)
{
	element = new StateVariableFilterElement();
}

float StateVariableFilter::update()
{
    res = vars->getResonance();
    element->mix = dynamic_cast< StateVariableFilterVariables* >(vars)->getModeMix();
    element->bp = dynamic_cast< StateVariableFilterVariables* >(vars)->isBandMode();
    return vars->getCutoff();
}

float StateVariableFilter::filter(float sample, float f)
{
	float min = 0.24f < f * 0.25f ? 0.24f : f * 0.25f;
    auto f1 = 2.0f * sin(static_cast< float >(M_PI * min));
	float v1 = 1.9f;
	float v2 = 2.0f / f1 - f1 * 0.5f;
	min = v1 < v2 ? v1 : v2;
	v1 = res;
	v2 = min;
	min = v1 < v2 ? v1 : v2;
    return element->filter(sample, f1, min);
}

StateVariableFilter::~StateVariableFilter() {
	if (element != nullptr) {
		//delete element;
	}
}
