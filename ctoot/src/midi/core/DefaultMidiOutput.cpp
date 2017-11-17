#include <midi/core/DefaultMidiOutput.hpp>

#include <midi/core/MidiInput.hpp>

using namespace ctoot::midi::core;
using namespace std;

DefaultMidiOutput::DefaultMidiOutput(string name) 
{
	this->name = name;
}

void DefaultMidiOutput::addConnectionTo(MidiInput* input)
{
	tos.push_back(input);
}

void DefaultMidiOutput::removeConnectionTo(MidiInput* input)
{
	for (int i = (int) (tos.size()) - 1; i >= 0; i--) {
		if (tos[i] == input) {
			tos.erase(tos.begin() + i);
			break;
		}
	}
}

int DefaultMidiOutput::getConnectionCount()
{
	return (int)(tos.size());
}

void DefaultMidiOutput::transport(ctoot::midi::core::MidiMessage* msg, int timestamp)
{
	for (auto& to : tos) {
		to->transport(msg, timestamp);
	}
}

void DefaultMidiOutput::setName(string name)
{
    this->name = name;
}

string DefaultMidiOutput::getName()
{
    return name;
}

string DefaultMidiOutput::toString()
{
    return name;
}
