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

Led::~Led() {
}
