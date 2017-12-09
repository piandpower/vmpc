#include "HwSlider.hpp"

#include <Mpc.hpp>
#include <controls/AbstractControls.hpp>

using namespace mpc::hardware;
using namespace std;

Slider::Slider(mpc::Mpc* mpc)
{
	this->mpc = mpc;
}

void Slider::setValue(int i) {
	if (i < 0 || i > 127) return;
	value = i;
	if (mpc->getActiveControls() != nullptr) {
		mpc->getActiveControls()->setSlider(value);
	}
}

int Slider::getValue() {
	return value;
}

Slider::~Slider() {
}
