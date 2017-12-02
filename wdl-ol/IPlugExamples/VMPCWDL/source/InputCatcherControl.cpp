#include "InputCatcherControl.hpp"

#include "Constants.hpp"

#include <Mpc.hpp>

#include <controls/KbMapping.hpp>
#include <hardware/Hardware.hpp>
#include <hardware/DataWheel.hpp>
#include <hardware/Button.hpp>


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
	hw->getButton("f1").lock()->push();
	hw->getButton("f2").lock()->push();
	hw->getButton("f3").lock()->push();
	hw->getButton("f4").lock()->push();
	hw->getButton("f5").lock()->push();
	hw->getButton("f6").lock()->push();
	hw->getButton("enter").lock()->push();
	hw->getButton("erase").lock()->push();
	hw->getButton("after").lock()->push();
	hw->getButton("banka").lock()->push();
	hw->getButton("bankb").lock()->push();
	hw->getButton("bankc").lock()->push();
	hw->getButton("bankd").lock()->push();
*/

	if (c == KbMapping::fullLevel()) {
		hw->getButton("fulllevel").lock()->push();
	}
	else if (c == KbMapping::sixteenLevels()) {
		hw->getButton("sixteenlevels").lock()->push();
	}
	else if (c == KbMapping::tap()) {
		hw->getButton("tap").lock()->push();
	}
	else if (c == KbMapping::rec()) {
		hw->getButton("rec").lock()->push();
	}
	else if (c == KbMapping::overdub()) {
		hw->getButton("overdub").lock()->push();
	}
	else if (c == KbMapping::stop()) {
		hw->getButton("stop").lock()->push();
	}
	else if (c == KbMapping::play()) {
		hw->getButton("play").lock()->push();
	}
	else if (c == KbMapping::playstart()) {
		hw->getButton("playstart").lock()->push();
	}
	else if (c == KbMapping::prevStepEvent()) {
		hw->getButton("prevstepevent").lock()->push();
	}
	else if (c == KbMapping::nextStepEvent()) {
		hw->getButton("nextstepevent").lock()->push();
	}
	else if (c == KbMapping::goTo()) {
		hw->getButton("goto").lock()->push();
	}
	else if (c == KbMapping::prevBarStart()) {
		hw->getButton("prevbarstart").lock()->push();
	}
	else if (c == KbMapping::nextBarEnd()) {
		hw->getButton("nextbarend").lock()->push();
	}
	else if (c == KbMapping::nextSeq()) {
		hw->getButton("nextseq").lock()->push();
	}
	else if (c == KbMapping::trackMute()) {
		hw->getButton("trackmute").lock()->push();
	}
	else if (c == KbMapping::mainscreen()) {
		hw->getButton("mainscreen").lock()->push();
	}
	else if (c == KbMapping::undoSeq()) {
		hw->getButton("undoseq").lock()->push();
	}
	//else if (numPressed) {
		//controls->numpad(numIndex);
	//}
	else if (c == KbMapping::openWindow()) {
		hw->getButton("openwindow").lock()->push();
	}
	else if (c == KbMapping::erase()) {
		//controls->erase();
	}
	else if (c == KbMapping::left()) {
		hw->getButton("left").lock()->push();
	}
	else if (c == KbMapping::right()) {
		hw->getButton("right").lock()->push();
	}
	else if (c == KbMapping::up()) {
		hw->getButton("up").lock()->push();
	}
	else if (c == KbMapping::down()) {
		hw->getButton("down").lock()->push();
	}
	else if (c == KbMapping::numPadShift()) {
		hw->getButton("shift").lock()->push();
	}
	else if (c == KbMapping::dataWheelBack()) {
		hw->getDataWheel().lock()->turn(-1);
	}
	else if (c == KbMapping::dataWheelForward()) {
		hw->getDataWheel().lock()->turn(1);
	}
	else if (c == KbMapping::f1()) {
		hw->getButton("f1").lock()->push();
	}
	else if (c == KbMapping::f2()) {
		hw->getButton("f2").lock()->push();
	}
	else if (c == KbMapping::f3()) {
		hw->getButton("f3").lock()->push();
	}
	else if (c == KbMapping::f4()) {
		hw->getButton("f4").lock()->push();
	}
	else if (c == KbMapping::f5()) {
		hw->getButton("f5").lock()->push();
	}
	else if (c == KbMapping::f6()) {
		hw->getButton("f6").lock()->push();
	}
	return true;
}

bool InputCatcherControl::OnKeyUp(int x, int y, int key) {
	//kbmc->release(key);
	return true;
}

InputCatcherControl::~InputCatcherControl() {
}
