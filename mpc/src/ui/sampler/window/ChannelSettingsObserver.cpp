#include "ChannelSettingsObserver.hpp"

#include <Mpc.hpp>
#include <Util.hpp>
#include <StartUp.hpp>
#include <lcdgui/Field.hpp>
#include <ui/sampler/MixerGui.hpp>
#include <ui/sampler/MixerSetupGui.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <sampler/MixerChannel.hpp>
#include <sampler/NoteParameters.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>

#include <lang/StrUtil.hpp>

#include <cmath>

using namespace mpc::ui::sampler;
using namespace std;

ChannelSettingsObserver::ChannelSettingsObserver(mpc::Mpc* mpc)
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
		auto pad = program.lock()->getPad(i);
		auto mixerChannel = pad->getMixerChannel();
		auto lMc = mixerChannel.lock();
		lMc->addObserver(this);
	}

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

void ChannelSettingsObserver::displayChannel()
{
	auto lProgram = program.lock();
	auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
	auto lMc = mixerChannel.lock();
    displayNoteField();
    stereoVolumeField.lock()->setTextPadded(lMc->getLevel(), " ");
    individualVolumeField.lock()->setTextPadded(lMc->getVolumeIndividualOut(), " ");
    fxSendLevelField.lock()->setTextPadded(lMc->getFxSendLevel(), " ");
    displayPanningField();
    displayOutputField();
    fxPathField.lock()->setText(fxPathNames[lMc->getFxPath()]);
    followStereoField.lock()->setText(lMc->isFollowingStereo() ? "YES" : "NO");
}

void ChannelSettingsObserver::displayOutputField()
{
	auto lProgram = program.lock();
	auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
	auto lMc = mixerChannel.lock();

	if (lMc->isStereo()) {
		outputField.lock()->setText(stereoNamesSlash[lMc->getOutput()]);
	}
	else {
		outputField.lock()->setText(" " + to_string(lMc->getOutput()));
	}
}

void ChannelSettingsObserver::displayNoteField()
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

void ChannelSettingsObserver::displayPanningField()
{
	auto lProgram = program.lock();
	auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
	auto lMc = mixerChannel.lock();
	auto panning = "L";
	if (lMc->getPanning() > 0) panning = "R";
	panningField.lock()->setText(panning + moduru::lang::StrUtil::padLeft(to_string(abs(lMc->getPanning())), " ", 2));
	if (lMc->getPanning() == 0) {
		panningField.lock()->setText("MID");
	}
}

void ChannelSettingsObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	auto lLs = ls.lock();
	auto lProgram = program.lock();

	for (int i = (bank * 16); i < (bank * 16) + 16; i++) {
		auto pad = lProgram->getPad(i);
		auto mixerChannel = pad->getMixerChannel();
		auto lMc = mixerChannel.lock();
		lMc->deleteObserver(this);
	}

	bank = samplerGui->getBank();

	for (int i = (bank * 16); i < (bank * 16) + 16; i++) {
		auto pad = lProgram->getPad(i);
		auto mixerChannel = pad->getMixerChannel();
		auto lMc = mixerChannel.lock();
		lMc->addObserver(this);
	}

	string s = boost::any_cast<string>(arg);

	if (s.compare("volume") == 0) {
		auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
		auto lMc = mixerChannel.lock();
		stereoVolumeField.lock()->setTextPadded(lMc->getLevel(), " ");
	}
	else if (s.compare("panning") == 0) {
		auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
		auto lMc = mixerChannel.lock();
		displayPanningField();
	}
	else if (s.compare("output") == 0) {
		auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
		auto lMc = mixerChannel.lock();
		displayOutputField();
	}
	else if (s.compare("volumeindividual") == 0) {
		auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
		auto lMc = mixerChannel.lock();
		individualVolumeField.lock()->setTextPadded(lMc->getVolumeIndividualOut(), " ");
	}
	else if (s.compare("fxpath") == 0) {
		auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
		auto lMc = mixerChannel.lock();
		fxPathField.lock()->setText(fxPathNames[lMc->getFxPath()]);
	}
	else if (s.compare("fxsendlevel") == 0) {
		auto mixerChannel = lProgram->getPad(lProgram->getPadNumberFromNote(mixGui->getChannelSettingsNote()))->getMixerChannel();
		auto lMc = mixerChannel.lock();
		fxSendLevelField.lock()->setTextPadded(lMc->getFxSendLevel(), " ");
	}
	else if (s.compare("note") == 0) {
		displayChannel();
	}
	else if (s.compare("followstereo") == 0) {
		auto mixerChannel = lProgram->getPad(mixGui->getXPos() + (bank * 16))->getMixerChannel();
		auto lMc = mixerChannel.lock();
		followStereoField.lock()->setText(lMc->isFollowingStereo() ? "YES" : "NO");
	}
	else if (s.compare("bank") == 0) {
		// update everything?
	}
}

ChannelSettingsObserver::~ChannelSettingsObserver() {
	mixerSetupGui->deleteObserver(this);
	mixGui->deleteObserver(this);
	samplerGui->deleteObserver(this);

	for (int i = (bank * 16); i < (bank * 16) + 16; i++) {
		auto pad = program.lock()->getPad(i);
		auto mixerChannel = pad->getMixerChannel();
		auto lMc = mixerChannel.lock();
		lMc->deleteObserver(this);
	}
}
