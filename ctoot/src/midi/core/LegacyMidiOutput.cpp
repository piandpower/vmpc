#include <midi/core/LegacyMidiOutput.hpp>

//#include <midi/core/LegacyMidiOutput_DeviceMidiOutput_1.hpp>
#include <midi/core/LegacyDeviceAdaptor.hpp>
#include <midi/core/MidiDevice.hpp>
#include <midi/core/ShortMessage.hpp>

using namespace ctoot::midi::core;
using namespace std;

LegacyMidiOutput::LegacyMidiOutput(LegacyDeviceAdaptor* adaptor, string name)
	: DefaultMidiOutput(LegacyDeviceAdaptor::simpleName(name))
{
	this->adaptor = adaptor;
	this->name = name;
	/*tx = new RtMidiIn();
	int portIndex = -1;
	for (auto i = 0; i < tx->getPortCount(); i++) {
		if (tx->getPortName(i).compare(name) == 0) {
			portIndex = i;
			break;
		}
	}
	if (portIndex != -1) tx->openPort(portIndex);*/
}

void LegacyMidiOutput::processQueue() {
	vector<unsigned char> msg;
	//while (tx->getMessage(&msg) > 0) {
	//	vector<char> convmsg;
	//	for (auto& c : msg)
	//		convmsg.push_back(c & 0xFF);
	//	auto midiMsg = ctoot::midi::core::ShortMessage(convmsg);
	//	transport(&midiMsg, 0);
	//}
}

void LegacyMidiOutput::closeOutput()
{
	//tx->closePort();
}

void LegacyMidiOutput::addConnectionTo(MidiInput* input)
{
    DefaultMidiOutput::addConnectionTo(input);
    if(getConnectionCount() > 0 && !adaptor->isOpen()) {
        try {
            adaptor->open();
        } catch (const std::exception& e) {
        }
    }
}
