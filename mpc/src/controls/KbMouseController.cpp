#include "KbMouseController.hpp"

#include <controls/Controls.hpp>

#include <controls/sequencer/SequencerControls.hpp>
//#include <maingui/Constants.hpp>
#include <Mpc.hpp>
#include <Util.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <controls/AbstractControls.hpp>
#include <controls/GlobalReleaseControls.hpp>
#include <controls/KbMapping.hpp>
//#include <disk/AbstractDisk.hpp>
#include <StartUp.hpp>
//#include <hardware/ControlPanel.hpp>
#include <ui/Uis.hpp>
//#include <gui/KeyLabels.hpp>
#include <lcdgui/LayeredScreen.hpp>
////#include <maingui/MainFrame.hpp>
#include <lcdgui/Field.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <ui/sequencer/SequencerGui.hpp>
#include <sampler/Sampler.hpp>
#include <gui/imagemap/Shape.hpp>
#include <gui/imagemap/ShapeList.hpp>

//#include <hardware/DataWheelWidget.hpp>

#include <cmath>

#include <lang/String.hpp>

using namespace mpc::controls;
//using namespace mpc::gui;
using namespace std;

KbMouseController::KbMouseController(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	shiftPressed = false;
	releaseControls = make_unique<GlobalReleaseControls>(mpc);
	samplerGui = mpc->getUis().lock()->getSamplerGui();
}

void KbMouseController::turnDataWheel(int increment) {
	init();
	if (controls != nullptr) {
		controls->turnWheel(increment);
	}
	dataWheelIndex += increment;
	if (dataWheelIndex < 0)
		dataWheelIndex = 99;

	if (dataWheelIndex > 99)
		dataWheelIndex = 0;
}

void KbMouseController::releaseShift() {
	shiftPressed = false;
}

int KbMouseController::getJump()
{
    return jump;
}

bool KbMouseController::isGoToPressed()
{
    return goToPressed;
}

set<int>* KbMouseController::getPressedPads()
{
    return &pressedPads;
}

vector<int>* KbMouseController::getPressedPadVelos() {
	return &pressedPadVelos;
}

void KbMouseController::setGoToPressed(bool b) {
	goToPressed = b;
}

void KbMouseController::setF6Pressed(bool b) {
	f6Pressed = b;
}

void KbMouseController::setAltPressed(bool b) {
	altPressed = b;
}

void KbMouseController::setShiftPressed(bool b) {
	shiftPressed = b;
}

void KbMouseController::setRecPressed(bool b) {
	recPressed = b;
}

void KbMouseController::setOverdubPressed(bool b) {
	overdubPressed = b;
}

void KbMouseController::setTapPressed(bool b) {
	tapPressed = b;
}

void KbMouseController::setErasePressed(bool b) {
	erasePressed = b;
}

bool KbMouseController::isF6Pressed()
{
    return f6Pressed;
}

bool KbMouseController::isAltPressed()
{
    return altPressed;
}

bool KbMouseController::isShiftPressed()
{
    return shiftPressed;
}

bool KbMouseController::isRecPressed()
{
    return recPressed;
}

bool KbMouseController::isOverdubPressed()
{
    return overdubPressed;
}

bool KbMouseController::isTapPressed()
{
    return tapPressed;
}

bool KbMouseController::isErasePressed()
{
    return erasePressed;
}

void KbMouseController::init()
{
	controls = mpc->getControls().lock()->getControls(mpc->getLayeredScreen().lock()->getCurrentScreenName());
}

void KbMouseController::press(unsigned char c)
{
	if (ctrlPressed && (c == 81 || c == 88)) {
		auto lAms = mpc->getAudioMidiServices().lock();
		if (!lAms || !lAms->isStandalone()) return;
		//exit(0);
		// EXIT ROUTINE
		return;
	}

    bool functionPressed = false;
    int functionIndex = 0;
    vector<int> fKeys { KbMapping::f1(), KbMapping::f2(), KbMapping::f3(), KbMapping::f4(), KbMapping::f5(), KbMapping::f6() };
    for (auto& i : fKeys) {
        if (i == c) {
            functionPressed = true;
            break;
        }
        functionIndex++;
    }
    
    bool numPressed = false;
    int numIndex = 0;
    for (auto& i : KbMapping::numPad()) {
        if  (i == c) {
            numPressed = true;
            break;
        }
        numIndex++;
    }
    
	init();
	
	if (mpc->getAudioMidiServices().lock()->isDisabled()) {
		if (!(c == KbMapping::numPadShift() || c == KbMapping::numPad()[0] || c == KbMapping::f4() || c == KbMapping::alt() || c == KbMapping::ctrl()) && mpc->getLayeredScreen().lock()->getCurrentScreenName().compare("audio") != 0) {
			mpc->getLayeredScreen().lock()->openScreen("audiomididisabled");
			return;
		}
	}

	if (c == KbMapping::alt()) {
		altPressed = true;
	}
	if (c == KbMapping::ctrl()) {
		ctrlPressed = true;
	}
	//if(ctrlPressed && altPressed) mainFrame->getKeyLabels()->displayKeys();
	
	jump = 1;
	if (shiftPressed)
		jump = 10;

	if (shiftPressed && ctrlPressed)
		jump = 100;

	if (ctrlPressed && altPressed)
		jump = 1000;

	if (ctrlPressed && shiftPressed && altPressed)
		jump = 10000;

	auto ls = mpc->getLayeredScreen().lock();
	ls->setLastFocus(ls->getCurrentScreenName(), ls->getFocus());

	if (controls != nullptr) {
		for (int i = 0; i < 4; i++) {
			if (c == KbMapping::bankKeys()[i]) {
				controls->bank(i);
			}
		}
		//auto lDisk = mpc->getDisk().lock();

		//if (!(lDisk && lDisk->isBusy())) {
			if ((c < KbMapping::numPad()[0] || c > KbMapping::numPad()[9]) && c != KbMapping::numPadEnter()) {
				if (ls->getFocus() != "") {
					auto tf = ls->lookupField(ls->getFocus()).lock();
					if (tf) {
						tf->disableTypeMode();
					}
				}
			}

			if (ls->getCurrentScreenName().compare("name") != 0) {
				if (c == KbMapping::fullLevel()) {
					controls->fullLevel();
				}
				else if (c == KbMapping::sixteenLevels()) {
					controls->sixteenLevels();
				}
				else if (c == KbMapping::tap()) {
					controls->tap();
				}
				else if (c == KbMapping::rec()) {
					controls->rec();
				}
				else if (c == KbMapping::overdub()) {
					controls->overDub();
				}
				else if (c == KbMapping::stop()) {
					controls->stop();
				}
				else if (c == KbMapping::play()) {
					controls->play();
				}
				else if (c == KbMapping::playstart()) {
					controls->playStart();
				}
				else if (c == KbMapping::prevStepEvent()) {
					controls->prevStepEvent();
				}
				else if (c == KbMapping::nextStepEvent()) {
					controls->nextStepEvent();
				}
				else if (c == KbMapping::goTo()) {
					controls->goTo();
				}
				else if (c == KbMapping::prevBarStart()) {
					controls->prevBarStart();
				}
				else if (c == KbMapping::nextBarEnd()) {
					controls->nextBarEnd();
				}
				else if (c == KbMapping::nextSeq()) {
					controls->nextSeq();
				}
				else if (c == KbMapping::trackMute()) {
					controls->trackMute();
				}
				else if (c == KbMapping::mainscreen()) {
					controls->mainScreen();
				}
				else if (c == KbMapping::undoSeq()) {
					controls->undoSeq();
				}
				else if (numPressed) {
					controls->numpad(numIndex);
				}
				else if (c == KbMapping::openWindow()) {
					controls->openWindow();
				}
			}

			if (c == KbMapping::erase()) controls->erase();

			auto csn = ls->getCurrentScreenName();
			bool soundCsn = csn.compare("trim") == 0 || csn.compare("loop") == 0 || csn.compare("zone") == 0;
			bool noSounds = mpc->getSampler().lock()->getSoundCount() == 0;
			
			if (!(soundCsn && noSounds)) {
				if (c == KbMapping::left()) {
					controls->left();
				}
				else if (c == KbMapping::right()) {
					controls->right();
				}
				else if (c == KbMapping::up() && csn.compare("sequencer_step") != 0 && csn.compare("multirecordingsetup") != 0) {
					controls->up();
				}
				else if (c == KbMapping::down() && !csn.compare("sequencer_step") == 0 && csn.compare("multirecordingsetup") != 0) {
					controls->down();
				}
			}
			if (c == KbMapping::numPadShift()) {
				controls->shift();
			}
			else if (c == KbMapping::dataWheelBack()) {
				turnDataWheel(-jump);
			}
			else if (c == KbMapping::dataWheelForward()) {
				turnDataWheel(jump);
			}
			else if (functionPressed && !altPressed) {
				if (c == KbMapping::f6()) {
					if (f6Pressed) {
						return;
					}
					else {
						f6Pressed = true;
					}
				}
				controls->function(functionIndex);
			}
			else if (c == KbMapping::numPadEnter()) {
				controls->pressEnter();
			}

			if (getPressedPad(c) != -1 && pressedPads.find((int)(getPressedPad(c))) == pressedPads.end()) {
				controls->pad(getPressedPad(c), 127, false, 0);
			}
			controls->keyEvent(c); // generic key stroke processing in AbstractControls or children
		}
	//}
}

void KbMouseController::release(unsigned char c)
{
	
	if (mpc->getAudioMidiServices().lock()->isDisabled()) {
		if (!(c == KbMapping::numPadShift() || c == KbMapping::alt() || c == KbMapping::ctrl()) && mpc->getLayeredScreen().lock()->getCurrentScreenName().compare("audio") != 0) {
			return;
		}
	}
	init();

	if (c == KbMapping::ctrl()) {
		ctrlPressed = false;
	}
	else if (c == KbMapping::alt()) {
		altPressed = false;
	}
	else if (c == KbMapping::f6()) {
		f6Pressed = false;
	}
	else if (c == KbMapping::numPadShift()) {
		releaseControls->shift();
	}

	//auto lDisk = gui->getMpc()->getDisk().lock();
	
	//if (lDisk && lDisk->isBusy()) {
//		return;
	//}

	if (releaseControls) {
		releaseControls->keyEvent(c);
		if (c == KbMapping::tap()) {
			releaseControls->tap();
		}
		else if (c == KbMapping::erase()) {
			releaseControls->erase();
		}
	}
	//if(!altPressed && !ctrlPressed)
	//    mainFrame->getKeyLabels()->removeKeys();

}

int KbMouseController::getPressedPad(char c)
{
	for (int i = 0; i < 16; i++) {
		if (c == KbMapping::padKeys()[i]) {
			return i;
		}
	}
	return -1;
}

void KbMouseController::mousePressed(int x, int y)
{
    init();
    auto padVelo = mpc::Util::getPadAndVelo(x, y);
	if (padVelo[0] != -1) {
		if (samplerGui == nullptr || controls == nullptr) {
			return;
		}
		controls->pad(padVelo[0], mpc->getUis().lock()->getSequencerGui()->isFullLevelEnabled() ? 127 : padVelo[1], false, 0);
		return;
	}
	/*
	auto lDisk = gui->getMpc()->getDisk().lock();
	if (lDisk && lDisk->isBusy()) {
		return;
	}
	*/

	auto control = getShape(x, y).lock();
    if (control && controls != nullptr) {
		auto s = control->get_href().lock()->to_cppstring();
		if (s.compare("play") == 0) {
			controls->play();
		}
		else if (s.compare("rec") == 0) {
			controls->rec();
		}
		else if (s.compare("playstart") == 0) {
			controls->playStart();
		}
		else if (s.compare("overdub") == 0) {
			controls->overDub();
		}
		else if (s.compare("stop") == 0) {
			controls->stop();
		}
		else if (s.compare("nextseq") == 0) {
			controls->nextSeq();
		}
		else if (s.compare("trackmute") == 0) {
			controls->trackMute();
		}
		else if (s.compare("mainscreen") == 0) {
			controls->mainScreen();
		}
		else if (s.compare("openwindow") == 0) {
			controls->openWindow();
		}
		else if (s.compare("taptemponoterepeat") == 0) {
			controls->tap();
		}
		else if (s.find("f") == 0 && s.length() == 2) {
			controls->function(stoi(s.substr(1, 2)) - 1);
		}
		else if (s.compare("up") == 0) {
			controls->up();
		}
		else if (s.compare("down") == 0) {
			controls->down();
		}
		else if (s.compare("left") == 0) {
			controls->left();
		}
		else if (s.compare("right") == 0) {
			controls->right();
		}
		else if (s.compare("prevstepevent") == 0) {
			controls->prevStepEvent();
		}
		else if (s.compare("nextstepevent") == 0) {
			controls->nextStepEvent();
		}
		else if (s.compare("goto") == 0) {
			controls->goTo();
		}
		else if (s.compare("prevbarstart") == 0) {
			controls->prevBarStart();
		}
		else if (s.compare("nextbarend") == 0) {
			controls->nextBarEnd();
		}
		else if (s.compare("banka") == 0) {
			controls->bank(0);
		}
		else if (s.compare("bankb") == 0) {
			controls->bank(1);
		}
		else if (s.compare("bankc") == 0) {
			controls->bank(2);
		}
		else if (s.compare("bankd") == 0) {
			controls->bank(3);
		}
		else if (s.compare("fulllevel") == 0) {
			controls->fullLevel();
		}
		else if (s.compare("16levels") == 0) {
			controls->sixteenLevels();
		}
		else if (s.compare("notevariationafter") == 0) {
			controls->after();
		}
		else if (s.compare("shift") == 0) {
			controls->shift();
		}
		else if (s.compare("undoseq") == 0) {
			controls->undoSeq();
		}
		else if (s.compare("erase") == 0) {
			controls->erase();
		}

    }
}

std::weak_ptr<moduru::gui::imagemap::Shape> KbMouseController::getShape(int x, int y)
{
	/*
	auto sl = mainFrame.lock()->getControlPanel().lock()->getControlShapes().lock();
	for (int i = 0; i < sl->size(); i++) {
		auto s = sl->get_shape(i).lock();
		if (s->inside(x, y))
			return s;
	}
	*/
	return std::weak_ptr<moduru::gui::imagemap::Shape>();
}

AbstractControls* KbMouseController::getControls()
{
    return controls;
}

KbMouseController::~KbMouseController() {
}
