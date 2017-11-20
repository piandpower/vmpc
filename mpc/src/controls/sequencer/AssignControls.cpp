#include <controls/sequencer/AssignControls.hpp>

#include <ui/sampler/SamplerGui.hpp>
#include <sampler/Program.hpp>
#include <sampler/PgmSlider.hpp>

using namespace mpc::controls::sequencer;
using namespace std;

AssignControls::AssignControls(mpc::Mpc* mpc)
	: AbstractSequencerControls(mpc)
{
}

void AssignControls::turnWheel(int i)
{
	init();
	auto notch = getNotch(i);
	auto slider = program.lock()->getSlider();
	auto parameter = slider->getParameter();

	if (param.compare("assignnote") == 0) {
		slider->setAssignNote(slider->getNote() + notch);
	}
	else if (param.compare("parameter") == 0) {
		slider->setParameter(slider->getParameter() + notch);
	}
	else if (param.compare("highrange") == 0) {
		switch (parameter) {
		case 0:
			slider->setTuneHighRange(slider->getTuneHighRange() + notch);
			break;
		case 1:
			slider->setDecayHighRange(slider->getDecayHighRange() + notch);
			break;
		case 2:
			slider->setAttackHighRange(slider->getAttackHighRange() + notch);
			break;
		case 3:
			slider->setFilterHighRange(slider->getFilterHighRange() + notch);
			break;
		}
	}
	else if (param.compare("lowrange") == 0) {
		switch (parameter) {
		case 0:
			slider->setTuneLowRange(slider->getTuneLowRange() + notch);
			break;
		case 1:
			slider->setDecayLowRange(slider->getDecayLowRange() + notch);
			break;
		case 2:
			slider->setAttackLowRange(slider->getAttackLowRange() + notch);
			break;
		case 3:
			slider->setFilterLowRange(slider->getFilterLowRange() + notch);
			break;
		}

	}
	else if (param.compare("assignnv") == 0) {
		slider->setControlChange(slider->getControlChange() + notch);
	}
}

void AssignControls::pad(int i, int velo, bool repeat, int tick)
{
    super::pad(i, velo, repeat, 0);
	auto lProgram = program.lock();
	auto nn = lProgram->getNoteFromPad(i + (samplerGui->getBank() * 16));
    lProgram->getSlider()->setAssignNote(nn);
}