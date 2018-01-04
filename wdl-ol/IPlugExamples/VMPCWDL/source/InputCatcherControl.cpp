#include "InputCatcherControl.hpp"

#include "Constants.hpp"

#include <Mpc.hpp>

#include <controls/KbMapping.hpp>
#include <controls/Controls.hpp>
#include <hardware/Hardware.hpp>
#include <hardware/DataWheel.hpp>
#include <hardware/Button.hpp>
#include <hardware/HwPad.hpp>

#include  "../resource.h"

using namespace mpc::controls;

InputCatcherControl::InputCatcherControl(IPlugBase* pPlug, mpc::Mpc* mpc)
	: IPanelControl(pPlug, *Constants::PLUG_RECT(), Constants::TRANS_BLACK())
{
	this->mpc = mpc;
	kbMapping = new mpc::controls::KbMapping();
}

bool InputCatcherControl::OnKeyDown(int x, int y, int c) {
	auto hw = mpc->getHardware().lock();

	for (int i = 0; i < KbMapping::padKeys().size(); i++) {
		if (c == KbMapping::padKeys().at(i)) {
			hw->getPad(i).lock()->push(127);
			return true;
		}
	}

	auto label = kbMapping->getLabelFromKeyCode(c);
	if (label.compare("") != 0) {
		hw->getButton(label).lock()->push();
		return true;
	}


	if (c == KbMapping::ctrl()) {
		mpc->getControls().lock()->setCtrlPressed(true);
		return true;
	}
	else  if (c == KbMapping::alt()) {
		mpc->getControls().lock()->setAltPressed(true);
		return false;
	}
	else if (c == KbMapping::dataWheelBack()) {
		auto controls = mpc->getControls().lock();
		auto increment = -1;
		if (controls->isShiftPressed()) increment *= 10;
		if (controls->isAltPressed()) increment *= 10;
		if (controls->isCtrlPressed()) increment *= 10;
		hw->getDataWheel().lock()->turn(increment);
		return true;
	}
	else if (c == KbMapping::dataWheelForward()) {
		auto controls = mpc->getControls().lock();
		auto increment = 1;
		if (controls->isShiftPressed()) increment *= 10;
		if (controls->isAltPressed()) increment *= 10;
		if (controls->isCtrlPressed()) increment *= 10;
		hw->getDataWheel().lock()->turn(increment);
		return true;
	}
	return false;
}

bool InputCatcherControl::OnKeyUp(int x, int y, int c) {	
	auto hw = mpc->getHardware().lock();

	for (int i = 0; i < KbMapping::padKeys().size(); i++) {
		if (c == KbMapping::padKeys().at(i)) {
			hw->getPad(i).lock()->release();
			return true;
		}
	}

	if (c == KbMapping::ctrl()) {
		mpc->getControls().lock()->setCtrlPressed(false);
		return true;
	}
	else  if (c == KbMapping::alt()) {
		mpc->getControls().lock()->setAltPressed(false);
		return false;
	}

	auto label = kbMapping->getLabelFromKeyCode(c);
	if (label.compare("") != 0) {
		hw->getButton(label).lock()->release();
		return true;
	}

	return false;
}

InputCatcherControl::~InputCatcherControl() {
}
