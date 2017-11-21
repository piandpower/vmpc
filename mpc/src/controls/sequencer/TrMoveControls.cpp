#include "TrMoveControls.hpp"

#include <ui/sequencer/BarCopyGui.hpp>
#include <ui/sequencer/EditSequenceGui.hpp>
#include <ui/sequencer/TrMoveGui.hpp>
#include <ui/Uis.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <lcdgui/Field.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Sequencer.hpp>

using namespace mpc::controls::sequencer;
using namespace std;

TrMoveControls::TrMoveControls(mpc::Mpc* mpc)
	: AbstractSequencerControls(mpc)
{
	tmGui = mpc->getUis().lock()->getTrMoveGui();
}

void TrMoveControls::init()
{
    super::init();
    seq = sequencer.lock()->getSequence(tmGui->getSq());
}

void TrMoveControls::turnWheel(int i)
{
	init();
	if (param.find("tr") != string::npos && i > 0) tmGui->goUp();
	if (param.find("tr") != string::npos && i < 0) tmGui->goDown();
	if (param.compare("sq") == 0) tmGui->setSq(tmGui->getSq() + i);
}

void TrMoveControls::up()
{
	init();
	if (param.find("tr") != string::npos) tmGui->goUp();
}

void TrMoveControls::down()
{
	init();
	if (param.find("tr") != string::npos) tmGui->goDown();
}

void TrMoveControls::left()
{
	init();
	if (tmGui->isSelected() && param.compare("tr0") == 0) return;
	if (!tmGui->isSelected() && param.compare("sq") == 0) return;
	super::left();
}

void TrMoveControls::right()
{
	init();
	if (tmGui->isSelected() && param.compare("tr0") == 0) return;
	if (!tmGui->isSelected() && param.compare("tr1") == 0) return;
	super::right();
}

void TrMoveControls::function(int i)
{
	init();
	switch (i) {
	case 0:
		mpc->getUis().lock()->getEditSequenceGui()->setFromSq(tmGui->getSq());
		ls.lock()->openScreen("edit");
		break;
	case 1:
		mpc->getUis().lock()->getBarCopyGui()->setFromSq(tmGui->getSq());
		ls.lock()->openScreen("barcopy");
		break;
	case 3:
		ls.lock()->openScreen("user");
		break;
	case 4:
		if (tmGui->isSelected()) tmGui->cancel();
		ls.lock()->setFocus("tr1", 0);
		break;
	case 5:
		if (tmGui->isSelected()) {
			tmGui->insert(seq.lock().get());
			ls.lock()->setFocus("tr1", 0);
		}
		else {
			tmGui->select();
			ls.lock()->setFocus("tr0", 0);
		}
		break;
	}
}
