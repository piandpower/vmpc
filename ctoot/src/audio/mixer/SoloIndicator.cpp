#include <audio/mixer/SoloIndicator.hpp>
#include <audio/mixer/MixControlIds.hpp>

using namespace ctoot::audio::mixer;

SoloIndicator::SoloIndicator() : BooleanControl(-MixControlIds::SOLO, ("Solo"), false)
{
	indicator = true;
	setAnnotation("S");
}

SoloIndicator::~SoloIndicator() {

}
