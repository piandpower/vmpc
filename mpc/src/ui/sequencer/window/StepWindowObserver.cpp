#include <ui/sequencer/window/StepWindowObserver.hpp>

#include <Mpc.hpp>
#include <Util.hpp>
#include <ui/Uis.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <lcdgui/Field.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <ui/sequencer/StepEditorGui.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sequencer/ChannelPressureEvent.hpp>
#include <sequencer/ControlChangeEvent.hpp>
#include <sequencer/Event.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/NoteEvent.hpp>
#include <sequencer/PolyPressureEvent.hpp>
#include <sequencer/ProgramChangeEvent.hpp>
#include <sequencer/Sequencer.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>

using namespace mpc::ui::sequencer::window;
using namespace mpc::sequencer;
using namespace std;

StepWindowObserver::StepWindowObserver(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	timingCorrectNames = { "OFF", "1/8", "1/8(3)", "1/16", "1/16(3)", "1/32", "1/32(3)" };
	eventTypeNames = { "NOTE", "PITCH BEND", "CONTROL CHANGE", "PROGRAM CHANGE", "CH PRESSURE", "POLY PRESSURE", "EXCLUSIVE", "MIXER" };
	noteVariationParameterNames = { "Tun", "Dcy", "Atk", "Flt" };
	editTypeNames = { "ADD VALUE", "SUB VALUE", "MULT VAL%", "SET TO VAL" };
	xPosSingle = 120;
	yPosSingle = 50;
	singleLabels = { "Change note to:", "Variation type:", "Variation value:" };
	xPosDouble = { 120, 144 };
	yPosDouble = { 45, 63 };
	doubleLabels = { "Edit type:", "Value:" };
	sequencer = mpc->getSequencer();
	sampler = mpc->getSampler();
	seqGui = mpc->getUis().lock()->getStepEditorGui();
	samplerGui = mpc->getUis().lock()->getSamplerGui();
	seqGui->deleteObservers();
	seqGui->addObserver(this);
	auto lSequencer = sequencer.lock();
	auto seqNum = lSequencer->getActiveSequenceIndex();
	auto trackNum = lSequencer->getActiveTrackIndex();
	track = lSequencer->getSequence(seqNum).lock()->getTrack(trackNum);
	auto lSampler = sampler.lock();
	mpcSoundPlayerChannel = lSampler->getDrum(track.lock()->getBusNumber() -1);
	program = lSampler->getProgram(mpcSoundPlayerChannel->getProgram());
	lSequencer->deleteObservers();
	lSequencer->addObserver(this);
	auto ls = mpc->getLayeredScreen().lock();
	auto csn = ls->getCurrentScreenName();
	if (csn.compare("step_tc") == 0) {
		tcValueField = ls->lookupField("tcvalue");
		tcValueField.lock()->setText(timingCorrectNames[mpc->getUis().lock()->getSequencerWindowGui()->getNoteValue()]);
	}
	else if (csn.compare("insertevent") == 0) {
		eventtypeField = ls->lookupField("eventtype");
		eventtypeField.lock()->setText(eventTypeNames[seqGui->getInsertEventType()]);
	}
	else if (csn.compare("editmultiple") == 0) {
		editMultiParam0Label = ls->lookupLabel("value0");
		editMultiParam1Label = ls->lookupLabel("value1");
		editMultiValue0Field = ls->lookupField("value0");
		editMultiValue1Field = ls->lookupField("value1");
		updateEditMultiple();
	}
}

void StepWindowObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	string s = boost::any_cast<string>(arg);
	if (s.compare("timing") == 0) {
		tcValueField.lock()->setText(timingCorrectNames[mpc->getUis().lock()->getSequencerWindowGui()->getNoteValue()]);
	}
	else if (s.compare("eventtype") == 0) {
		eventtypeField.lock()->setText(eventTypeNames[seqGui->getInsertEventType()]);
	}
	else if (s.compare("editmultiple") == 0) {
		updateEditMultiple();
	}

}

void StepWindowObserver::updateEditMultiple()
{
	auto lSampler = sampler.lock();
	auto event = seqGui->getSelectedEvent().lock();
	if (dynamic_pointer_cast<NoteEvent>(event) && track.lock()->getBusNumber() != 0) {
		if (seqGui->getParamLetter().compare("a") == 0 || seqGui->getParamLetter().compare("b") == 0 || seqGui->getParamLetter().compare("c") == 0) {
			editMultiParam1Label.lock()->Hide(true);
			editMultiValue1Field.lock()->Hide(true);
			editMultiParam0Label.lock()->Hide(false);
			editMultiParam0Label.lock()->setLocation(xPosSingle/2, yPosSingle/2);
			if (seqGui->getParamLetter().compare("a") == 0) {
				editMultiParam0Label.lock()->setText(singleLabels[0]);
				editMultiValue0Field.lock()->setSize(6 * 6 * 2 + 2, 18);
				editMultiValue0Field.lock()->setText(to_string(seqGui->getChangeNoteToNumber()) + "/" + lSampler->getPadName(program.lock()->getPadNumberFromNote(seqGui->getChangeNoteToNumber())));
			}
			if (seqGui->getParamLetter().compare("b") == 0) {
				editMultiParam0Label.lock()->setText(singleLabels[1]);
				editMultiValue0Field.lock()->setSize(3 * 6 * 2 + 2, 18);
				editMultiValue0Field.lock()->setText(noteVariationParameterNames[seqGui->getChangeVariationTypeNumber()]);
			}
			if (seqGui->getParamLetter().compare("c") == 0) {
				editMultiParam0Label.lock()->setText(singleLabels[2]);
				if (seqGui->getChangeVariationTypeNumber() == 0) {
					editMultiValue0Field.lock()->setSize(4 * 6 * 2 + 2, 18);
					editMultiValue0Field.lock()->setLocation(90/2, editMultiValue0Field.lock()->getY());
					auto noteVarValue = (seqGui->getChangeVariationValue() * 2) - 128;

					if (noteVarValue < -120) {
						noteVarValue = -120;
					}
					else if (noteVarValue > 120) {
						noteVarValue = 120;
					}

					if (noteVarValue == 0) {
						editMultiValue0Field.lock()->setTextPadded(0, " ");
					}
					else if (noteVarValue < 0) {
						editMultiValue0Field.lock()->setText("-" + moduru::lang::StrUtil::padLeft(to_string(abs(noteVarValue)), " ", 3));
					}
					else if (noteVarValue > 0) {
						editMultiValue0Field.lock()->setText("+" + moduru::lang::StrUtil::padLeft(to_string(noteVarValue), " ", 3));
					}
				}

				if (seqGui->getChangeVariationTypeNumber() == 1 || seqGui->getChangeVariationTypeNumber() == 2) {
					auto noteVarValue = seqGui->getChangeVariationValue();
					if (noteVarValue > 100)
						noteVarValue = 100;

					editMultiValue0Field.lock()->setText(moduru::lang::StrUtil::padLeft(to_string(noteVarValue), " ", 3));
					editMultiValue0Field.lock()->setSize(3 * 6 * 2 + 2, 18);
					editMultiValue0Field.lock()->setLocation((90 + 12)/2, editMultiValue0Field.lock()->getY());
				}
				else if (seqGui->getChangeVariationTypeNumber() == 3) {
					editMultiValue0Field.lock()->setSize(4 * 6 * 2 + 2, 18);
					editMultiValue0Field.lock()->setLocation(90/2, editMultiValue0Field.lock()->getY());
					auto noteVarValue = seqGui->getChangeVariationValue() - 50;
					if (noteVarValue > 50)
						noteVarValue = 50;

					if (noteVarValue < 0) {
						editMultiValue0Field.lock()->setText("-" + moduru::lang::StrUtil::padLeft(to_string(abs(noteVarValue)), " ", 2));
					}
					if (noteVarValue > 0) {
						editMultiValue0Field.lock()->setText("+" + moduru::lang::StrUtil::padLeft(to_string(noteVarValue), " ", 2));
					}
					if (noteVarValue == 0) {
						editMultiValue0Field.lock()->setTextPadded("0", " ");
					}
				}
			}
			editMultiParam0Label.lock()->setSize(editMultiParam0Label.lock()->getText().length() * 6 * 2 + 2, 18);
			editMultiValue0Field.lock()->Hide(false);
			//editMultiValue0Field.lock()->setLocation((xPosSingle/2) + (editMultiParam0Label.lock()->GetRECT()->W()/2), yPosSingle/2);
		}
		if (seqGui->getParamLetter().compare("d") == 0 || seqGui->getParamLetter().compare("e") == 0) {
			updateDouble();
		}
	}

	if (dynamic_pointer_cast<NoteEvent>(event) && track.lock()->getBusNumber() == 0) {
		if (seqGui->getParamLetter().compare("a") == 0) {
			editMultiParam1Label.lock()->Hide(true);
			editMultiValue1Field.lock()->Hide(true);
			editMultiParam0Label.lock()->Hide(false);
			editMultiParam0Label.lock()->setLocation(xPosSingle/2, yPosSingle/2);
			editMultiParam0Label.lock()->setText(singleLabels[0]);
			editMultiValue0Field.lock()->setSize(8 * 6 * 2 + 2, 18);
			editMultiValue0Field.lock()->setText((moduru::lang::StrUtil::padLeft(to_string(seqGui->getChangeNoteToNumber()), " ", 3) + "(" + mpc::ui::Uis::noteNames[seqGui->getChangeNoteToNumber()]) + ")");
			editMultiParam0Label.lock()->setSize(editMultiParam0Label.lock()->GetTextEntryLength() * 6 * 2 + 2, 18);
			editMultiValue0Field.lock()->Hide(false);
			//editMultiValue0Field.lock()->setLocation((xPosSingle/2) + (editMultiParam0Label.lock()->GetRECT()->W()/2), yPosSingle/2);
		}
		else if (seqGui->getParamLetter().compare("b") == 0 || seqGui->getParamLetter().compare("c") == 0) {
			updateDouble();
		}
	}
	if (dynamic_pointer_cast<ProgramChangeEvent>(event)
		|| dynamic_pointer_cast<PolyPressureEvent>(event)
		|| dynamic_pointer_cast<ChannelPressureEvent>(event)
		|| dynamic_pointer_cast<ControlChangeEvent>(event)) {
		updateDouble();
	}
}

void StepWindowObserver::updateDouble()
{
    editMultiParam0Label.lock()->Hide(false);
    editMultiParam1Label.lock()->Hide(false);
    editMultiValue0Field.lock()->Hide(false);
    editMultiValue1Field.lock()->Hide(false);
    editMultiParam0Label.lock()->setText(doubleLabels[0]);
    editMultiParam1Label.lock()->setText(doubleLabels[1]);
	editMultiParam0Label.lock()->setSize(editMultiParam0Label.lock()->GetTextEntryLength() * 6 * 2 + 2, 18);
    editMultiParam0Label.lock()->setLocation(xPosDouble[0]/2, yPosDouble[0]/2);
    editMultiParam1Label.lock()->setSize(editMultiParam1Label.lock()->GetTextEntryLength() * 6 * 2 + 2, 18);
    editMultiParam1Label.lock()->setLocation(xPosDouble[1]/2, yPosDouble[1]/2);
    //editMultiValue0Field.lock()->setLocation((xPosDouble[0] + editMultiParam0Label.lock()->GetRECT()->W())/2, yPosDouble[0]/2);
    //editMultiValue1Field.lock()->setLocation((xPosDouble[1] + editMultiParam1Label.lock()->GetRECT()->W())/2, yPosDouble[1]/2);
    editMultiValue0Field.lock()->setText(editTypeNames[seqGui->getEditTypeNumber()]);
	editMultiValue1Field.lock()->setText(to_string(seqGui->getEditValue()));
    editMultiValue0Field.lock()->setSize(editMultiValue0Field.lock()->GetTextEntryLength() * 6 * 2 + 2, 18);
    editMultiValue1Field.lock()->setSize(editMultiValue1Field.lock()->GetTextEntryLength() * 6 * 2 + 2, 18);
}

StepWindowObserver::~StepWindowObserver() {
}
