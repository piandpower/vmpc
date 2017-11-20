#include "UserControls.hpp"

#include <maingui/StartUp.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <ui/UserDefaults.hpp>
#include <sequencer/TimeSignature.hpp>

using namespace mpc::controls::sequencer;
using namespace std;

UserControls::UserControls(mpc::Mpc* mpc) 
	: AbstractSequencerControls(mpc)
{
	ud = mpc::maingui::StartUp::getUserDefaults();
}

void UserControls::function(int i)
{
    init();
	switch (i) {
    case 0:
        ls.lock()->openScreen("edit");
        break;
    case 1:
        ls.lock()->openScreen("barcopy");
        break;
    case 2:
        ls.lock()->openScreen("trmove");
        break;
    }

}

void UserControls::turnWheel(int i)
{
	init();
	auto notch = getNotch(i);
	auto lUd = ud.lock();
	if (param.compare("tempo") == 0) {
		double oldTempo = lUd->getTempo().toDouble();
		double newTempo = oldTempo + (notch / 10.0);
		lUd->setTempo(BCMath(newTempo));
	}
	else if (param.compare("loop") == 0) {
		lUd->setLoop(notch > 0);
	}
	else if (param.compare("tsig") == 0) {
		if (notch > 0) {
			lUd->getTimeSig()->increase();
		}
		else {
			lUd->getTimeSig()->decrease();
		}
	}
	else if (param.compare("bars") == 0) {
		lUd->setLastBar(lUd->getLastBarIndex() + notch);
	}
	else if (param.compare("pgm") == 0) {
		lUd->setPgm(lUd->getPgm() + notch);
	}
	else if (param.compare("recordingmode") == 0) {
		lUd->setRecordingModeMulti(notch > 0);
	}
	else if (param.compare("tracktype") == 0) {
		lUd->setBus(lUd->getBus() + notch);
	}
	else if (param.compare("devicenumber") == 0) {
		lUd->setDeviceNumber(lUd->getDeviceNumber() + notch);
	}
	else if (param.compare("velo") == 0) {
		lUd->setVelo(lUd->getVeloRatio() + notch);
	}
}
