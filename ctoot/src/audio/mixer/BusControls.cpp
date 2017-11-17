#include <audio/mixer/BusControls.hpp>

#include <audio/core/ChannelFormat.hpp>
#include <control/BooleanControl.hpp>

using namespace ctoot::audio::core;
using namespace ctoot::control;
using namespace ctoot::audio::mixer;
using namespace std;

BusControls::BusControls(int id, string name, ChannelFormat* format) : AudioControls(id, name, -1)
{
	soloCount = 0;
	soloIndicator = new SoloIndicator();
	channelFormat = format;
}

bool BusControls::hasSolo()
{
    return soloCount > 0;
}

ChannelFormat* BusControls::getChannelFormat()
{
    return channelFormat;
}

void BusControls::update(moduru::observer::Observable* o, boost::any a)
{
	try {
		auto c = boost::any_cast<BooleanControl*>(a);
		if (c->getName().compare("Solo") == 0) {
			soloCount += c->getValue() ? 1 : -1;
			soloIndicator->setValue(hasSolo());
		}
	}
	catch (boost::bad_any_cast e) {
		e.what();
	}
}

bool BusControls::canBypass()
{
    return false;
}

string BusControls::toString()
{
	return getName() + " Bus";
}

SoloIndicator* BusControls::getSoloIndicator() {
	return soloIndicator;
}

BusControls::~BusControls() {
	if (soloIndicator != nullptr) {
		//delete soloIndicator; // is already deleted by ControlRow/CompoundControl
	}
}
