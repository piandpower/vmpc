#include <ui/sampler/PgmAssignObserver.hpp>

#include <Mpc.hpp>
#include <Util.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/Label.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <sampler/NoteParameters.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>


#include <lang/StrUtil.hpp>

using namespace mpc::ui::sampler;
using namespace std;

PgmAssignObserver::PgmAssignObserver(mpc::Mpc* mpc)
{
	samplerGui = mpc->getUis().lock()->getSamplerGui();
	samplerGui->addObserver(this);
	sampler = mpc->getSampler();
	auto lSampler = sampler.lock();
	mpcSoundPlayerChannel = lSampler->getDrum(samplerGui->getSelectedDrum());
	program = lSampler->getProgram(mpcSoundPlayerChannel->getProgram());
	auto lProgram = program.lock();
	lProgram->addObserver(this);
	lastNp = lSampler->getLastNp(lProgram.get());
	lastNp->addObserver(this);
	lastPad = lSampler->getLastPad(lProgram.get());
	lastPad->addObserver(this);
	mpcSoundPlayerChannel->addObserver(this);
	auto ls = mpc->getLayeredScreen().lock();
	velocityRangeLowerLabel = ls->lookupLabel("velocityrangelower");
	velocityRangeLowerField = ls->lookupField("velocityrangelower");
	velocityRangeUpperLabel = ls->lookupLabel("velocityrangeupper");
	velocityRangeUpperField = ls->lookupField("velocityrangeupper");
	optionalNoteALabel = ls->lookupLabel("optionalnotenumbera");
	optionalNoteAField = ls->lookupField("optionalnotenumbera");
	optionalNoteBLabel = ls->lookupLabel("optionalnotenumberb");
	optionalNoteBField = ls->lookupField("optionalnotenumberb");

	optionalNoteALabel.lock()->enableRigorousClearing();
	optionalNoteBLabel.lock()->enableRigorousClearing();

	pgmField = ls->lookupField("pgm");
	selectedPadNumberField = ls->lookupField("pad");
	padNoteField = ls->lookupField("padnote");
	padAssignModeField = ls->lookupField("padassign");
	selectedNoteField = ls->lookupField("note");
	sndNumberField = ls->lookupField("snd");
	soundGenerationModeField = ls->lookupField("mode");
	isSoundStereoLabel = ls->lookupLabel("issoundstereo");
	displaySoundGenerationMode();
	displayPgm();
	displaySelectedPadName();
	displayPadNote();
	displayPadAssignMode();
	displaySelectedNote();
	displaySoundName();
}

void PgmAssignObserver::displayPgm()
{
	pgmField.lock()->setText(moduru::lang::StrUtil::padLeft(to_string(mpcSoundPlayerChannel->getProgram() + 1), " ", 2) + "-" + program.lock()->getName());
}

void PgmAssignObserver::displaySoundName()
{
	auto lSampler = sampler.lock();
	int sndNumber = lSampler->getLastNp(program.lock().get())->getSndNumber();
	if (sndNumber == -1) {
		sndNumberField.lock()->setText("OFF");
		isSoundStereoLabel.lock()->setText("");
	}
	else {
		string name = lSampler->getSoundName(sndNumber);
		sndNumberField.lock()->setText(name);
	}

	if (lSampler->getSoundCount() != 0 && sndNumber != -1) {
		if (lSampler->getSound(sndNumber).lock()->isMono()) {
			isSoundStereoLabel.lock()->setText("");
		}
		else {
			isSoundStereoLabel.lock()->setText("(ST)");
		}
	}
}

void PgmAssignObserver::displayPadAssignMode()
{
	padAssignModeField.lock()->setText(samplerGui->isPadAssignMaster() ? "MASTER" : "PROGRAM");
}

void PgmAssignObserver::displayPadNote()
{
	auto lSampler = sampler.lock();
	auto lProgram = program.lock();
	if (lSampler->getLastPad(lProgram.get())->getNote() == 34) {
		padNoteField.lock()->setText("--");
		return;
	}
	padNoteField.lock()->setText(to_string(lSampler->getLastPad(lProgram.get())->getNote()));
}

void PgmAssignObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	auto lSampler = sampler.lock();
	auto lProgram = program.lock();

	lastNp->deleteObserver(this);
	lastPad->deleteObserver(this);
	lProgram->deleteObserver(this);
	mpcSoundPlayerChannel->deleteObserver(this);

	mpcSoundPlayerChannel = lSampler->getDrum(samplerGui->getSelectedDrum());
	program = lSampler->getProgram(mpcSoundPlayerChannel->getProgram());
	lProgram = program.lock();

	lastNp = lSampler->getLastNp(lProgram.get());
	lastNp->addObserver(this);
	lastPad = lSampler->getLastPad(lProgram.get());
	lastPad->addObserver(this);
	lProgram->addObserver(this);
	mpcSoundPlayerChannel->addObserver(this);

	string s = boost::any_cast<string>(arg);

	if (s.compare("pgm") == 0 || s.compare("padandnote") == 0 || s.compare("padnotenumber") == 0) {
		displayPgm();
		displaySelectedNote();
		displaySelectedPadName();
		displayPadNote();
		displaySoundName();
		displaySoundGenerationMode();
	}
	else if (s.compare("padassignmode") == 0) {
		displayPadAssignMode();
		displayPadNote();
		displaySoundName();
		displaySoundGenerationMode();
	}
	else if (s.compare("samplenumber") == 0) {
		displaySoundName();
	}
	else if (s.compare("soundgenerationmode") == 0) {
		displaySoundGenerationMode();
	}
	else if (s.compare("ifover1") == 0) {
		displayVeloRangeLower();
	}
	else if (s.compare("ifover2") == 0) {
		displayVeloRangeUpper();
	}
	else if (s.compare("use1") == 0) {
		displayOptionalNoteA();
	}
	else if (s.compare("use2") == 0) {
		displayOptionalNoteB();
	}
}

void PgmAssignObserver::displaySoundGenerationMode()
{
	auto lSampler = sampler.lock();
	auto sgm = -1;
	auto lProgram = program.lock();
	if (lSampler->getLastNp(lProgram.get()) != nullptr) {
		sgm = lSampler->getLastNp(lProgram.get())->getSoundGenerationMode();
		soundGenerationModeField.lock()->setText(soundGenerationModes[sgm]);
		if (sgm != 0) {
			velocityRangeLowerLabel.lock()->Hide(true);
			velocityRangeLowerField.lock()->Hide(true);
			velocityRangeUpperLabel.lock()->Hide(true);
			velocityRangeUpperField.lock()->Hide(true);
			optionalNoteALabel.lock()->Hide(false);
			optionalNoteAField.lock()->Hide(false);
			optionalNoteBLabel.lock()->Hide(false);
			optionalNoteBField.lock()->Hide(false);
			optionalNoteALabel.lock()->setText("Also play note:");
			optionalNoteBLabel.lock()->setText("Also play note:");
			displayOptionalNoteA();
			displayOptionalNoteB();
		}
		if (sgm == 2 || sgm == 3) {
			optionalNoteALabel.lock()->setText("over:   , use:");
			optionalNoteBLabel.lock()->setText("over:   , use:");
			optionalNoteALabel.lock()->setOpaque(false);
			optionalNoteBLabel.lock()->setOpaque(false);
			velocityRangeLowerLabel.lock()->Hide(false);
			velocityRangeLowerField.lock()->Hide(false);
			velocityRangeUpperLabel.lock()->Hide(false);
			velocityRangeUpperField.lock()->Hide(false);
			displayVeloRangeLower();
			displayVeloRangeUpper();
		}
	}
	if (lSampler->getLastNp(lProgram.get()) == nullptr || sgm == -1 || sgm == 0) {
		velocityRangeLowerLabel.lock()->Hide(true);
		velocityRangeLowerField.lock()->Hide(true);
		velocityRangeUpperLabel.lock()->Hide(true);
		velocityRangeUpperField.lock()->Hide(true);
		optionalNoteALabel.lock()->Hide(true);
		optionalNoteAField.lock()->Hide(true);
		optionalNoteBLabel.lock()->Hide(true);
		optionalNoteBField.lock()->Hide(true);
	}
}

void PgmAssignObserver::displayVeloRangeUpper()
{
	auto rangeB = sampler.lock()->getLastNp(program.lock().get())->getVelocityRangeUpper();
	velocityRangeUpperField.lock()->setTextPadded(rangeB, " ");
}

void PgmAssignObserver::displayVeloRangeLower()
{
	auto rangeA = sampler.lock()->getLastNp(program.lock().get())->getVelocityRangeLower();
	velocityRangeLowerField.lock()->setTextPadded(rangeA, " ");
}

void PgmAssignObserver::displayOptionalNoteA()
{
	auto lSampler = sampler.lock();
	auto lProgram = program.lock();
	auto noteIntA = lSampler->getLastNp(lProgram.get())->getOptionalNoteA();
	auto padIntA = lProgram->getPadNumberFromNote(noteIntA);
	auto noteA = noteIntA != 34 ? to_string(noteIntA) : "--";
	auto padA = padIntA != -1 ? lSampler->getPadName(padIntA) : "OFF";
	optionalNoteAField.lock()->setText(noteA + "/" + padA);
}

void PgmAssignObserver::displayOptionalNoteB()
{
	auto lSampler = sampler.lock();
	auto lProgram = program.lock();
	auto noteIntB = lSampler->getLastNp(lProgram.get())->getOptionalNoteB();
	auto padIntB = lProgram->getPadNumberFromNote(noteIntB);
	auto noteB = noteIntB != 34 ? to_string(noteIntB) : "--";
	auto padB = padIntB != -1 ? lSampler->getPadName(padIntB) : "OFF";
	optionalNoteBField.lock()->setText(noteB + "/" + padB);
}

void PgmAssignObserver::displaySelectedNote()
{
	selectedNoteField.lock()->setText(to_string(sampler.lock()->getLastNp(program.lock().get())->getNumber()));
}

void PgmAssignObserver::displaySelectedPadName()
{
	selectedPadNumberField.lock()->setText(sampler.lock()->getPadName(samplerGui->getPad()));
}

PgmAssignObserver::~PgmAssignObserver() {
	samplerGui->deleteObserver(this);
	program.lock()->deleteObserver(this);
	lastNp->deleteObserver(this);
	lastPad->deleteObserver(this);
	mpcSoundPlayerChannel->deleteObserver(this);
}
