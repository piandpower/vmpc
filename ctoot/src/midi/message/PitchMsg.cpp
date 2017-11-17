#include <midi/message/PitchMsg.hpp>
#include <midi/core/MidiMessage.hpp>

using namespace ctoot::midi::message;

PitchMsg::PitchMsg()
{
}

bool PitchMsg::isPitch(::ctoot::midi::core::MidiMessage* msg)
{
    return isPitch(getStatus(msg));
}

bool PitchMsg::isPitch(int status)
{
    auto cmd = getCommand(status);
    return (cmd == NOTE_ON) || (cmd == NOTE_OFF) || (cmd == POLY_PRESSURE);
}

ctoot::midi::core::MidiMessage* PitchMsg::transpose(::ctoot::midi::core::MidiMessage* msg, int semitones)
{
	auto note = getData1(msg) + semitones;
	if (note > 127) note = 127;
	else if (note < 0) note = 0;
	return setData1(msg, note);
}

int PitchMsg::getPitch(::ctoot::midi::core::MidiMessage* msg)
{
	return getData1(msg);
}

ctoot::midi::core::MidiMessage* PitchMsg::setPitch(::ctoot::midi::core::MidiMessage* msg, int pitch)
{
	return setData1(msg, pitch);
}