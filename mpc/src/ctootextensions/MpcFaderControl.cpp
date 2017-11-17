#include <ctootextensions/MpcFaderControl.hpp>

#include <audio/mixer/MixControlIds.hpp>
#include <control/LinearLaw.hpp>

using namespace mpc::ctootextensions;

MpcFaderControl::MpcFaderControl(bool muted) : FaderControl(ctoot::audio::mixer::MixControlIds::GAIN, mpcFaderLaw_, muted ? 0 : 100)
{
	gain = muted ? 0.0f : 1.0f;
}
ctoot::control::ControlLaw MpcFaderControl::mpcFaderLaw_ = ctoot::control::LinearLaw(0, 100, "");

void MpcFaderControl::setValue(float value)
{
    gain = value * 0.01f;
    FaderControl::setValue(value);
}

MpcFaderControl::~MpcFaderControl() {
}
