#include <control/LogLaw.hpp>

#include <control/ControlLaw.hpp>

#include <cmath>

using namespace ctoot::control;
using namespace std;

LogLaw::LogLaw(float min, float max, string units) : AbstractLaw(min, max, units)
{
	logMin = log10(min);
	logMax = log10(max);
	logSpan = logMax - logMin;
}

int LogLaw::intValue(float userVal)
{
	if (userVal == 0) userVal = 1;
	return static_cast<int>((resolution - 1) * (log10(userVal) - logMin) / logSpan);
}

float LogLaw::userValue(int intVal)
{
    auto p = logMin + (logSpan * intVal) / (resolution - 1);
    return static_cast< float >(pow(10, p));
}
