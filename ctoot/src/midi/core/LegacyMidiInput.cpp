#include <midi/core/LegacyMidiInput.hpp>
#include <midi/core/LegacyDeviceAdaptor.hpp>

#include <vector>

//#include <RtMidi.h>

using namespace ctoot::midi::core;
using namespace std;

LegacyMidiInput::LegacyMidiInput(LegacyDeviceAdaptor* adaptor, string name)  
{
	//this->adaptor = adaptor;
	//this->name = name;
	//receiver = new RtMidiOut();
	//int portIndex = -1;
	//for (auto i = 0; i < receiver->getPortCount(); i++) {
	//	if (receiver->getPortName(i).compare(name) == 0) {
	//		portIndex = i;
	//		break;
	//	}
	//}
	//if (portIndex != -1) receiver->openPort(portIndex);
}

void LegacyMidiInput::transport(ctoot::midi::core::MidiMessage* message, int timestamp)
{
	if (!adaptor->isOpen()) {
		try {
			adaptor->open();
		}
		catch (const std::exception &mua) {
		}
	}
	vector<unsigned char> msgData;
	for (char c : *message->getMessage())
		msgData.push_back(c & 0xFF);

	//receiver->sendMessage(&msgData);
}

void LegacyMidiInput::closeInput()
{
	//receiver->closePort();
}

int32_t LegacyMidiInput::getLatency()
{
	/*
    if(dynamic_cast< ::javax::sound::midi::Synthesizer* >(device) != nullptr) {
        return static_cast< int32_t >(npc((java_cast< ::javax::sound::midi::Synthesizer* >(device)))->getLatency());
    }
	*/
    return 0;
}

string LegacyMidiInput::getName()
{
    return name;
}
