#include <controls/mixer/window/ChannelSettingsControls.hpp>

#include <ui/sampler/MixerGui.hpp>
#include <sampler/mixerChannel.hpp>

using namespace mpc::controls::mixer::window;
using namespace std;

ChannelSettingsControls::ChannelSettingsControls(mpc::Mpc* mpc)
	: AbstractMixerControls(mpc)
{
}

void ChannelSettingsControls::turnWheel(int i)
{
    init();
	auto lMc = mixerChannel.lock();
	if (param.compare("note") == 0) {
		mixerGui->setChannelSettingsNote(mixerGui->getChannelSettingsNote() + i);
	}
	else if (param.compare("stereovolume") == 0) {
		lMc->setLevel(lMc->getLevel() + i);
	}
	else if (param.compare("individualvolume") == 0) {
		lMc->setVolumeIndividualOut(lMc->getVolumeIndividualOut() + i);
	}
	else if (param.compare("fxsendlevel") == 0) {
		lMc->setFxSendLevel(lMc->getFxSendLevel() + i);
	}
	else if (param.compare("panning") == 0) {
		lMc->setPanning(lMc->getPanning() + i);
	}
	else if (param.compare("output") == 0) {
		lMc->setOutput(lMc->getOutput() + i);
	}
	else if (param.compare("fxpath") == 0) {
		lMc->setFxPath(lMc->getFxPath() + i);
	}
	else if (param.compare("followstereo") == 0) {
		lMc->setFollowStereo(true);
	}
}

ChannelSettingsControls::~ChannelSettingsControls() {
}
