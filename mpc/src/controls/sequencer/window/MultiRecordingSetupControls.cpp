#include "MultiRecordingSetupControls.hpp"

#include <lcdgui/LayeredScreen.hpp>
#include <lcdgui/Field.hpp>
#include <ui/sequencer/window/MultiRecordingSetupLine.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <controls/KbMapping.hpp>

#include <lang/StrUtil.hpp>

using namespace mpc::controls::sequencer::window;
using namespace std;

MultiRecordingSetupControls::MultiRecordingSetupControls(mpc::Mpc* mpc)
	: AbstractSequencerControls(mpc)
{
}

void MultiRecordingSetupControls::init()
{
	super::init();
	yPos = 0;
	if (param.length() == 2) yPos = stoi(param.substr(1, 2));
}

void MultiRecordingSetupControls::left()
{
	init();
	if (csn.compare("multirecordingsetup") == 0 && param[0] == 'a') return;
	super::left();
}

void MultiRecordingSetupControls::right()
{
	init();
	if (param[0] == 'c') return;
	super::right();
}

void MultiRecordingSetupControls::turnWheel(int i)
{
	init();
	auto seq = sequence.lock();
	if (param[0] == 'b') {
		swGui->setMrsTrack(yPos + swGui->getMrsYOffset(), (*swGui->getVisibleMrsLines())[yPos]->getTrack() + i);
		swGui->setMrsOut(yPos + swGui->getMrsYOffset(), seq->getTrack((*swGui->getVisibleMrsLines())[yPos]->getTrack()).lock()->getDevice());
	}
	else if (param[0] == 'c') {
		swGui->setMrsOut(yPos + swGui->getMrsYOffset(), (*swGui->getVisibleMrsLines())[yPos]->getOut() + i);
		seq->getTrack((*swGui->getVisibleMrsLines())[yPos]->getTrack()).lock()->setDeviceNumber((*swGui->getVisibleMrsLines())[yPos]->getOut());
	}
}

void MultiRecordingSetupControls::keyEvent(unsigned char e)
{
	init();
	if (yPos == 0) {
		if (e == mpc::controls::KbMapping::down() || (param[0] == 'a' && e == mpc::controls::KbMapping::dataWheelForward())) {
			string res = param.substr(0, 1) + to_string(yPos + 1);
			ls.lock()->setFocus(param.substr(0, 1).append(to_string(yPos + 1)), 1);
			return;
		}
		else if (e == mpc::controls::KbMapping::up() || (param[0] == 'a' && e == mpc::controls::KbMapping::dataWheelBack())) {
			swGui->setMrsYOffset(swGui->getMrsYOffset() - 1);
			return;
		}
	}
	else if (yPos == 1) {
		if (e == mpc::controls::KbMapping::down() || (param[0] == 'a' && e == mpc::controls::KbMapping::dataWheelForward())) {
			ls.lock()->setFocus(param.substr(0, 1).append(to_string(yPos + 1)), 1);
			return;
		}
		else if (e == mpc::controls::KbMapping::up() || (param[0] == 'a' && e == mpc::controls::KbMapping::dataWheelBack())) {
			ls.lock()->setFocus(param.substr(0, 1).append(to_string(yPos - 1)), 1);
			return;
		}
	}
	else if (yPos == 2) {
		if (e == mpc::controls::KbMapping::down() || (param[0] == 'a' && e == mpc::controls::KbMapping::dataWheelForward())) {
			swGui->setMrsYOffset(swGui->getMrsYOffset() + 1);
			return;
		}
		else if (e == mpc::controls::KbMapping::up() || (param[0] == 'a' && e == mpc::controls::KbMapping::dataWheelBack())) {
			ls.lock()->setFocus(param.substr(0, 1).append(to_string(yPos - 1)), 1);
			return;
		}
	}
}
