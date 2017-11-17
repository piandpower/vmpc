#include <audio/mixer/MixControls.hpp>

#include <audio/mixer/MainMixControls.hpp>

#include <audio/core/ChannelFormat.hpp>
#include <audio/fader/FaderControl.hpp>
#include <audio/mixer/SoloIndicator.hpp>
#include <audio/mixer/BusControls.hpp>
#include <audio/mixer/MixControlIds.hpp>
#include <audio/mixer/BalanceControl.hpp>
#include <audio/mixer/FrontRearControl.hpp>
#include <audio/mixer/LCRControl.hpp>
#include <audio/mixer/PanControl.hpp>
#include <audio/mixer/MixerControls.hpp>
#include <audio/mixer/MixerControlsIds.hpp>
#include <control/ControlRow.hpp>
#include <control/Control.hpp>
#include <control/EnumControl.hpp>
#include <control/BooleanControl.hpp>

#include <audio/mixer/RouteControl.hpp>

#include <cmath>

using namespace ctoot::audio::mixer;
using namespace ctoot::audio::core;
using namespace ctoot::audio::fader;
using namespace ctoot::control;

using namespace std;

MixControls::MixControls(weak_ptr<MixerControls> mixerControls, int stripId, weak_ptr<BusControls> busControls, bool master) : AudioControls(busControls.lock()->getId(), busControls.lock()->getName(), -1)
{
	//this->mixerControls = mixerControls;
	this->wMixerControls = mixerControls;
	auto lMixerControls = wMixerControls.lock();
	this->busControls = busControls;
	this->master = master;
	gainControl = shared_ptr<FaderControl>(lMixerControls->createFaderControl(false));
	auto busId = busControls.lock()->getId();
	ChannelFormat* format = getChannelFormat();
	channelCount = format->getCount();
	if (format->getLFE() >= 0) {
	}

	if (channelCount >= 4) {
		frontRearControl = make_shared<FrontRearControl>();
		add(frontRearControl);
		derive(frontRearControl.get());
	}

	if (format->getCenter() >= 0 && channelCount > 1) {
	}
	if (channelCount > 1) {
		if (stripId == MixerControlsIds::CHANNEL_STRIP) {
			lcrControl = make_shared<PanControl>();
			add(lcrControl);
		}
		else {
			lcrControl = make_shared<BalanceControl>();
			add(lcrControl);
		}
		derive(lcrControl.get());
	}

	auto enables = make_shared<ControlRow>();
	if (master) {
		enables->add(shared_ptr<Control>(busControls.lock()->getSoloIndicator()));
	}
	else {
		soloControl = shared_ptr<BooleanControl>(createSoloControl());
		enables->add(soloControl);
		derive(soloControl.get());
		soloControl->addObserver(busControls.lock().get());
	}

	muteControl = shared_ptr<BooleanControl>(createMuteControl());
	enables->add(muteControl);
	derive(muteControl.get());
	add(enables);
	
	/* Compared to toot (Java), this method was moved to be called from outside the constructor so it calls the derived class's overrided function.
	if (busId == MixerControlsIds::MAIN_BUS) {
		auto routeControl = createRouteControl(strpId);
		if (routeControl != nullptr) {
			add(routeControl);
		}
	}
	*/

	auto muted = ((busId == MixerControlsIds::AUX_BUS || busId == MixerControlsIds::FX_BUS) && !master);
	gainControl = shared_ptr<FaderControl>(lMixerControls->createFaderControl(muted));
	add(gainControl);
	derive(gainControl.get());
}

float& MixControls::HALF_ROOT_TWO()
{
	return HALF_ROOT_TWO_;
}
float MixControls::HALF_ROOT_TWO_ = static_cast< float >((sqrt(2) / 2));

void MixControls::derive(Control* c)
{
	switch (c->getId()) {
	case MixControlIds::MUTE:
		mute = muteControl->getValue();
		break;
	case MixControlIds::SOLO:
		solo = soloControl->getValue();
		break;
	case MixControlIds::GAIN:
		gain = gainControl->getGain();
		break;
	case MixControlIds::LCR:
		left = lcrControl->getLeft();
		right = lcrControl->getRight();
		break;
	case MixControlIds::FRONT_SURROUND:
		front = frontRearControl->getFront();
		rear = frontRearControl->getRear();
		break;
	}
}

BooleanControl* MixControls::getSoloControl()
{
    return soloControl.get();
}

BooleanControl* MixControls::getMuteControl()
{
    return muteControl.get();
}


bool MixControls::isMaster()
{
    return master;
}


ChannelFormat* MixControls::getChannelFormat()
{
    return busControls.lock()->getChannelFormat();
}

bool MixControls::canBypass()
{
    return false;
}

bool MixControls::isAlwaysVertical()
{
    return true;
}

bool MixControls::canBeDeleted()
{
    return false;
}


bool MixControls::hasPresets()
{
    return false;
}

bool MixControls::isSolo()
{
    return &soloControl == nullptr ? hasSolo() : solo;
}

bool MixControls::isMute()
{
    return mute;
}

bool MixControls::isEnabled()
{
	return !(isMute() || isSolo() != hasSolo());
}

bool MixControls::hasSolo()
{
    return busControls.lock()->hasSolo();
}

float MixControls::getGain()
{
    return gain;
}

void MixControls::getChannelGains(vector<float>* dest)
{
	float r;
	float f;
	switch (channelCount) {
	case 6:
		//dest[5] = gain;
		//dest[4] = gain;
	case 4:
		r = gain * rear;
		//dest[3] = r * right;
		//dest[2] = r * left;
		f = gain * front;
		//dest[1] = f * right;
		//dest[0] = f * left;
		break;
	case 2:
		(*dest)[1] = gain * right;
		(*dest)[0] = gain * left;
		break;
	case 1:
		//dest[0] = gain;
		break;
	}
}

float MixControls::getSmoothingFactor()
{
    return wMixerControls.lock()->getSmoothingFactor();
}

EnumControl* MixControls::createRouteControl(int stripId)
{
	return nullptr;
}


BooleanControl* MixControls::createMuteControl()
{
    auto c = new BooleanControl(MixControlIds::MUTE, "Mute", false);
	string name = c->getName();
	name = name.substr(0, 1);
    c->setAnnotation(name);
    return c;
}

BooleanControl* MixControls::createSoloControl()
{
    auto c = new BooleanControl(MixControlIds::SOLO, "Solo", false);
    c->setAnnotation(c->getName().substr(0, 1));
    return c;
}

string MixControls::getName()
{
    return Control::getName();
}

MixControls::~MixControls() {
	soloControl.reset();
	muteControl.reset();
	gainControl.reset();
	lcrControl.reset();
	frontRearControl.reset();
	busControls.reset();
}
