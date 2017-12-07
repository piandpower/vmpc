#include <controls/sampler/window/ZoomControls.hpp>

#include <lcdgui/Wave.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <ui/sampler/window/ZoomGui.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>


using namespace mpc::controls::sampler::window;
using namespace std;

ZoomControls::ZoomControls(mpc::Mpc* mpc)
	: AbstractSamplerControls(mpc)
{
}

void ZoomControls::init()
{
    super::init();
	auto lSound = sound.lock();
    sampleLength = lSound->getSampleData()->size();
    if(!lSound->isMono()) sampleLength /= 2;
}

void ZoomControls::function(int i)
{
	super::function(i);
	auto lSampler = sampler.lock();;
	vector<int> zone;
	switch (i) {
	case 1:
		ls.lock()->getFineWave().lock()->zoomPlus();
		break;
	case 2:
		ls.lock()->getFineWave().lock()->zoomMinus();
		break;
	case 4:
		zone = vector<int>{ soundGui->getZoneStart(soundGui->getZoneNumber()), soundGui->getZoneEnd(soundGui->getZoneNumber()) };
		lSampler->playX(soundGui->getPlayX(), &zone);
		break;
	}
}

void ZoomControls::turnWheel(int i)
{
    init();
    	auto lSound = sound.lock();
	auto startEndLength = static_cast< int >((lSound->getEnd() - lSound->getStart()));
    auto loopLength = static_cast< int >((lSound->getEnd() - lSound->getLoopTo()));
	if (param.compare("looplngth") == 0) {
		zoomGui->setLoopLngthFix(i > 0);
	}
	else if (param.compare("lngth") == 0) {
		if (lSound->getEnd() + i > sampleLength) {
			return;
		}
		lSound->setEnd(lSound->getEnd() + i);
	}
	else if (param.compare("to") == 0) {
		if (!zoomGui->isLoopLngthFix() && lSound->getEnd() - (lSound->getLoopTo() + i) < 0) {
			return;
		}
		auto highestLoopTo = sampleLength - 1;
		if (zoomGui->isLoopLngthFix()) {
			highestLoopTo -= loopLength;
			if (lSound->getLoopTo() + i > highestLoopTo) {
				return;
			}
		}
		lSound->setLoopTo(lSound->getLoopTo() + i);
		if (zoomGui->isLoopLngthFix()) {
			lSound->setEnd(lSound->getLoopTo() + loopLength);
		}
	}
	else if (param.compare("start") == 0) {
		if (csn.find("zone") == string::npos) {
			auto highestStartPos = sampleLength - 1;
			auto length = lSound->getEnd() - lSound->getStart();
			if (zoomGui->isSmplLngthFix()) {
				highestStartPos -= startEndLength;
				if (lSound->getStart() + i > highestStartPos) {
					return;
				}
			}
			lSound->setStart(lSound->getStart() + i);
			if (zoomGui->isSmplLngthFix()) {
				lSound->setEnd(lSound->getStart() + length);
			}
		}
		else {
			soundGui->setZoneStart(soundGui->getZoneNumber(), soundGui->getZoneStart(soundGui->getZoneNumber()) + i);
		}
	}
	else if (param.compare("end") == 0 && csn.compare("endfine") == 0) {
		lSound->setEnd(lSound->getEnd() + i);
		if (zoomGui->isSmplLngthFix()) {
			lSound->setStart(lSound->getEnd() - startEndLength);
		}
	}
	else if (param.compare("end") == 0 && csn.compare("loopendfine") == 0) {
		lSound->setEnd(lSound->getEnd() + i);
		if (zoomGui->isLoopLngthFix()) {
			lSound->setLoopTo(lSound->getEnd() - loopLength);
		}
	}
	else if (param.compare("end") == 0 && csn.compare("zoneendfine") == 0) {
		soundGui->setZoneEnd(soundGui->getZoneNumber(), soundGui->getZoneEnd(soundGui->getZoneNumber()) + i);
	}
	else if (param.compare("smpllngth") == 0) {
		zoomGui->setSmplLngthFix(i > 0);
	}
	else if (param.compare("playx") == 0) {
		soundGui->setPlayX(soundGui->getPlayX() + 1);
	}
}
