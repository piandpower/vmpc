#include "LoopObserver.hpp"

#include <Mpc.hpp>
#include <Util.hpp>
#include <disk/AbstractDisk.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/TwoDots.hpp>
#include <lcdgui/Wave.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>
#include <ctootextensions/MpcSoundOscillatorControls.hpp>

using namespace mpc::ui::sampler;
using namespace std;

LoopObserver::LoopObserver(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	playXNames = vector<string>{ "ALL", "ZONE", "BEFOR ST", "BEFOR TO", "AFTR END" };
	this->sampler = sampler;
	soundGui = mpc->getUis().lock()->getSoundGui();
	soundGui->addObserver(this);
	auto lSampler = sampler.lock();
	if (lSampler->getSoundCount() != 0) {
		sound = lSampler->getSound(soundGui->getSoundIndex());
		auto lSound = sound.lock();
		lSound->addObserver(this);
		lSound->getMsoc()->addObserver(this);
	}
	auto ls = mpc->getLayeredScreen().lock();
	twoDots = ls->getTwoDots();
	twoDots->Hide(false);
	twoDots->setVisible(0, true);
	twoDots->setVisible(1, true);
	twoDots->setVisible(2, false);
	twoDots->setVisible(3, false);
	wave = ls->getWave();
	wave->Hide(false);
	sndField = ls->lookupField("snd");
	playXField = ls->lookupField("playx");
	toField = ls->lookupField("to");
	endLengthField = ls->lookupField("endlength");
	endLengthValueField = ls->lookupField("endlengthvalue");
	loopField = ls->lookupField("loop");
	dummyField = ls->lookupField("dummy");
	toField.lock()->setSize(8 * 6 * 2 + 2, 18);
	endLengthValueField.lock()->setSize(8 * 6 * 2 + 2, 18);
	displaySnd();
	displayPlayX();
	displayTo();
	displayEndLength();
	displayEndLengthValue();
	displayLoop();
	if (lSampler->getSoundCount() != 0) {
		dummyField.lock()->setFocusable(false);
		waveformLoadData();
		auto lSound = sound.lock();
		wave->setSelection(lSound->getLoopTo(), lSound->getEnd());
	}
	else {
		wave->setSampleData(nullptr, false, 0);
		sndField.lock()->setFocusable(false);
		playXField.lock()->setFocusable(false);
		toField.lock()->setFocusable(false);
		endLengthField.lock()->setFocusable(false);
		endLengthValueField.lock()->setFocusable(false);
		loopField.lock()->setFocusable(false);
	}
}

void LoopObserver::displaySnd()
{
	auto lSampler = sampler.lock();
	auto ls = mpc->getLayeredScreen().lock();
	if (lSampler->getSoundCount() != 0) {
		ls->setFocus(sndField.lock()->getName());
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
		ls->setFocus("dummy");
	}
}

void LoopObserver::displayPlayX()
{
    playXField.lock()->setText(playXNames[soundGui->getPlayX()]);
}

void LoopObserver::displayTo()
{
	if (sampler.lock()->getSoundCount() != 0) {
		toField.lock()->setTextPadded(sound.lock()->getLoopTo(), " ");
	}
	else {
		toField.lock()->setTextPadded("0", " ");
	}
	if (!soundGui->isEndSelected()) displayEndLengthValue();
}

void LoopObserver::displayEndLength()
{
	endLengthField.lock()->setSize(62, 18);
	endLengthField.lock()->setText(soundGui->isEndSelected() ? "  End" : "Lngth");
	displayEndLengthValue();
}

void LoopObserver::displayEndLengthValue()
{
	if (sampler.lock()->getSoundCount() != 0) {
		if (soundGui->isEndSelected()) {
			endLengthValueField.lock()->setTextPadded(sound.lock()->getEnd(), " ");
		}
		else {
			auto lSound = sound.lock();
			endLengthValueField.lock()->setTextPadded(lSound->getEnd() - lSound->getLoopTo(), " ");
		}
	}
	else {
		endLengthValueField.lock()->setTextPadded("0", " ");
	}
}

void LoopObserver::displayLoop()
{
	if (sampler.lock()->getSoundCount() == 0) {
		loopField.lock()->setText("OFF");
		return;
	}
	loopField.lock()->setText(sound.lock()->isLoopEnabled() ? "ON" : "OFF");
}

void LoopObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	string s = boost::any_cast<string>(arg);
	if (s.compare("soundnumber") == 0) {
		auto lSound = sound.lock();
		displaySnd();
		displayTo();
		displayEndLength();
		waveformLoadData();
		wave->setSelection(lSound->getLoopTo(), lSound->getEnd());
		soundGui->initZones(lSound->getLastFrameIndex() + 1);
	}
	else if (s.compare("loopto") == 0) {
		displayTo();
		auto lSound = sound.lock();
		wave->setSelection(lSound->getLoopTo(), lSound->getEnd());
	}
	else if (s.compare("endlength") == 0) {
		displayEndLength();
	}
	else if (s.compare("end") == 0) {
		displayEndLengthValue();
		auto lSound = sound.lock();
		wave->setSelection(lSound->getLoopTo(), lSound->getEnd());
	}
	else if (s.compare("loopenabled") == 0) {
		displayLoop();
	}
	else if (s.compare("playx") == 0) {
		displayPlayX();
	}
}

void LoopObserver::waveformLoadData()
{
	auto lSound = sound.lock();
	auto sampleData = lSound->getSampleData();
	wave->setSampleData(sampleData, lSound->isMono(), soundGui->getView());
}

LoopObserver::~LoopObserver() {
	wave->setSampleData(nullptr, false, 0);
	wave->Hide(true);
	twoDots->Hide(true);
	soundGui->deleteObserver(this);
	auto lSound = sound.lock();
	if (lSound) {
		lSound->deleteObserver(this);
		lSound->getMsoc()->deleteObserver(this);
	}
}
