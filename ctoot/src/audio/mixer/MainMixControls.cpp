#include <audio/mixer/MainMixControls.hpp>
#include <audio/mixer/RouteControl.hpp>
#include <audio/mixer/MixerControls.hpp>
#include <audio/mixer/MixerControlsIds.hpp>
#include <control/Control.hpp>
#include <control/EnumControl.hpp>

using namespace ctoot::audio::mixer;
using namespace std;

MainMixControls::MainMixControls(weak_ptr<MixerControls> mixerControls, int stripId, shared_ptr<BusControls> busControls, bool isMaster) : MixControls(mixerControls, stripId, busControls, isMaster)
{
}

EnumControl* MainMixControls::createRouteControl(int stripId)
{
	if (stripId != MixerControlsIds::MAIN_STRIP && stripId != MixerControlsIds::AUX_STRIP) {
		auto  controls = wMixerControls.lock()->getControls();
		auto c = controls[0];
		routeControl = new RouteControl(this, c->getName(), stripId == MixerControlsIds::CHANNEL_STRIP);
		return routeControl;
	}
	return nullptr;
}


EnumControl* MainMixControls::getRouteControl()
{
    return routeControl;
}

MainMixControls::~MainMixControls() {
}