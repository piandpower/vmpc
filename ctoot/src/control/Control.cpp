#include <control/Control.hpp>
#include <control/CompoundControl.hpp>
#include <control/BooleanControl.hpp>
#include <control/ChainMutation.hpp>

using namespace ctoot::control;
using namespace std;

Control::Control(int id, string name) 
{
	this->name = name;
	this->id = id;
	annotation = name;
}

void Control::setHidden(bool h)
{
    hidden = h;
}

int Control::getId()
{
    return id;
}


CompoundControl* Control::getParent()
{
    return parent;
}

void Control::setParent(CompoundControl* c)
{
	parent = c;
}

void Control::notifyParent(Control* c)
{

	derive(c);
	setChanged();
	notifyObservers(c);
	if (c->isIndicator()) return;

	if (parent != nullptr) {
		parent->notifyParent(c);
	}
}

void Control::derive(Control* obj)
{
}

string Control::getName()
{
    return name;
}

void Control::setName(string s)
{
    name = s;
    annotation = s;
}

string Control::getAnnotation()
{
    return annotation;
}

void Control::setAnnotation(string a)
{
    annotation = a;
}

void Control::setIntValue(int value)
{
	string description = "ERROR: Unexpected setIntValue(" + to_string(value) + ") called on " + getControlPath();
	printf("%s", description.c_str());
}

int Control::getIntValue()
{
    return -1;
}

string Control::getValueString()
{
	return string("");
}

bool Control::isHidden()
{
    return hidden;
}

bool Control::isIndicator()
{
    return indicator;
}

bool Control::isAdjusting()
{
    return adjusting;
}

void Control::setAdjusting(bool state)
{
    adjusting = state;
    notifyParent(this);
}

void Control::setEnabled(bool enable)
{
    enabled = enable;
}

bool Control::isEnabled()
{
    return enabled;
}

string Control::toString()
{
	string result = getName() + " Control";
	return result;
}

string Control::getControlPath()
{
	return getControlPath(nullptr, "/");
}

string Control::getControlPath(Control* from, string sep)
{
	if (sizeof(getName()) > 0) {
		string result = parent->getControlPath(from, sep) + sep + getName();
		return result;
	}
	else {
		return parent->getControlPath(from, sep);
	}
	return getName();
}

Control::~Control() {
}
