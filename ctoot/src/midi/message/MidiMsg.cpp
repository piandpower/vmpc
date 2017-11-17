#include <midi/message/MidiMsg.hpp>

using namespace ctoot::midi::message;
using namespace std;

MidiMsg::MidiMsg()
{
}

int MidiMsg::getStatus(ctoot::midi::core::MidiMessage* msg)
{
    return msg->getStatus();
}

vector<char>* MidiMsg::getMessage(ctoot::midi::core::MidiMessage* msg)
{
    return msg->getMessage();
}

int MidiMsg::getLength(ctoot::midi::core::MidiMessage* msg)
{
	return msg->length;
}