#include <controls/mixer/AbstractMixerControls.hpp>

#include <Mpc.hpp>
#include <ui/sampler/MixerGui.hpp>
#include <ui/sampler/MixerSetupGui.hpp>
#include <sampler/MixerChannel.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>

using namespace mpc::controls::mixer;
using namespace std;

AbstractMixerControls::AbstractMixerControls(mpc::Mpc* mpc) 
	: AbstractControls(mpc)
{
	mixerGui = mpc->getUis().lock()->getMixerGui();
	mixerSetupGui = mpc->getUis().lock()->getMixerSetupGui();
}

void AbstractMixerControls::init()
{
	super::init();
	auto lProgram = program.lock();
	mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixerGui->getChannelSettingsNote()))->getMixerChannel();
}

AbstractMixerControls::~AbstractMixerControls() {
}
