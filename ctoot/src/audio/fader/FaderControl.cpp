#include <audio/fader/FaderControl.hpp>
#include <audio/fader/FaderLaw.hpp>

using namespace ctoot::audio::fader;

FaderControl::FaderControl(int id, ctoot::control::ControlLaw law, float initialValue) : LawControl(id, "Level", law, 0.1f, initialValue)
{
}

float FaderControl::getGain()
{
    return gain;
}
FaderLaw* FaderControl::SEMI_LOG()
{
	return new FaderLaw(1024, -10.0f, 10.0f, 0.33f);
}

FaderLaw* FaderControl::LOG()
{
	return new FaderLaw(1024, -20.0f, 15.0f, 0.2f);
}

FaderLaw* FaderControl::BROADCAST()
{
	return new FaderLaw(1024, -30.0f, 15.0f, 0.125f);
}

float FaderControl::ATTENUATION_CUTOFF()
{
	return 100.0f;
}

FaderLaw* FaderControl::defaultLaw()
{
	return FaderControl::BROADCAST();
}

FaderControl::~FaderControl() {

}
