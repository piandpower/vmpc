#include <audio/mixer/PanControl.hpp>
#include <audio/mixer/MixControls.hpp>
#include <control/LinearLaw.hpp>

using namespace ctoot::audio::mixer;

PanControl::PanControl() : LCRControl("Pan", *ctoot::control::LinearLaw::UNITY(), 0.01f, 0.5f)
{
	left = MixControls::HALF_ROOT_TWO();
	right = MixControls::HALF_ROOT_TWO();
}

float PanControl::getLeft()
{
    return left;
}

float PanControl::getRight()
{
    return right;
}

void PanControl::setValue(float value)
{
    left = static_cast< float >(cos(M_PI / 2 * value));
    right = static_cast< float >(sin(M_PI / 2 * value));
	LCRControl::setValue(value);
}
