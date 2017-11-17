#include <audio/mixer/PostFadeMixControls.hpp>
#include <audio/mixer/MainMixControls.hpp>

using namespace std;

ctoot::audio::mixer::PostFadeMixControls::PostFadeMixControls(weak_ptr<MixerControls> mixerControls, int stripId, weak_ptr<BusControls> busControls, MainMixControls* mainMixControls) : MixControls(mixerControls, stripId, busControls, false)
{
	this->mainMixControls = mainMixControls;
}

bool ctoot::audio::mixer::PostFadeMixControls::isEnabled()
{
    return super::isEnabled() && !mainMixControls->isMute();
}

float ctoot::audio::mixer::PostFadeMixControls::getGain()
{
    return super::getGain() * mainMixControls->getGain();
}

ctoot::audio::mixer::PostFadeMixControls::~PostFadeMixControls() {
	mainMixControls = nullptr;
}