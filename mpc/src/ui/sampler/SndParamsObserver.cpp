#include <ui/sampler/SndParamsObserver.hpp>

#include <Mpc.hpp>
#include <Util.hpp>
#include <disk/AbstractDisk.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/Label.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>
#include <ctootextensions/MpcSoundOscillatorControls.hpp>

#include <lang/StrUtil.hpp>
#include <cmath>

using namespace mpc::ui::sampler;
using namespace std;

SndParamsObserver::SndParamsObserver(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	sampler = mpc->getSampler();
	
	playXNames = vector<string>{ "ALL", "ZONE", "BEFOR ST", "BEFOR TO", "AFTR END" };
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
	sndField = ls->lookupField("snd");
	playXField = ls->lookupField("playx");
	levelField = ls->lookupField("level");
	tuneField = ls->lookupField("tune");
	beatField = ls->lookupField("beat");
	sampleTempoLabel = ls->lookupLabel("sampletempo");
	newTempoLabel = ls->lookupLabel("newtempo");
	dummyField = ls->lookupField("dummy");
	if (lSampler->getSoundCount() != 0) {
		//dummyField->setFocusable(false);
	}
	displaySnd();
	displayPlayX();
	displayLevel();
	displayTune();
	displayBeat();
	displaySampleAndNewTempo();
}

void SndParamsObserver::displayLevel()
{
	auto lSound = sound.lock();
	if (lSound) {
		levelField.lock()->setText(to_string(lSound->getSndLevel()));
	}
	else {
		levelField.lock()->setText("100");
	}
}

void SndParamsObserver::displayTune()
{
	auto lSound = sound.lock();
	if (lSound) {
		tuneField.lock()->setText(to_string(lSound->getTune()));
	}
	else {
		tuneField.lock()->setText("0");
	}
}

void SndParamsObserver::displayBeat()
{
	auto lSound = sound.lock();
	if (lSound) {
		beatField.lock()->setText(to_string(lSound->getBeatCount()));
	}
	else {
		beatField.lock()->setText("4");
	}
}

void SndParamsObserver::displaySampleAndNewTempo()
{
	auto lSound = sound.lock();
    if(!lSound) {
        sampleTempoLabel.lock()->setText("");
        newTempoLabel.lock()->setText("");
        return;
    }
    auto length = lSound->getEnd() - lSound->getLoopTo();
    auto lengthMs = static_cast< float >((length / (lSound->getSampleRate() / 1000.0)));
    auto bpm = (int)((600000.0 / lengthMs) * lSound->getBeatCount());
	auto bpmString = to_string(bpm);
    auto part1 = bpmString.substr(0, bpmString.length() - 1);
    auto part2 = bpmString.substr(bpmString.length() - 1, bpmString.length());
    if(bpm < 300 || bpm > 3000) {
        part1 = "---";
        part2 = "-";
    }
	sampleTempoLabel.lock()->setText("Sample tempo=" + moduru::lang::StrUtil::padLeft(part1 + u8"\u00CB" + part2, " ", 5));
    auto newBpm = (int) (pow(2.0, (lSound->getTune() / 120.0) * bpm));
	bpmString = to_string(newBpm);
    part1 = bpmString.substr(0, bpmString.length() - 1);
    part2 = bpmString.substr(bpmString.length() - 1, bpmString.length());
    if(newBpm < 300 || newBpm > 3000) {
        part1 = "---";
        part2 = "-";
    }
	newTempoLabel.lock()->setText("New tempo=" + moduru::lang::StrUtil::padLeft(part1 + u8"\u00CB" + part2, " ", 5));
}

void SndParamsObserver::displaySnd()
{
	auto lSound = sound.lock();
	if(sampler.lock()->getSoundCount() != 0) {
        //sndField->grabFocus();
        auto sampleName = lSound->getName();
        if(!lSound->isMono()) {
            //sampleName = pc::disk::AbstractDisk::padRightSpace(sampleName, 16)  + "(ST)";
        }
        sndField.lock()->setText(sampleName);
    } else {
        sndField.lock()->setText("(no sound)");
 //       dummyField->grabFocus();
    }
}

void SndParamsObserver::displayPlayX()
{
	playXField.lock()->setText(playXNames[soundGui->getPlayX()]);
}

void SndParamsObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	auto lSampler = sampler.lock();
	
	if (lSampler->getSoundCount() != 0) {
		auto lSound = sound.lock();
		lSound->deleteObserver(this);
		lSound->getMsoc()->deleteObserver(this);
		sound = lSampler->getSound(soundGui->getSoundIndex());
		lSound = sound.lock();
		lSound->addObserver(this);
		lSound->getMsoc()->addObserver(this);
	}

	string s = boost::any_cast<string>(arg);

	if (s.compare("soundnumber") == 0) {
		auto lSound = sound.lock();
		displaySnd();
		soundGui->initZones(lSound->getLastFrameIndex() + 1);
		displayBeat();
		displaySampleAndNewTempo();
		displayTune();
		displayLevel();
	}
	else if (s.compare("playx") == 0) {
		displayPlayX();
	}
	else if (s.compare("beat") == 0) {
		displayBeat();
		displaySampleAndNewTempo();
	}
	else if (s.compare("tune") == 0) {
		displayTune();
		displaySampleAndNewTempo();
	}
	else if (s.compare("level") == 0) {
		displayLevel();
	}
}

SndParamsObserver::~SndParamsObserver() {
	soundGui->deleteObserver(this);
	auto lSampler = sampler.lock();
	if (lSampler->getSoundCount() != 0) {
		sound = lSampler->getSound(soundGui->getSoundIndex());
		auto lSound = sound.lock();
		lSound->deleteObserver(this);
		lSound->getMsoc()->deleteObserver(this);
	}
}
