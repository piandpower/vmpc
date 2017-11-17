#include <synth/ControlChange.hpp>

using namespace ctoot::synth;

ControlChange::ControlChange(int controller, int value) 
{
	this->controller = controller;
	this->value = value;
}

int ControlChange::getController()
{
    return controller;
}

int ControlChange::getValue()
{
    return value;
}
