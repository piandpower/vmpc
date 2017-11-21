#include "Pot.hpp"

using namespace mpc::hardware;
using namespace std;

Pot::Pot(string label)
{
	this->label = label;
}

string Pot::getLabel() {
	return label;
}

Pot::~Pot() {
}
