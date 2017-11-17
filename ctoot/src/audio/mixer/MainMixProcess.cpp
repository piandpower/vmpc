#include <audio/mixer/MainMixProcess.hpp>
#include <audio/mixer/AudioMixer.hpp>
#include <audio/mixer/AudioMixerStrip.hpp>
#include <util/RouteObserver.hpp>
#include <audio/mixer/MixVariables.hpp>
#include <control/EnumControl.hpp>
#include <audio/mixer/MainMixControls.hpp>

using namespace std;
using namespace ctoot::audio::mixer;

MainMixProcess::MainMixProcess(shared_ptr<AudioMixerStrip> strip, MixVariables* vars, AudioMixer* mixer) : MixProcess(strip, vars)
{
	auto mmc = dynamic_cast<MainMixControls*>(vars);
	routeControl = mmc->getRouteControl();
	if (routeControl != nullptr) {
		routeObserver = new util::RouteObserver(this, mixer);
		routedStrip = mixer->getStripImpl(routeControl->getValueString());
	}
}

AudioMixerStrip* MainMixProcess::getRoutedStrip()
{
	if (routeControl != nullptr && nextRoutedStrip.lock() != nullptr) {
		routedStrip = nextRoutedStrip;
		nextRoutedStrip = weak_ptr<AudioMixerStrip>();
	}
	return super::getRoutedStrip();
}

void MainMixProcess::open()
{
	super::open();
	if (routeControl != nullptr && routeObserver != nullptr) {
		routeControl->addObserver(routeObserver);
	}
}

void MainMixProcess::close()
{
	if (routeControl != nullptr && routeObserver != nullptr) {
		routeControl->deleteObserver(routeObserver);
	}
	super::close();
}

MainMixProcess::~MainMixProcess() {
	if (routeObserver != nullptr) {
		delete routeObserver;
	}
}
