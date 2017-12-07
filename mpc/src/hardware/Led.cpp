#include "Led.hpp"

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
	}
	else {
		notifyObservers(label + "off");
	}
}

Led::~Led() {
}
