#include "InputCatcherControl.hpp"

#include "Constants.hpp"

#include  "../resource.h"

InputCatcherControl::InputCatcherControl(IPlugBase* pPlug, mpc::controls::KbMouseController* kbmc)
	: IPanelControl(pPlug, *Constants::PLUG_RECT(), Constants::TRANS_BLACK())
{
	this->kbmc = kbmc;
}

bool InputCatcherControl::OnKeyDown(int x, int y, int key) {
	kbmc->press(key);
	return true;
}

bool InputCatcherControl::OnKeyUp(int x, int y, int key) {
	kbmc->release(key);
	return true;
}

InputCatcherControl::~InputCatcherControl() {
}
