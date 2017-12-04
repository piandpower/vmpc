#include <ui/sampler/ZoneObserver.hpp>

#include <Util.hpp>
#include <Mpc.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/TwoDots.hpp>
#include <lcdgui/Wave.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>
#include <ctootextensions/MpcSoundOscillatorControls.hpp>

using namespace mpc::ui::sampler;
using namespace std;

ZoneObserver::ZoneObserver(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	sampler = mpc->getSampler();
	playXNames = { "ALL", "ZONE", "BEFOR ST", "BEFOR TO", "AFTR END" };
	soundGui = mpc->getUis().lock()->getSoundGui();
	soundGui->addObserver(this);
	auto ls = mpc->getLayeredScreen().lock();
	csn = ls->getCurrentScreenName();
	auto lSampler = sampler.lock();
	if (lSampler->getSoundCount() != 0) {
		sound = lSampler->getSound(soundGui->getSoundIndex());
		auto lSound = sound.lock();
		lSound->addObserver(this);
		lSound->getMsoc()->addObserver(this);
	}
	twoDots = ls->getTwoDots();
	twoDots->setVisible(0, true);
	twoDots->setVisible(1, true);
	twoDots->setVisible(2, false);
	twoDots->setVisible(3, false);
	wave= ls->getWave();
	wave->Hide(false);
	if (csn.compare("zone") == 0) {
		twoDots->Hide(false);
		sndField = ls->lookupField("snd");
		playXField = ls->lookupField("playx");
		stField = ls->lookupField("st");
		endField = ls->lookupField("end");
		zoneField = ls->lookupField("zone");
		dummyField = ls->lookupField("dummy");
		stField.lock()->setSize(8 * 6 * 2 + 2, 18);
		endField.lock()->setSize(8 * 6 * 2 + 2, 18);
		displaySnd();
		if (lSampler->getSoundCount() != 0) {
			dummyField.lock()->setFocusable(false);
			soundGui->initZones(lSampler->getSound(soundGui->getSoundIndex()).lock()->getLastFrameIndex() + 1);
			wave->setSelection(soundGui->getZoneStart(soundGui->getZoneNumber()), soundGui->getZoneEnd(soundGui->getZoneNumber()));
		}
		else {
			sndField.lock()->setFocusable(false);
			playXField.lock()->setFocusable(false);
			stField.lock()->setFocusable(false);
			endField.lock()->setFocusable(false);
			zoneField.lock()->setFocusable(false);
		}
		displayPlayX();
		displaySt();
		displayEnd();
		displayZone();
	}
	else if (csn.compare("numberofzones") == 0) {
		twoDots->Hide(true);
		numberOfZonesField = ls->lookupField("numberofzones");
		displayNumberOfZones();
	}
}

void ZoneObserver::displaySnd()
{
	auto lSampler = sampler.lock();
	if (lSampler->getSoundCount() != 0) {
		waveformLoadData();
		mpc->getLayeredScreen().lock()->setFocus("snd");
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
		mpc->getLayeredScreen().lock()->setFocus("dummy");
	}
}

void ZoneObserver::displayPlayX()
{
	playXField.lock()->setText(playXNames[soundGui->getPlayX()]);
}

void ZoneObserver::displaySt()
{
	if (sampler.lock()->getSoundCount() != 0) {
		stField.lock()->setTextPadded(soundGui->getZoneStart(soundGui->getZoneNumber()), " ");
	}
	else {
		stField.lock()->setText("       0");
	}
}

void ZoneObserver::displayEnd()
{
	if (sampler.lock()->getSoundCount() != 0) {
		endField.lock()->setTextPadded(soundGui->getZoneEnd(soundGui->getZoneNumber()), " ");
	}
	else {
		endField.lock()->setText("       0");
	}
}

void ZoneObserver::displayZone()
{
	if (sampler.lock()->getSoundCount() == 0) {
		zoneField.lock()->setText("1");
		return;
	}
	zoneField.lock()->setText(to_string(soundGui->getZoneNumber() + 1));
}

void ZoneObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	string s = boost::any_cast<string>(arg);
	if (s.compare("soundnumber") == 0) {
		displaySnd();
		soundGui->initZones(sampler.lock()->getSound(soundGui->getSoundIndex()).lock()->getLastFrameIndex() + 1);
		displaySt();
		displayEnd();
		waveformLoadData();
		wave->setSelection(soundGui->getZoneStart(soundGui->getZoneNumber()), soundGui->getZoneEnd(soundGui->getZoneNumber()));
	}
	else if (s.compare("zone") == 0) {
		displayZone();
		displaySt();
		displayEnd();
		wave->setSelection(soundGui->getZoneStart(soundGui->getZoneNumber()), soundGui->getZoneEnd(soundGui->getZoneNumber()));
	}
	else if (s.compare("playx") == 0) {
		displayPlayX();
	}
	else if (s.compare("numberofzones") == 0) {
		displayNumberOfZones();
	}
}

void ZoneObserver::displayNumberOfZones()
{
	numberOfZonesField.lock()->setText(to_string(soundGui->getNumberOfZones()));
}

void ZoneObserver::waveformLoadData()
{
	auto lSound = sound.lock();
	auto sampleData = lSound->getSampleData();
	wave->setSampleData(sampleData, lSound->isMono(), soundGui->getView());
}

ZoneObserver::~ZoneObserver() {
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
