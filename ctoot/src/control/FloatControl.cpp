#include <control/FloatControl.hpp>
#include <control/ControlLaw.hpp>

using namespace ctoot::control;
using namespace std;

FloatControl::FloatControl(int id, string name, ControlLaw law, float precision, float initialValue, string minLabel, string midLabel, string maxLabel) : LawControl(id, name, law, precision, initialValue)
{
	this->minLabel = minLabel;
	this->midLabel = midLabel;
	this->maxLabel = maxLabel;
}

bool FloatControl::isRotary()
{
    return true;
}

float FloatControl::getMaximum()
{
    return getLaw()->getMaximum();
}

float FloatControl::getMinimum()
{
    return getLaw()->getMinimum();
}

string FloatControl::getMinLabel()
{
    return minLabel;
}

string FloatControl::getMidLabel()
{
    return midLabel;
}

string FloatControl::getMaxLabel()
{
    return maxLabel;
}

string FloatControl::toString()
{
//    return stringBuilder().append(getName())->append(u" with current value: "_j)
//        ->append(getValue())
//        ->append(u" "_j)
//        ->append(getLaw()->getUnits())
//        ->append(u" (range: "_j)
//        ->append(getMinimum())
//        ->append(u" - "_j)
//        ->append(getMaximum())
//        ->append(u")"_j)->toString();
	return "fix the method izzy!";
}

FloatControl::~FloatControl() {
}
