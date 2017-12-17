#include <controls/sampler/window/KeepOrRetryControls.hpp>

#include <ui/NameGui.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <sampler/NoteParameters.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>

using namespace mpc::controls::sampler::window;
using namespace std;

KeepOrRetryControls::KeepOrRetryControls(mpc::Mpc* mpc) 
	: AbstractSamplerControls(mpc)
{
}

void KeepOrRetryControls::mainScreen() {
	auto lSampler = sampler.lock();
	lSampler->deleteSound(lSampler->getPreviewSound());
	super::mainScreen();
}

void KeepOrRetryControls::function(int i)
{
	auto lSampler = sampler.lock();
	auto lLs = ls.lock();
	switch (i) {
	case 1:
		lSampler->deleteSound(lSampler->getPreviewSound());
		lLs->openScreen("sample");
		break;
	case 3 :
		lSampler->playPreviewSample(0, lSampler->getPreviewSound().lock()->getLastFrameIndex(), 0, 2);
		break;
	case 4:
		lSampler->getLastNp(program.lock().get())->setSoundNumber(lSampler->getSoundCount() - 1);
		soundGui->initZones(lSampler->getPreviewSound().lock()->getLastFrameIndex());
		soundGui->setSoundIndex(lSampler->getSoundCount() - 1, lSampler->getSoundCount());
		lLs->openScreen("sample");
		break;
	}
}

void KeepOrRetryControls::turnWheel(int i)
{
    init();
	auto lLs = ls.lock();
    nameGui->setName(sampler.lock()->getPreviewSound().lock()->getName());
    nameGui->setParameterName(param);
    lLs->openScreen("name");
}
