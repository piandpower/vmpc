#include <controls/sequencer/NextSeqControls.hpp>

#include <lcdgui/LayeredScreen.hpp>
#include <sequencer/Sequencer.hpp>

using namespace mpc::controls::sequencer;
using namespace std;

NextSeqControls::NextSeqControls(mpc::Mpc* mpc)
	: AbstractSequencerControls(mpc)
{
}

void NextSeqControls::turnWheel(int i)
{
	init();
	auto lSequencer = sequencer.lock();
	auto notch = getNotch(i);
	if (param.compare("sq") == 0) {
		if (lSequencer->isPlaying()) {
			lSequencer->setNextSq(lSequencer->getCurrentlyPlayingSequenceIndex() + notch);
		}
		else {
			lSequencer->setActiveSequenceIndex(lSequencer->getActiveSequenceIndex() + notch);
		}
	}
	if (param.compare("nextsq") == 0)
		lSequencer->setNextSq(lSequencer->getNextSq() + notch);
}

void NextSeqControls::function(int i)
{
	init();
	switch (i) {
	case 5:
		ls.lock()->openScreen("nextseqpad");
		break;
	}
}
