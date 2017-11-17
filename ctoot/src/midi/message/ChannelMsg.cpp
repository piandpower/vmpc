#include <midi/message/ChannelMsg.hpp>

#include <midi/core/MidiMessage.hpp>
#include <midi/core/ShortMessage.hpp>

using namespace ctoot::midi::message;

ChannelMsg::ChannelMsg()
{
}

bool ChannelMsg::isChannel(ctoot::midi::core::MidiMessage* msg)
{
    return isChannel(getStatus(msg));
}

bool ChannelMsg::isChannel(int status)
{
    return status < 240;
}

ctoot::midi::core::MidiMessage* ChannelMsg::createChannel(int status, int chan, int data1, int data2) /* throws(InvalidMidiDataException) */
{
    auto msg = dynamic_cast< ctoot::midi::core::ShortMessage* >(fastShortPrototype()->clone());
    msg->setMessage(status, chan, data1, data2);
    return msg;
}

ctoot::midi::core::MidiMessage* ChannelMsg::createChannel(int status, int chan, int data1) /* throws(InvalidMidiDataException) */
{
    return createChannel(status, chan, data1, 0);
}

int ChannelMsg::getCommand(ctoot::midi::core::MidiMessage* msg)
{
    
    return dynamic_cast< ctoot::midi::core::ShortMessage* >(msg)->getCommand();
}

int ChannelMsg::getCommand(int status)
{
    return (status & int32_t(240));
}

int ChannelMsg::getChannel(ctoot::midi::core::MidiMessage* msg)
{   
    return dynamic_cast< ctoot::midi::core::ShortMessage* >(msg)->getChannel();
}

int ChannelMsg::getChannel(int status)
{
    
    return status & int32_t(15);
}

ctoot::midi::core::MidiMessage* ChannelMsg::setChannel(ctoot::midi::core::MidiMessage* msg, int chan) /* throws(InvalidMidiDataException) */
{
    dynamic_cast< ctoot::midi::core::ShortMessage* >(msg)->setMessage(getCommand(msg), chan, getData1(msg), getData2(msg));
    return msg;
}

int ChannelMsg::setChannel(int status, int chan)
{   
    return (status & int32_t(240)) | (chan & int32_t(15));
}
/*
const int ChannelMsg::NOTE_OFF;
const int ChannelMsg::NOTE_ON;
const int ChannelMsg::POLY_PRESSURE;
const int ChannelMsg::CONTROL_CHANGE;
const int ChannelMsg::PROGRAM_CHANGE;
const int ChannelMsg::CHANNEL_PRESSURE;
const int ChannelMsg::PITCH_BEND;
*/