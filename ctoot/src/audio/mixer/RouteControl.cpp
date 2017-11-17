#include <audio/mixer/RouteControl.hpp>
#include <audio/core/AudioControlsChain.hpp>
#include <audio/mixer/MainMixControls.hpp>
#include <audio/mixer/MixControlIds.hpp>
#include <audio/mixer/MixerControls.hpp>
#include <audio/mixer/MixerControlsIds.hpp>
#include <control/Control.hpp>

using namespace ctoot::audio::mixer;
using namespace ctoot::control;
using namespace std;

RouteControl::RouteControl(MainMixControls* mmc, string defaultRoute, bool canRouteToGroups) : EnumControl(MixControlIds::ROUTE, string("Route"), defaultRoute)
{
	this->mmc = mmc;
	canRouteToGroups = true;
	this->canRouteToGroups = canRouteToGroups;
	indicator = !canRouteToGroups;
}


vector<boost::any> RouteControl::getValues()
{
	vector<boost::any> values;
	auto controls = mmc->mixerControls->getControls();
	for (int i = 0; i < controls.size(); i++) {
		auto control = controls[i];
		if (dynamic_pointer_cast<core::AudioControlsChain>(control) != nullptr) {
			if (control->getId() == MixerControlsIds::MAIN_STRIP || (control->getId() == MixerControlsIds::GROUP_STRIP && canRouteToGroups)) {
				values.push_back(control->getName());
			}
		}
	}
	return values;
}
