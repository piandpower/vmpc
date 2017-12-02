#include <hardware/HwPad.hpp>

#include <Mpc.hpp>

#include <controls/AbstractControls.hpp>

using namespace mpc::hardware;
using namespace std;

HwPad::HwPad(mpc::Mpc* mpc, int index)
{
	this->mpc = mpc;
	this->index = index;
}

int HwPad::getIndex() {
	return index;
}

void HwPad::push(int velo) {
	auto c = mpc->getActiveControls();
	if (!c) return;
	c->pad(index, velo, false, 0);
}

HwPad::~HwPad() {
}
