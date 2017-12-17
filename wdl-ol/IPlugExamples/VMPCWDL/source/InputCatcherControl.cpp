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
}

bool InputCatcherControl::OnKeyDown(int x, int y, int c) {
	auto hw = mpc->getHardware().lock();
/*
	hw->getButton("after").lock()->push();
*/
	for (int i = 0; i < KbMapping::padKeys().size(); i++) {
		if (c == KbMapping::padKeys().at(i)) {
			hw->getPad(i).lock()->push(127);
			return true;
		}
	}

	if (c == KbMapping::numPadEnter()) {
		hw->getButton("enter").lock()->push();
		return true;
	}
	else if (c == KbMapping::fullLevel()) {
		hw->getButton("fulllevel").lock()->push();
		return true;
	}
	else if (c == KbMapping::sixteenLevels()) {
		hw->getButton("sixteenlevels").lock()->push();
		return true;
	}
	else if (c == KbMapping::tap()) {
		hw->getButton("tap").lock()->push();
		return true;
	}
	else if (c == KbMapping::rec()) {
		hw->getButton("rec").lock()->push();
		return true;
	}
	else if (c == KbMapping::overdub()) {
		hw->getButton("overdub").lock()->push();
		return true;
	}
	else if (c == KbMapping::stop()) {
		hw->getButton("stop").lock()->push();
		return true;
	}
	else if (c == KbMapping::play()) {
		hw->getButton("play").lock()->push();
		return true;
	}
	else if (c == KbMapping::playstart()) {
		hw->getButton("playstart").lock()->push();
		return true;
	}
	else if (c == KbMapping::prevStepEvent()) {
		hw->getButton("prevstepevent").lock()->push();
		return true;
	}
	else if (c == KbMapping::nextStepEvent()) {
		hw->getButton("nextstepevent").lock()->push();
		return true;
	}
	else if (c == KbMapping::goTo()) {
		hw->getButton("goto").lock()->push();
		return true;
	}
	else if (c == KbMapping::prevBarStart()) {
		hw->getButton("prevbarstart").lock()->push();
		return true;
	}
	else if (c == KbMapping::nextBarEnd()) {
		hw->getButton("nextbarend").lock()->push();
		return true;
	}
	else if (c == KbMapping::nextSeq()) {
		hw->getButton("nextseq").lock()->push();
		return true;
	}
	else if (c == KbMapping::trackMute()) {
		hw->getButton("trackmute").lock()->push();
		return true;
	}
	else if (c == KbMapping::mainscreen()) {
		hw->getButton("mainscreen").lock()->push();
		return true;
	}
	else if (c == KbMapping::undoSeq()) {
		hw->getButton("undoseq").lock()->push();
		return true;
	}
	else if (c == KbMapping::openWindow()) {
		hw->getButton("openwindow").lock()->push();
		return true;
	}
	else if (c == KbMapping::erase()) {
		hw->getButton("erase").lock()->push();
		return true;
	}
	else if (c == KbMapping::left()) {
		hw->getButton("left").lock()->push();
		return true;
	}
	else if (c == KbMapping::right()) {
		hw->getButton("right").lock()->push();
		return true;
	}
	else if (c == KbMapping::up()) {
		hw->getButton("up").lock()->push();
		return true;
	}
	else if (c == KbMapping::down()) {
		hw->getButton("down").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPadShift()) {
		hw->getButton("shift").lock()->push();
		return true;
	}
	else if (c == KbMapping::f1()) {
		hw->getButton("f1").lock()->push();
		return true;
	}
	else if (c == KbMapping::f2()) {
		hw->getButton("f2").lock()->push();
		return true;
	}
	else if (c == KbMapping::f3()) {
		hw->getButton("f3").lock()->push();
		return true;
	}
	else if (c == KbMapping::f4()) {
		hw->getButton("f4").lock()->push();
		return true;
	}
	else if (c == KbMapping::f5()) {
		hw->getButton("f5").lock()->push();
		return true;
	}
	else if (c == KbMapping::f6()) {
		hw->getButton("f6").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(0)) {
		hw->getButton("0").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(1)) {
		hw->getButton("1").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(2)) {
		hw->getButton("2").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(3)) {
		hw->getButton("3").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(4)) {
		hw->getButton("4").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(5)) {
		hw->getButton("5").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(6)) {
		hw->getButton("6").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(7)) {
		hw->getButton("7").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(8)) {
		hw->getButton("8").lock()->push();
		return true;
	}
	else if (c == KbMapping::numPad().at(9)) {
		hw->getButton("9").lock()->push();
		return true;
	}
	else if (c == KbMapping::bankKeys().at(0)) {
		hw->getButton("banka").lock()->push();
		return true;
	}
	else if (c == KbMapping::bankKeys().at(1)) {
		hw->getButton("bankb").lock()->push();
		return true;
	}
	else if (c == KbMapping::bankKeys().at(2)) {
		hw->getButton("bankc").lock()->push();
		return true;
	}
	else if (c == KbMapping::bankKeys().at(3)) {
		hw->getButton("bankd").lock()->push();
		return true;
	}
	else if (c == KbMapping::dataWheelBack()) {
		auto controls = mpc->getControls().lock();
		auto increment = -1;
		if (controls->isShiftPressed()) increment *= 10;
		hw->getDataWheel().lock()->turn(increment);
		return true;
	}
	else if (c == KbMapping::dataWheelForward()) {
		auto controls = mpc->getControls().lock();
		auto increment = 1;
		if (controls->isShiftPressed()) increment *= 10;
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

	if (c == KbMapping::erase()) {
		hw->getButton("erase").lock()->release();
		return true;
	}
	else if (c == KbMapping::numPadShift()) {
		hw->getButton("shift").lock()->release();
		return true;
	}
	else if (c == KbMapping::f1()) {
		hw->getButton("f1").lock()->release();
		return true;
	}
	else if (c == KbMapping::f3()) {
		hw->getButton("f3").lock()->release();
		return true;
	}
	else if (c == KbMapping::f5()) {
		hw->getButton("f5").lock()->release();
		return true;
	}
	else if (c == KbMapping::f6()) {
		hw->getButton("f6").lock()->release();
		return true;
	}
	else if (c == KbMapping::rec()) {
		hw->getButton("rec").lock()->release();
		return true;
	}
	else if (c == KbMapping::overdub()) {
		hw->getButton("overdub").lock()->release();
		return true;
	}
	else if (c == KbMapping::tap()) {
		hw->getButton("tap").lock()->release();
		return true;
	}
	return false;
}

InputCatcherControl::~InputCatcherControl() {
}
