#include <control/LawControl.hpp>
#include <control/ControlLaw.hpp>

using namespace ctoot::control;
using namespace std;

LawControl::LawControl(int id, string name, ControlLaw law, float precision, float initialValue) : Control(id, name)
{
	this->law = &law;
	this->precision = precision;
	this->value = initialValue;
}

int LawControl::calculateDecimalPlaces()
{
    return law->getMaximum() - law->getMinimum() > 100.0f ? 1 : 2;
}

ControlLaw* LawControl::getLaw()
{
    return law;
}

float LawControl::getValue()
{
    return value;
}

string LawControl::getValueString()
{
	string res1 = getLaw()->getUnits();
	string res2 = to_string(getValue());
	return string(res1 + res2);
}

void LawControl::setValue(float value)
{
	if (!isEnabled()) return;
	this->value = value;
	notifyParent(this);
}

float LawControl::getPrecision()
{
    return precision;
}

void LawControl::setIntValue(int value)
{
    setValue(getLaw()->userValue(value));
}

int LawControl::getIntValue()
{
    return getLaw()->intValue(getValue());
}

vector<string>* LawControl::getPresetNames()
{
	auto res = new vector < string >({ "foo" });
	return res;
}

void LawControl::applyPreset(string name)
{
}

LawControl::~LawControl() {
}
