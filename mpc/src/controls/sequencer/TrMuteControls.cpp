#include <controls/sequencer/TrMuteControls.hpp>

#include <controls/KbMouseController.hpp>
#include <lcdgui/LayeredScreen.hpp>
//#include <maingui/MainFrame.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/Sequencer.hpp>

using namespace mpc::controls::sequencer;
using namespace std;

TrMuteControls::TrMuteControls(mpc::Mpc* mpc)
	: AbstractSequencerControls(mpc)
{
}

void TrMuteControls::right()
{
}

void TrMuteControls::pad(int i, int velo, bool repeat, int tick)
{
	init();
	auto lSequencer = sequencer.lock();
	auto lLs = ls.lock();
	if (kbmc.lock()->isF6Pressed() || lSequencer->isSoloEnabled()) {
		if (!lSequencer->isSoloEnabled())
			lSequencer->setSoloEnabled(true);

		lSequencer->setSelectedTrackIndex(i + (bank_ * 16));
		lLs->removeCurrentBackground();
		lLs->setCurrentBackground("trackmutesolo2");
	}
	else {
		auto s = lSequencer->getActiveSequence().lock();
		auto t = s->getTrack(i + (bank_ * 16)).lock();
		t->setOn(!t->isOn());
	}
}

void TrMuteControls::turnWheel(int i)
{
	init();
	auto lSequencer = sequencer.lock();
	if (param.compare("sq") == 0) lSequencer->setActiveSequenceIndex(lSequencer->getActiveSequenceIndex() + i);
}

void TrMuteControls::function(int i)
{
	init();
	auto lSequencer = sequencer.lock();
	auto lLs = ls.lock();
	switch (i) {
	case 5:
		//mpc::controls::KbMouseController::f6IsPressed() = true;
		if (lSequencer->isSoloEnabled()) {
			lLs->removeCurrentBackground();
			lLs->setCurrentBackground("trackmute");
			lSequencer->setSoloEnabled(false);
		}
		else {
			lLs->removeCurrentBackground();
			lLs->setCurrentBackground("trackmutesolo1");
		}
		break;
	}
}
