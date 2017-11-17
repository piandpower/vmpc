#include <midi/message/NoteMsg.hpp>

#include <midi/core/MidiMessage.hpp>
#include <midi/message/ChannelMsg.hpp>

using namespace ctoot::midi::message;

NoteMsg::NoteMsg()
{
}

int& NoteMsg::noteOff()
{
	return noteOff_;
}
int NoteMsg::noteOff_ = ChannelMsg::NOTE_ON;

bool NoteMsg::isNote(::ctoot::midi::core::MidiMessage* msg)
{
    return isNote(getStatus(msg));
}

bool NoteMsg::isNote(int status)
{
	auto cmd = getCommand(status);
	return (cmd == NOTE_ON) || (cmd == NOTE_OFF);
}

ctoot::midi::core::MidiMessage* NoteMsg::on(int chan, int note, int vel) 
{
    return createChannel(NOTE_ON, chan, note, vel);
}

ctoot::midi::core::MidiMessage* NoteMsg::off(int chan, int note, int vel) 
{
    return createChannel(NOTE_OFF, chan, note, vel);
}

ctoot::midi::core::MidiMessage* NoteMsg::off(int chan, int note)
{
	return createChannel(noteOff_, chan, note, 0);
}

bool NoteMsg::isOn(ctoot::midi::core::MidiMessage* msg)
{
    return NoteMsg::isOn(getStatus(msg), getVelocity(msg));
}

bool NoteMsg::isOn(int status, int data2)
{
    return getCommand(status) == NOTE_ON && data2 != 0;
}

bool NoteMsg::isOff(::ctoot::midi::core::MidiMessage* msg)
{
    return isOff(getStatus(msg), getVelocity(msg));
}

bool NoteMsg::isOff(int status, int data2)
{
	return !isOn(status, data2);
}

int NoteMsg::getVelocity(::ctoot::midi::core::MidiMessage* msg)
{
	return getData2(msg);
}

ctoot::midi::core::MidiMessage* NoteMsg::setVelocity(::ctoot::midi::core::MidiMessage* msg, int vel)
{
	return setData2(msg, vel);
}

ctoot::midi::core::MidiMessage* NoteMsg::louden(::ctoot::midi::core::MidiMessage* msg, int velocityDelta)
{
	auto velocity = getData2(msg) + velocityDelta;
	if (velocity > 127) velocity = 127;
	else if (velocity < 0) velocity = 0;
	return setData2(msg, velocity);
}