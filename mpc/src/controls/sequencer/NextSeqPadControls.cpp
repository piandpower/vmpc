#include <controls/sequencer/NextSeqPadControls.hpp>

#include <lcdgui/LayeredScreen.hpp>
#include <sequencer/Sequencer.hpp>

using namespace mpc::controls::sequencer;
using namespace std;

NextSeqPadControls::NextSeqPadControls(mpc::Mpc* mpc)
	: AbstractSequencerControls(mpc)
{
}

void NextSeqPadControls::right()
{
}

void NextSeqPadControls::pad(int i, int velo, bool repeat, int tick)
{
	init();
	sequencer.lock()->setNextSqPad(i + (bank_ * 16));
}

void NextSeqPadControls::turnWheel(int i)
{
	init();
	auto lSequencer = sequencer.lock();
	if (param.compare("sq") == 0)
		lSequencer->setActiveSequenceIndex(lSequencer->getActiveSequenceIndex() + i);
}

void NextSeqPadControls::function(int i)
{
	init();
	switch (i) {
	case 5:
		ls.lock()->openScreen("nextseq");
		break;
	}
}
