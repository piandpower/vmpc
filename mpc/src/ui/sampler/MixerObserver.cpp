#include "MixerObserver.hpp"

#include <Mpc.hpp>
#include <Util.hpp>
#include <StartUp.hpp>
#include <lcdgui/MixerStrip.hpp>
#include <lcdgui/Field.hpp>
#include <ui/sampler/MixerGui.hpp>
#include <ui/sampler/MixerSetupGui.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <sampler/MixerChannel.hpp>
#include <sampler/NoteParameters.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/Sequencer.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>
#include <lcdgui/Field.hpp>

#include <lang/StrUtil.hpp>

#include <cmath>

using namespace mpc::ui::sampler;
using namespace std;

MixerObserver::MixerObserver(mpc::Mpc* mpc)  
{
	this->mpc = mpc;
	fxPathNames = { "--", "M1", "M2", "R1", "R2" };
	stereoNames = { "-", "12", "12", "34", "34", "56", "56", "78", "78" };
	stereoNamesSlash = { "-", "1/2", "1/2", "3/4", "3/4", "5/6", "5/6", "7/8", "7/8" };
	monoNames = { "-", "1", "2", "3", "4", "5", "6", "7", "8" };
	auto uis = mpc->getUis().lock();
	samplerGui = uis->getSamplerGui();
	samplerGui->addObserver(this);
	bank = samplerGui->getBank();
	mixGui = uis->getMixerGui();
	mixGui->addObserver(this);

	ls = mpc->getLayeredScreen();
	auto lLs = ls.lock();
	sampler = mpc->getSampler();
	mixerSetupGui = uis->getMixerSetupGui();
	mixerSetupGui->addObserver(this);
	auto lSampler = sampler.lock();
	mpcSoundPlayerChannel = lSampler->getDrum(samplerGui->getSelectedDrum());
	program = lSampler->getProgram(mpcSoundPlayerChannel->getProgram());

	for (int i = (bank * 16); i < (bank * 16) + 16; i++) {
		pad = program.lock()->getPad(i);
		mixerChannel = pad->getMixerChannel();
		auto lMc = mixerChannel.lock();
		lMc->addObserver(this);
	}

	if (lLs->getCurrentScreenName().compare("mixer") == 0) {
		initPadNameLabels();
		initMixerStrips();
		for (auto& m : mixerStrips) {
			m->initFields();
			m->setColors();
		}
		displayMixerStrips();
		displayFunctionKeys();
	}
	else if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
		noteField = lLs->lookupField("note");
		stereoVolumeField = lLs->lookupField("stereovolume");
		individualVolumeField = lLs->lookupField("individualvolume");
		fxSendLevelField = lLs->lookupField("fxsendlevel");
		panningField = lLs->lookupField("panning");
		outputField = lLs->lookupField("output");
		fxPathField = lLs->lookupField("fxpath");
		followStereoField = lLs->lookupField("followstereo");
		displayChannel();
	}
	else if (lLs->getCurrentScreenName().compare("mixersetup") == 0) {
		masterLevelField = lLs->lookupField("masterlevel");
		fxDrumField = lLs->lookupField("fxdrum");
		stereoMixSourceField = lLs->lookupField("stereomixsource");
		indivFxSourceField = lLs->lookupField("indivfxsource");
		copyPgmMixToDrumField = lLs->lookupField("copypgmmixtodrum");
		recordMixChangesField = lLs->lookupField("recordmixchanges");
		displayMasterLevel();
		displayFxDrum();
		displayStereoMixSource();
		displayIndivFxSource();
		displayCopyPgmMixToDrum();
		displayRecordMixChanges();
	}
}

void MixerObserver::initPadNameLabels()
{
	int tfCounter3 = 0;
	int tfCounter4 = 0;
	for (auto textField : ls.lock()->getLayer(0)->getAllFields()) {
		auto tf = dynamic_pointer_cast<mpc::lcdgui::Field>(textField.lock());
		if (tf) {
			if (tf->getName().compare("e3") == 0 || tf->getName().compare("e4") == 0) {
				tf->setSize(6, 9);
			}
			if (tf->getName().length() == 2) {
				if (tf->getName()[1] == '3') {
					tf->setText("0");
					if (tfCounter3 > 8) {
						tf->setText("1");
					}
					tfCounter3++;
				}
				else if (tf->getName()[1] == '4') {
					tf->setText(to_string(tfCounter4 + 1));
					tfCounter4++;
					if (tfCounter4 == 9)
						tfCounter4 = -1;

				}
			}
		}
	}
}

void MixerObserver::initMixerStrips()
{
	auto lProgram = program.lock();
	for (int i = (bank * 16); i < (bank * 16) + 16; i++) {
		pad = lProgram->getPad(i);
		mixerChannel = pad->getMixerChannel();
	}
	mixerStrips.clear();
	for (int i = 0; i < 16; i++) {
		auto mixerStrip = new mpc::lcdgui::MixerStrip(i, bank, mpc);
		mixerStrips.push_back(mixerStrip);
	}
	mixGui->setMixerStrips(mixerStrips);
	displayMixerStrips();
	mixerStrips[mixGui->getXPos()]->setSelection(mixGui->getYPos());
}

void MixerObserver::displayChannel()
{
	auto lProgram = program.lock();
	mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
	auto lMc = mixerChannel.lock();
    setNoteField();
    stereoVolumeField.lock()->setTextPadded(lMc->getLevel(), " ");
    individualVolumeField.lock()->setTextPadded(lMc->getVolumeIndividualOut(), " ");
    fxSendLevelField.lock()->setTextPadded(lMc->getFxSendLevel(), " ");
    setPanningField();
    setOutputField();
    fxPathField.lock()->setText(fxPathNames[lMc->getFxPath()]);
    followStereoField.lock()->setText(lMc->isFollowingStereo() ? "YES" : "NO");
}

void MixerObserver::setOutputField()
{
	auto lProgram = program.lock();
	mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
	auto lMc = mixerChannel.lock();

	if (lMc->isStereo()) {
		outputField.lock()->setText(stereoNamesSlash[lMc->getOutput()]);
	}
	else {
		outputField.lock()->setText(" " + to_string(lMc->getOutput()));
	}
}

void MixerObserver::setNoteField()
{
	auto lSampler = sampler.lock();
	string soundString = "OFF";
	auto lProgram = program.lock();
	auto sampleNumber = lProgram->getNoteParameters(mixGui->getChannelSettingsNote())->getSndNumber();
	auto padNumber = lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote());
	if (sampleNumber > 0 && sampleNumber < lSampler->getSoundCount()) {
		soundString = lSampler->getSoundName(sampleNumber);
		if (!lSampler->getSound(sampleNumber).lock()->isMono()) {
			soundString += moduru::lang::StrUtil::padLeft("(ST)", " ", 23 - soundString.length());
		}
	}
	noteField.lock()->setText(to_string(mixGui->getChannelSettingsNote()) + "/" + lSampler->getPadName(padNumber) + "-" + soundString);
}

void MixerObserver::setPanningField()
{
	auto lProgram = program.lock();
	mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
	auto lMc = mixerChannel.lock();
	auto panning = "L";
	if (lMc->getPanning() > 0) panning = "R";
	panningField.lock()->setText(panning + moduru::lang::StrUtil::padLeft(to_string(abs(lMc->getPanning())), " ", 2));
	if (lMc->getPanning() == 0) {
		panningField.lock()->setText("MID");
	}
}
void MixerObserver::displayMixerStrips()
{
	for (int i = 0; i < 16; i++) {
		mixerChannel = program.lock()->getPad(i + (bank * 16))->getMixerChannel();
		auto lMc = mixerChannel.lock();
		if (mixGui->getTab() == 0) {
			mixerStrips[i]->setValueA(lMc->getPanning());
			mixerStrips[i]->setValueB(lMc->getLevel());
		}
		else if (mixGui->getTab() == 1) {
			if (lMc->isStereo()) {
				mixerStrips[i]->setValueAString(stereoNames[lMc->getOutput()]);
			}
			else {
				mixerStrips[i]->setValueAString(monoNames[lMc->getOutput()]);
			}
			mixerStrips[i]->setValueB(lMc->getVolumeIndividualOut());
		}
		else if (mixGui->getTab() == 2) {
			mixerStrips[i]->setValueAString(fxPathNames[lMc->getFxPath()]);
			mixerStrips[i]->setValueB(lMc->getFxSendLevel());
		}
	}
}

void MixerObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	auto lLs = ls.lock();
	auto lProgram = program.lock();
	auto lMc = mixerChannel.lock();

	for (int i = (bank * 16); i < (bank * 16) + 16; i++) {
		pad = lProgram->getPad(i);
		mixerChannel = pad->getMixerChannel();
		auto lMc = mixerChannel.lock();
		lMc->deleteObserver(this);
	}

	bank = samplerGui->getBank();

	for (int i = (bank * 16); i < (bank * 16) + 16; i++) {
		pad = lProgram->getPad(i);
		mixerChannel = pad->getMixerChannel();
		auto lMc = mixerChannel.lock();
		lMc->addObserver(this);
	}

	string s = boost::any_cast<string>(arg);

	if (s.compare("tab") == 0) {
		for (auto& m : mixerStrips) {
			m->initFields();
			m->setColors();
		}
		displayMixerStrips();
		displayFunctionKeys();
	}
	else if (s.compare("position") == 0) {
		if (!mixGui->getLink()) {
			for (auto& m : mixerStrips) {
				m->setSelection(-1);
			}
			mixerStrips[mixGui->getXPos()]->setSelection(mixGui->getYPos());
		}
		else {
			for (auto& m : mixerStrips) {
				m->setSelection(mixGui->getYPos());
			}
		}
	}
	else if (s.compare("volume") == 0) {
		if (lLs->getCurrentScreenName().compare("mixer") == 0) {
			if (!mixGui->getLink()) {
				mixerChannel = lProgram->getPad(mixGui->getXPos() + (bank * 16))->getMixerChannel();
				lMc = mixerChannel.lock();
				mixerStrips[mixGui->getXPos()]->setValueB(lMc->getLevel());
			}
			else {
				for (int i = 0; i < 16; i++) {
					auto mc = lProgram->getPad(i + (bank * 16))->getMixerChannel().lock();
					mixerStrips[i]->setValueB(mc->getLevel());
				}
			}
		}
		if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
			mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
			lMc = mixerChannel.lock();
			stereoVolumeField.lock()->setTextPadded(lMc->getLevel(), " ");
		}
	}
	else if (s.compare("panning") == 0) {
		if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
			mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
			lMc = mixerChannel.lock();
			setPanningField();
		}
		else if (lLs->getCurrentScreenName().compare("mixer") == 0) {
			if (!mixGui->getLink()) {
				mixerChannel = lProgram->getPad(mixGui->getXPos() + (bank * 16))->getMixerChannel();
				lMc = mixerChannel.lock();
				mixerStrips[mixGui->getXPos()]->setValueA(lMc->getPanning());
			}
			if (mixGui->getLink()) {
				for (int i = 0; i < 16; i++) {
					auto mc = lProgram->getPad(i + (bank * 16))->getMixerChannel().lock();
					mixerStrips[i]->setValueA(mc->getPanning());
				}
			}
		}
	}
	else if (s.compare("output") == 0) {
		if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
			mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
			lMc = mixerChannel.lock();
			setOutputField();
		}
		else if (lLs->getCurrentScreenName().compare("mixer") == 0) {
			if (!mixGui->getLink()) {
				mixerChannel = lProgram->getPad(mixGui->getXPos() + (bank * 16))->getMixerChannel();
				lMc = mixerChannel.lock();
				if (lMc->isStereo()) {
					mixerStrips[mixGui->getXPos()]->setValueAString(stereoNames[lMc->getOutput()]);
				}
				else {
					mixerStrips[mixGui->getXPos()]->setValueAString(monoNames[lMc->getOutput()]);
				}
			}
			else {
				for (int i = 0; i < 16; i++) {
					auto mc = lProgram->getPad(i + bank * 16)->getMixerChannel().lock();
					if (mc->isStereo()) {
						mixerStrips[i]->setValueAString(stereoNames[mc->getOutput()]);
					}
					else {
						mixerStrips[i]->setValueAString(monoNames[mc->getOutput()]);
					}
				}
			}
		}
	}
	else if (s.compare("volumeindividual") == 0) {
		if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
			mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
			lMc = mixerChannel.lock();
			individualVolumeField.lock()->setTextPadded(lMc->getVolumeIndividualOut(), " ");
		}
		else if (lLs->getCurrentScreenName().compare("mixer") == 0) {
			if (!mixGui->getLink()) {
				mixerChannel = lProgram->getPad(mixGui->getXPos() + (bank * 16))->getMixerChannel();
				lMc = mixerChannel.lock();
				mixerStrips[mixGui->getXPos()]->setValueB(lMc->getVolumeIndividualOut());
			}
			else {
				for (int i = 0; i < 16; i++) {
					auto mc = lProgram->getPad(i + (bank * 16))->getMixerChannel().lock();
					mixerStrips[i]->setValueB(mc->getVolumeIndividualOut());
				}
			}
		}
	}
	else if (s.compare("fxpath") == 0) {
		if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
			mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
			lMc = mixerChannel.lock();
			fxPathField.lock()->setText(fxPathNames[lMc->getFxPath()]);
		}
		else if (lLs->getCurrentScreenName().compare("mixer") == 0) {
			if (!mixGui->getLink()) {
				mixerChannel = lProgram->getPad(mixGui->getXPos() + (bank * 16))->getMixerChannel();
				lMc = mixerChannel.lock();
				mixerStrips[mixGui->getXPos()]->setValueAString(fxPathNames[lMc->getFxPath()]);
			}
			else {
				for (int i = 0; i < 16; i++) {
					auto mc = lProgram->getPad(i + (bank * 16))->getMixerChannel().lock();
					mixerStrips[i]->setValueAString(fxPathNames[mc->getFxPath()]);
				}
			}
		}
	}
	else if (s.compare("fxsendlevel") == 0) {
		if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
			mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
			lMc = mixerChannel.lock();
			fxSendLevelField.lock()->setTextPadded(lMc->getFxSendLevel(), " ");
		}
		else if (lLs->getCurrentScreenName().compare("mixer") == 0) {
			if (!mixGui->getLink()) {
				mixerChannel = lProgram->getPad(mixGui->getXPos() + (bank * 16))->getMixerChannel();
				lMc = mixerChannel.lock();
				mixerStrips[mixGui->getXPos()]->setValueB(lMc->getFxSendLevel());
			}
			else {
				for (int i = 0; i < 16; i++) {
					auto mc = lProgram->getPad(i + (bank * 16))->getMixerChannel().lock();
					mixerStrips[i]->setValueB(mc->getFxSendLevel());
				}
			}
		}
	}
	else if (s.compare("link") == 0) {
		if (mixGui->getLink()) {
			for (auto& m : mixerStrips) {
				m->setSelection(mixGui->getYPos());
			}
		}
		else {
			for (auto& m : mixerStrips) {
				m->setSelection(-1);
			}
			mixerStrips[mixGui->getXPos()]->setSelection(mixGui->getYPos());
		}
		displayFunctionKeys();
	}
	else if (s.compare("note") == 0) {
		if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
			displayChannel();
		}
	}
	else if (s.compare("followstereo") == 0) {
		if (lLs->getCurrentScreenName().compare("channelsettings") == 0) {
			followStereoField.lock()->setText(lMc->isFollowingStereo() ? "YES" : "NO");
		}
	}
	else if (s.compare("bank") == 0) {
		initPadNameLabels();
		initMixerStrips();
		for (auto& m : mixerStrips) {
			m->initFields();
			m->setColors();
		}
		displayMixerStrips();
		displayFunctionKeys();
	}
	else if (s.compare("masterlevel") == 0) {
		displayMasterLevel();
	}
	else if (s.compare("fxdrum") == 0) {
		displayFxDrum();
	}
	else if (s.compare("stereomixsource") == 0) {
		displayStereoMixSource();
	}
	else if (s.compare("indivfxsource") == 0) {
		displayIndivFxSource();
	}
	else if (s.compare("copypgmmixtodrum") == 0) {
		displayCopyPgmMixToDrum();
	}
	else if (s.compare("recordmixchanges") == 0) {
		displayRecordMixChanges();
	}
}

void MixerObserver::displayFunctionKeys()
{
	string link = "";
	if (mixGui->getLink()) {
		link = "_link";
	}

	auto lLs = ls.lock();

	if (mixGui->getTab() == 0) {
		lLs->drawFunctionKeys("mixer_stereo" + link);
	}
	else if (mixGui->getTab() == 1) {
		lLs->drawFunctionKeys("mixer_indiv" + link);
	}
	else if (mixGui->getTab() == 2) {
		lLs->drawFunctionKeys("mixer_fxsend" + link);
	}
}

void MixerObserver::displayMasterLevel() {
	if (mixerSetupGui->getMasterLevel() != -73) {
		masterLevelField.lock()->setTextPadded(to_string(mixerSetupGui->getMasterLevel()) + "dB", " ");
	}
	else {
		masterLevelField.lock()->setTextPadded(u8"-\u00D9\u00DAdB", " ");
	}
}

void MixerObserver::displayFxDrum() {
	fxDrumField.lock()->setText(to_string(mixerSetupGui->getFxDrum()));
}

void MixerObserver::displayStereoMixSource() {
	stereoMixSourceField.lock()->setText(mixerSetupGui->isStereoMixSourceDrum() ? "DRUM" : "PROGRAM");
}

void MixerObserver::displayIndivFxSource() {
	indivFxSourceField.lock()->setText(mixerSetupGui->isIndivFxSourceDrum() ? "DRUM" : "PROGRAM");
}

void MixerObserver::displayCopyPgmMixToDrum() {
	copyPgmMixToDrumField.lock()->setText(mixerSetupGui->isCopyPgmMixToDrumEnabled() ? "YES" : "NO");
}

void MixerObserver::displayRecordMixChanges() {
	recordMixChangesField.lock()->setText(mixerSetupGui->isRecordMixChangesEnabled() ? "YES" : "NO");
}


MixerObserver::~MixerObserver() {
	mixerSetupGui->deleteObserver(this);
	mixGui->deleteObserver(this);
	samplerGui->deleteObserver(this);

	for (int i = (bank * 16); i < (bank * 16) + 16; i++) {
		pad = program.lock()->getPad(i);
		mixerChannel = pad->getMixerChannel();
		auto lMc = mixerChannel.lock();
		lMc->deleteObserver(this);
	}

	for (auto& m : mixerStrips) {
		if (m != nullptr) delete m;
	}
}
