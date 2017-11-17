#include <audio/core/AudioControls.hpp>
#include <control/BypassControl.hpp>

using namespace ctoot::audio::core;
using namespace ctoot::control;
using namespace std;

AudioControls::AudioControls(int id, string name) : AudioControls(id, name, 127) {
}

AudioControls::AudioControls(int id, string name, int bypassId) : CompoundControl(id, name)
{
	// can't call derived class virtual function in ctor, so we do a bypassId check instead
	//if (canBypass()) {
	if (bypassId != -1) {
		bypassControl = new BypassControl(bypassId);
		try {
			add(shared_ptr<Control>(bypassControl));
		}
		catch (bad_cast e) {
			e.what();
		}
	}
}

bool AudioControls::hasOrderedFrequencies()
{
    return false;
}

bool AudioControls::canBeMinimized()
{
    return true;
}

bool AudioControls::canBypass()
{
    return true;
}

void AudioControls::setBypassed(bool state)
{
	if (canBypass() && bypassControl != nullptr) {
		bypassControl->setValue(state);
	}
}

bool AudioControls::isBypassed()
{
    if(bypassControl == nullptr) return false;
    return bypassControl->getValue();
}

ctoot::control::BooleanControl* AudioControls::getBypassControl()
{
    return bypassControl;
}

string AudioControls::getPersistenceDomain()
{
	return "audio";
}

AudioControls::~AudioControls() {
	if (bypassControl != nullptr) {
		delete bypassControl;
		bypassControl = nullptr;
	}
}