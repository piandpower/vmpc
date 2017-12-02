#include <file/all/MidiInput.hpp>

#include <Mpc.hpp>
#include <ui/Uis.hpp>
#include <ui/sequencer/window/MultiRecordingSetupLine.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <sequencer/Sequencer.hpp>

using namespace mpc::file::all;
using namespace std;

MidiInput::MidiInput(vector<char> b) 
{
	receiveCh = b[RECEIVE_CH_OFFSET];
	sustainPedalToDuration = b[SUSTAIN_PEDAL_TO_DURATION_OFFSET] > 0;
	filterEnabled = b[FILTER_ENABLED_OFFSET] > 0;
	filterType = b[FILTER_TYPE_OFFSET];
	multiRecEnabled = b[MULTI_REC_ENABLED_OFFSET] > 0;
	for (int i = 0; i < MULTI_REC_TRACK_DESTS_LENGTH; i++)
		multiRecTrackDests[i] = b[MULTI_REC_TRACK_DESTS_OFFSET + i] - 1;

	notePassEnabled = b[NOTE_PASS_ENABLED_OFFSET] > 0;
	pitchBendPassEnabled = b[PITCH_BEND_PASS_ENABLED_OFFSET] > 0;
	pgmChangePassEnabled = b[PGM_CHANGE_PASS_ENABLED_OFFSET] > 0;
	chPressurePassEnabled = b[CH_PRESSURE_PASS_ENABLED_OFFSET] > 0;
	polyPressurePassEnabled = b[POLY_PRESSURE_PASS_ENABLED_OFFSET] > 0;
	exclusivePassEnabled = b[EXCLUSIVE_PASS_ENABLED_OFFSET] > 0;
}

MidiInput::MidiInput(mpc::Mpc* mpc)
{
	saveBytes = vector<char>(LENGTH);
	for (int i = 0; i < LENGTH; i++)
		saveBytes[i] = TEMPLATE[i];
	auto swgui = mpc->getUis().lock()->getSequencerWindowGui();
	saveBytes[RECEIVE_CH_OFFSET] = static_cast<int8_t>(swgui->getReceiveCh());
	saveBytes[SUSTAIN_PEDAL_TO_DURATION_OFFSET] = static_cast<int8_t>(swgui->isSustainPedalToDurationEnabled() ? 1 : 0);
	saveBytes[FILTER_ENABLED_OFFSET] = static_cast<int8_t>((swgui->isMidiFilterEnabled() ? 1 : 0));
	saveBytes[FILTER_TYPE_OFFSET] = static_cast<int8_t>(swgui->getMidiFilterType());
	saveBytes[MULTI_REC_ENABLED_OFFSET] = static_cast<int8_t>(mpc->getSequencer().lock()->isRecordingModeMulti() ? 1 : 0);
	for (int i = 0; i < MULTI_REC_TRACK_DESTS_LENGTH; i++)
		saveBytes[MULTI_REC_TRACK_DESTS_OFFSET + i] = static_cast<int8_t>(swgui->getMrsLines()[i]->getTrack() + 1);

	saveBytes[NOTE_PASS_ENABLED_OFFSET] = static_cast<int8_t>(swgui->isNotePassEnabled() ? 1 : 0);
	saveBytes[PITCH_BEND_PASS_ENABLED_OFFSET] = static_cast<int8_t>(swgui->isPitchBendPassEnabled() ? 1 : 0);
	saveBytes[PGM_CHANGE_PASS_ENABLED_OFFSET] = static_cast<int8_t>(swgui->isPgmChangePassEnabled() ? 1 : 0);
	saveBytes[CH_PRESSURE_PASS_ENABLED_OFFSET] = static_cast<int8_t>(swgui->isChPressurePassEnabled() ? 1 : 0);
	saveBytes[POLY_PRESSURE_PASS_ENABLED_OFFSET] = static_cast<int8_t>(swgui->isPolyPressurePassEnabled() ? 1 : 0);
	saveBytes[EXCLUSIVE_PASS_ENABLED_OFFSET] = static_cast<int8_t>(swgui->isExclusivePassEnabled() ? 1 : 0);
}

vector<char> MidiInput::TEMPLATE = vector<char>{ 127, 64, 1, 0, 1, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 1, 0, 0, 0, 0, 0 };
const int MidiInput::LENGTH;
const int MidiInput::RECEIVE_CH_OFFSET;
const int MidiInput::SUSTAIN_PEDAL_TO_DURATION_OFFSET;
const int MidiInput::FILTER_ENABLED_OFFSET;
const int MidiInput::FILTER_TYPE_OFFSET;
const int MidiInput::MULTI_REC_ENABLED_OFFSET;
const int MidiInput::MULTI_REC_TRACK_DESTS_OFFSET;
const int MidiInput::MULTI_REC_TRACK_DESTS_LENGTH;
const int MidiInput::NOTE_PASS_ENABLED_OFFSET;
const int MidiInput::PITCH_BEND_PASS_ENABLED_OFFSET;
const int MidiInput::PGM_CHANGE_PASS_ENABLED_OFFSET;
const int MidiInput::CH_PRESSURE_PASS_ENABLED_OFFSET;
const int MidiInput::POLY_PRESSURE_PASS_ENABLED_OFFSET;
const int MidiInput::EXCLUSIVE_PASS_ENABLED_OFFSET;

int MidiInput::getReceiveCh()
{
    return receiveCh;
}

bool MidiInput::isSustainPedalToDurationEnabled()
{
    return sustainPedalToDuration;
}

bool MidiInput::isFilterEnabled()
{
    return filterEnabled;
}

int MidiInput::getFilterType()
{
    return filterType;
}

bool MidiInput::isMultiRecEnabled()
{
    return multiRecEnabled;
}

vector<int> MidiInput::getMultiRecTrackDests()
{
    return multiRecTrackDests;
}

bool MidiInput::isNotePassEnabled()
{
    return notePassEnabled;
}

bool MidiInput::isPitchBendPassEnabled()
{
    return pitchBendPassEnabled;
}

bool MidiInput::isPgmChangePassEnabled()
{
    return pgmChangePassEnabled;
}

bool MidiInput::isChPressurePassEnabled()
{
    return chPressurePassEnabled;
}

bool MidiInput::isPolyPressurePassEnabled()
{
    return polyPressurePassEnabled;
}

bool MidiInput::isExclusivePassEnabled()
{
    return exclusivePassEnabled;
}

vector<char> MidiInput::getBytes()
{
    return saveBytes;
}
