#include <ui/sampler/SoundObserver.hpp>

#include <Util.hpp>
#include <Mpc.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/Wave.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>

#include <lang/StrUtil.hpp>

using namespace mpc::ui::sampler;
using namespace std;

SoundObserver::SoundObserver(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	sampler = mpc->getSampler();
	convertNames = vector<string>{ "STEREO TO MONO", "RE-SAMPLE" };
	qualityNames = vector<string>{ "LOW", "MED", "HIGH" };
	bitNames = vector<string>{ "16", "12", "8" };
	auto ls = mpc->getLayeredScreen().lock();
	csn = ls->getCurrentScreenName();
	soundGui = mpc->getUis().lock()->getSoundGui();
	soundGui->addObserver(this);
	if (csn.compare("sound") == 0) {
		soundNameField = ls->lookupField("soundname");
		typeLabel = ls->lookupLabel("type");
		rateLabel = ls->lookupLabel("rate");
		sizeLabel = ls->lookupLabel("size");
		displaySoundName();
		displayType();
		displayRate();
		displaySize();
	}
	else if (csn.compare("deletesound") == 0) {
		sndField = ls->lookupField("snd");
		displayDeleteSoundSnd();
	}
	else if (csn.compare("convertsound") == 0) {
		convertField = ls->lookupField("convert");
		displayConvert();
	}
	else if (csn.compare("resample") == 0) {
		newFsField = ls->lookupField("newfs");
		qualityField = ls->lookupField("quality");
		newBitField = ls->lookupField("newbit");
		newNameField = ls->lookupField("newname");
		displayNewFs();
		displayQuality();
		displayNewBit();
		displayNewName();
	}
	else if (csn.compare("stereotomono") == 0) {
		stereoSourceField = ls->lookupField("stereosource");
		newLNameField = ls->lookupField("newlname");
		newRNameField = ls->lookupField("newrname");
		displayStereoSource();
		displayNewLName();
		displayNewRName();
	}
	else if (csn.compare("monotostereo") == 0) {
		lSourceField = ls->lookupField("lsource");
		rSourceField = ls->lookupField("rsource");
		newStNameField = ls->lookupField("newstname");
		displayLSource();
		displayRSource();
		displayNewStName();
	}
	else if (csn.compare("copysound") == 0) {
		sndField = ls->lookupField("snd");
		newNameField = ls->lookupField("newname");
		displaySnd();
		displayNewName();
	}
}

void SoundObserver::displaySnd()
{
	sndField.lock()->setText(sampler.lock()->getSoundName(soundGui->getSoundIndex()));
}

void SoundObserver::displayLSource()
{
	auto lSampler = sampler.lock();
	auto ls = mpc->getLayeredScreen().lock();
	lSourceField.lock()->setText(lSampler->getSoundName(soundGui->getSoundIndex()));
	if (lSampler->getSound(soundGui->getSoundIndex()).lock()->isMono() && lSampler->getSound(soundGui->getRSource()).lock()->isMono()) {
		ls->drawFunctionKeys("monotostereo");
		ls->getWave().lock()->SetDirty();
		ls->getLayer(0)->setDirty();
		ls->getLayer(1)->setDirty();
		ls->getLayer(2)->setDirty();
	}
	else {
		ls->drawFunctionKeys("convertnodoit");
		ls->getWave().lock()->SetDirty();
		ls->getLayer(0)->setDirty();
		ls->getLayer(1)->setDirty();
		ls->getLayer(2)->setDirty();
	}
}

void SoundObserver::displayRSource()
{
	auto lSampler = sampler.lock();
	auto ls = mpc->getLayeredScreen().lock();
	rSourceField.lock()->setText(lSampler->getSoundName(soundGui->getRSource()));
	if (lSampler->getSound(soundGui->getSoundIndex()).lock()->isMono() && lSampler->getSound(soundGui->getRSource()).lock()->isMono()) {
		ls->drawFunctionKeys("monotostereo");
		ls->getWave().lock()->SetDirty();
		ls->getLayer(0)->setDirty();
		ls->getLayer(1)->setDirty();
		ls->getLayer(2)->setDirty();
	}
	else {
		ls->drawFunctionKeys("convertnodoit");
		ls->getWave().lock()->SetDirty();
		ls->getLayer(0)->setDirty();
		ls->getLayer(1)->setDirty();
		ls->getLayer(2)->setDirty();
	}
}

void SoundObserver::displayNewStName()
{
    newStNameField.lock()->setText(soundGui->getNewStName());
}

void SoundObserver::displayStereoSource()
{
	auto lSampler = sampler.lock();
	auto ls = mpc->getLayeredScreen().lock();
	stereoSourceField.lock()->setText(lSampler->getSoundName(soundGui->getSoundIndex()));
	if (lSampler->getSound(soundGui->getSoundIndex()).lock()->isMono()) {
		ls->drawFunctionKeys("convertnodoit");
		ls->getWave().lock()->SetDirty();
		ls->getLayer(0)->setDirty();
		ls->getLayer(1)->setDirty();
		ls->getLayer(2)->setDirty();
	}
	else {
		ls->drawFunctionKeys("stereotomono");
		ls->getWave().lock()->SetDirty();
		ls->getLayer(0)->setDirty();
		ls->getLayer(1)->setDirty();
		ls->getLayer(2)->setDirty();
	}
}

void SoundObserver::displayNewLName()
{
    newLNameField.lock()->setText(soundGui->getNewLName());
}

void SoundObserver::displayNewRName()
{
    newRNameField.lock()->setText(soundGui->getNewRName());
}

void SoundObserver::displayNewFs()
{
    newFsField.lock()->setText(to_string(soundGui->getNewFs()));
}

void SoundObserver::displayQuality()
{
    qualityField.lock()->setText(qualityNames[soundGui->getQuality()]);
}

void SoundObserver::displayNewBit()
{
    newBitField.lock()->setText(bitNames[soundGui->getNewBit()]);
}

void SoundObserver::displayNewName()
{
    newNameField.lock()->setText(soundGui->getNewName());
}

void SoundObserver::displayConvert()
{
	convertField.lock()->setText(convertNames[soundGui->getConvert()]);
	if (soundGui->getConvert() == 0 && sampler.lock()->getSound(soundGui->getSoundIndex()).lock()->isMono()) {
		convertField.lock()->setText("MONO TO STEREO");
	}
}

void SoundObserver::displayDeleteSoundSnd()
{
    sndField.lock()->setText(sampler.lock()->getSoundName(soundGui->getSoundIndex()));
}

void SoundObserver::displaySoundName()
{
	if (soundGui->getSoundIndex() == -1) {
		soundNameField.lock()->setText("");
		return;
	}
	soundNameField.lock()->setText(sampler.lock()->getSoundName(soundGui->getSoundIndex()));
}

void SoundObserver::displayType()
{
	if (soundGui->getSoundIndex() == -1) {
		typeLabel.lock()->setText("");
		return;
	}
	typeLabel.lock()->setText("Type:" + sampler.lock()->getSound(soundGui->getSoundIndex()).lock()->isMono() ? "MONO" : "STEREO");
}

void SoundObserver::displayRate()
{
	if (soundGui->getSoundIndex() == -1) {
		rateLabel.lock()->setText("");
		return;
	}
	rateLabel.lock()->setText("Rate: " + to_string(sampler.lock()->getSound(soundGui->getSoundIndex()).lock()->getSampleRate()) + "Hz");
}

void SoundObserver::displaySize()
{
	if (soundGui->getSoundIndex() == -1) {
		sizeLabel.lock()->setText("");
		return;
	}
	sizeLabel.lock()->setText("Size:" + moduru::lang::StrUtil::padLeft(to_string(sampler.lock()->getSound(soundGui->getSoundIndex()).lock()->getSampleData()->size() / 500), " ", 4) + "kbytes");
}

void SoundObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	string s = boost::any_cast<string>(arg);
	if (s.compare("soundnumber") == 0) {
		if (csn.compare("sound") == 0) {
			displaySoundName();
			displayType();
			displayRate();
			displaySize();
		}
		else if (csn.compare("deletesound") == 0) {
			displayDeleteSoundSnd();
		}
		else if (csn.compare("stereotomono") == 0) {
			displayStereoSource();
		}
		else if (csn.compare("monotostereo") == 0) {
			displayLSource();
		}
		else if (csn.compare("copysound") == 0) {
			displaySnd();
		}
	}
	else if (s.compare("convert") == 0) {
		displayConvert();
	}
	else if (s.compare("newfs") == 0) {
		displayNewFs();
	}
	else if (s.compare("newbit") == 0) {
		displayNewBit();
	}
	else if (s.compare("quality") == 0) {
		displayQuality();
	}
	else if (s.compare("newname") == 0) {
		displayNewName();
	}
	else if (s.compare("stereosource") == 0) {
		displayStereoSource();
	}
	else if (s.compare("rsource") == 0) {
		if (csn.compare("monotostereo") == 0) {
			displayRSource();
		}
	}
}

SoundObserver::~SoundObserver() {
	soundGui->deleteObserver(this);
}
