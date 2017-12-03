#include <ui/sampler/TrimObserver.hpp>

#include <Util.hpp>
#include <Mpc.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/TwoDots.hpp>
#include <lcdgui/Wave.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>
#include <ctootextensions/MpcSoundOscillatorControls.hpp>

using namespace mpc::ui::sampler;
using namespace std;

TrimObserver::TrimObserver(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	sampler = mpc->getSampler();
	playXNames = vector<string>{ "ALL", "ZONE", "BEFOR ST", "BEFOR TO", "AFTR END" };
	soundGui = mpc->getUis().lock()->getSoundGui();
	soundGui->addObserver(this);
	samplerGui = mpc->getUis().lock()->getSamplerGui();
	samplerGui->addObserver(this);

	auto lSampler = sampler.lock();
	if (lSampler->getSoundCount() != 0) {
		sound = lSampler->getSound(soundGui->getSoundIndex());
		auto lSound = sound.lock();
		lSound->addObserver(this);
		lSound->getMsoc()->addObserver(this);
	}
	auto ls = mpc->getLayeredScreen().lock();
	wave = ls->getWave();
	wave->Hide(false);
	twoDots = ls->getTwoDots();
	twoDots->Hide(false);
	twoDots->setVisible(0, true);
	twoDots->setVisible(1, true);
	twoDots->setVisible(2, false);
	twoDots->setVisible(3, false);
	sndField = ls->lookupField("snd");
	playXField = ls->lookupField("playx");
	stField = ls->lookupField("st");
	endField = ls->lookupField("end");
	viewField = ls->lookupField("view");
	dummyField = ls->lookupField("dummy");
	stField.lock()->setSize(8 * 6 * 2 + 2, 18);
	endField.lock()->setSize(8 * 6 * 2 + 2, 18);
	displaySnd();
	displayPlayX();
	displaySt();
	displayEnd();
	if (lSampler->getSoundCount() != 0) {
		auto lSound = sound.lock();
		dummyField.lock()->setFocusable(false);
		waveformLoadData();
		wave->setSelection(lSound->getStart(), lSound->getEnd());
	}
	else {
		wave->setSampleData(nullptr, false, 0);
		sndField.lock()->setFocusable(false);
		playXField.lock()->setFocusable(false);
		stField.lock()->setFocusable(false);
		endField.lock()->setFocusable(false);
		viewField.lock()->setFocusable(false);
	}
	displayView();
}

void TrimObserver::displaySnd()
{
	auto lSampler = sampler.lock();
	if (lSampler->getSoundCount() != 0) {
		mpc->getLayeredScreen().lock()->setFocus(sndField.lock()->getName(), 0);
		auto lSound = sound.lock();
		lSound->deleteObserver(this);
		lSound->getMsoc()->deleteObserver(this);
		sound = lSampler->getSound(soundGui->getSoundIndex());
		lSound = sound.lock();
		lSound->addObserver(this);
		lSound->getMsoc()->addObserver(this);
		auto sampleName = lSound->getName();
		if (!lSound->isMono()) {
			sampleName = moduru::lang::StrUtil::padRight(sampleName, " ", 16) + "(ST)";
		}
		sndField.lock()->setText(sampleName);
	}
	else {
		sndField.lock()->setText("(no sound)");
		mpc->getLayeredScreen().lock()->setFocus("dummy", 0);
	}
}

void TrimObserver::displayPlayX()
{
    playXField.lock()->setText(playXNames[soundGui->getPlayX()]);
}

void TrimObserver::displaySt()
{
	auto lSampler = sampler.lock();
	if (lSampler->getSoundCount() != 0) {
		stField.lock()->setTextPadded(sound.lock()->getStart(), " ");
	}
	else {
		stField.lock()->setTextPadded("0", " ");
	}
}

void TrimObserver::displayEnd()
{
	auto lSampler = sampler.lock();
	if (lSampler->getSoundCount() != 0) {
		endField.lock()->setTextPadded(sound.lock()->getEnd(), " ");
	}
	else {
		endField.lock()->setTextPadded("0", " ");
	}
}

void TrimObserver::displayView()
{
	if (soundGui->getView() == 0) {
		viewField.lock()->setText("LEFT");
	}
	else {
		viewField.lock()->setText("RIGHT");
	}
}

void TrimObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	string s = boost::any_cast<string>(arg);
	if (s.compare("soundnumber") == 0) {
		displaySnd();
		displaySt();
		displayEnd();
		waveformLoadData();
		auto lSound = sound.lock();
		wave->setSelection(lSound->getStart(), lSound->getEnd());
		soundGui->initZones(lSound->getLastFrameIndex() + 1);
	}
	else if (s.compare("start") == 0) {
		displaySt();
		auto lSound = sound.lock();
		wave->setSelection(lSound->getStart(), lSound->getEnd());
	}
	else if (s.compare("end") == 0) {
		displayEnd();
		auto lSound = sound.lock();
		wave->setSelection(lSound->getStart(), lSound->getEnd());
	}
	else if (s.compare("view") == 0) {
		displayView();
		waveformLoadData();
		auto lSound = sound.lock();
		wave->setSelection(lSound->getStart(), lSound->getEnd());
	}
	else if (s.compare("playx") == 0) {
		displayPlayX();
	}
}

void TrimObserver::waveformLoadData()
{
	auto lSound = sound.lock();
	auto sampleData = lSound->getSampleData();
	wave->setSampleData(lSound->getSampleData(), lSound->isMono(), soundGui->getView());
}

TrimObserver::~TrimObserver() {
	wave->setSampleData(nullptr, false, 0);
	wave->Hide(true);
	twoDots->Hide(true);
	samplerGui->deleteObserver(this);
	soundGui->deleteObserver(this);
	auto lSound = sound.lock();
	if (lSound) {
		lSound->deleteObserver(this);
		lSound->getMsoc()->deleteObserver(this);
	}
}
