#include "TrackControls.hpp"

#include <lcdgui/LayeredScreen.hpp>
#include <ui/NameGui.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/Sequencer.hpp>

using namespace mpc::controls::sequencer::window;
using namespace std;

TrackControls::TrackControls(mpc::Mpc* mpc)
	: AbstractSequencerControls(mpc)
{
}

void TrackControls::function(int i)
{
	super::function(i);
	switch (i) {
	case 1:
		ls.lock()->openScreen("deletetrack");
		break;
	case 4:
		ls.lock()->openScreen("copytrack");
		break;
	}
}

void TrackControls::turnWheel(int i)
{
	init();
	auto lSequencer = sequencer.lock();
	if (param.find("default") != string::npos) {
		nameGui->setName(lSequencer->getDefaultTrackName(lSequencer->getActiveTrackIndex()));
	}
	else {
		nameGui->setName(track.lock()->getName());
	}
	nameGui->setParameterName(param);
	ls.lock()->openScreen("name");
}
