#include <controls/sequencer/window/Assign16LevelsControls.hpp>

#include <lcdgui/LayeredScreen.hpp>
//#include <hardware/LedPanel.hpp>
//#include <maingui/MainFrame.hpp>
#include <ui/sequencer/SequencerGui.hpp>

using namespace mpc::controls::sequencer::window;
using namespace std;

Assign16LevelsControls::Assign16LevelsControls(mpc::Mpc* mpc)
	: AbstractSequencerControls(mpc)
{
}

void Assign16LevelsControls::function(int i)
{
    super::function(i);
	switch (i) {
    case 4:
        sequencerGui->setSixteenLevelsEnabled(true);
        //lMainFrame->getLedPanel().lock()->setSixteenLevels(true);
        ls.lock()->openScreen(ls.lock()->getPreviousScreenName());
        break;
    }
}

void Assign16LevelsControls::turnWheel(int i)
{
	init();
	if (param.compare("note") == 0)
		sequencerGui->setNote(sequencerGui->getNote() + i);

	if (param.compare("param") == 0)
		sequencerGui->setParameter(sequencerGui->getParameter() + i);

	if (param.compare("type") == 0)
		sequencerGui->setType(sequencerGui->getType() + i);

	if (param.compare("originalkeypad") == 0)
		sequencerGui->setOriginalKeyPad(sequencerGui->getOriginalKeyPad() + i);
}
