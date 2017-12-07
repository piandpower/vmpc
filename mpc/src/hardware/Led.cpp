#include "Led.hpp"

#include <Logger.hpp>

using namespace mpc::hardware;
using namespace std;

Led::Led(string label)
{
	this->label = label;
}

string Led::getLabel() {
	return label;
}

void Led::light(bool b) {
	if (b) {
		notifyObservers(label + "on");
		string obscount = to_string(countObservers());
		MLOG("Led with label " + label + ", observer count: " + obscount);
	}
	else {
		notifyObservers(label + "off");
	}
}

Led::~Led() {
}
