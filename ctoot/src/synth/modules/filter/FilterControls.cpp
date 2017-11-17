#include <synth/modules/filter/FilterControls.hpp>
#include <control/Control.hpp>
#include <control/FloatControl.hpp>
#include <control/LinearLaw.hpp>
//#include <misc/Localisation.hpp>
#include <synth/modules/filter/FilterControlIds.hpp>

using namespace ctoot::synth::modules::filter;
using namespace std;

FilterControls::FilterControls(int id, int instanceIndex, string name, int idOffset) : CompoundControl(id, instanceIndex, name)
{
	this->idOffset = idOffset;
	createControls();
	deriveSampleRateIndependentVariables();
	deriveSampleRateDependentVariables();
}

ctoot::control::ControlLaw* FilterControls::SEMITONE_LAW_ = new ctoot::control::LinearLaw(-48, 96, "semitones");

void FilterControls::derive(ctoot::control::Control* c)
{
	switch (c->getId() - idOffset) {
	case FilterControlIds::FREQUENCY:
		cutoff = deriveCutoff();
		break;
	case FilterControlIds::RESONANCE:
		resonance = deriveResonance();
		break;
	}
}

void FilterControls::createControls()
{
	cutoffControl = createCutoffControl();
	add(shared_ptr<Control>(cutoffControl));
	resonanceControl = createResonanceControl();
	add(shared_ptr<Control>(resonanceControl));
}

void FilterControls::deriveSampleRateIndependentVariables()
{
    resonance = deriveResonance();
    cutoff = deriveCutoff();
}

float FilterControls::deriveResonance()
{
    return resonanceControl->getValue();
}

void FilterControls::deriveSampleRateDependentVariables()
{
}

float FilterControls::deriveCutoff()
{
    return cutoffControl->getValue();
}

ctoot::control::FloatControl* FilterControls::createCutoffControl()
{
    return new ctoot::control::FloatControl(FilterControlIds::FREQUENCY + idOffset, "Cutoff", *SEMITONE_LAW_, 1.0f, 0.0f);
}

ctoot::control::FloatControl* FilterControls::createResonanceControl()
{
    return new ctoot::control::FloatControl(FilterControlIds::RESONANCE + idOffset, "Resonance", *ctoot::control::LinearLaw::UNITY(), 0.01f, 0.25f);
}

float FilterControls::getCutoff()
{
    return cutoff;
}

float FilterControls::getResonance()
{
    return resonance;
}

void FilterControls::setSampleRate(int rate)
{
    if(sampleRate != rate) {
        sampleRate = rate;
        deriveSampleRateDependentVariables();
    }
}

FilterControls::~FilterControls() {
	cutoffControl = nullptr;
	resonanceControl = nullptr;
}
