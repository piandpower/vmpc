#include "DataWheel.hpp"

#include <Mpc.hpp>
#include <controls/AbstractControls.hpp>

using namespace mpc::hardware;
using namespace std;

DataWheel::DataWheel(mpc::Mpc* mpc)
{
	this->mpc = mpc;
}

void DataWheel::turn(int increment) {
	mpc->getActiveControls()->turnWheel(increment);
	notifyObservers(increment);
}

DataWheel::~DataWheel() {
}
