#include <midi/core/AbstractMidiDevice.hpp>

//#include <midi/core/MidiInput.hpp>
//#include <midi/core/MidiOutput.hpp>

using namespace ctoot::midi::core;
using namespace std;

AbstractMidiDevice::AbstractMidiDevice(string name) 
{
	this->name = name;
}

vector<MidiInput*> AbstractMidiDevice::getMidiInputs()
{
	return inputs;
}

vector<MidiOutput*> AbstractMidiDevice::getMidiOutputs()
{
	return outputs;
}

string AbstractMidiDevice::getName()
{
    return name;
}

string AbstractMidiDevice::toString()
{
    return name;
}

void AbstractMidiDevice::addMidiInput(MidiInput* input)
{
	inputs.push_back(input);
	setChanged();
	notifyObservers();
}

void AbstractMidiDevice::removeMidiInput(MidiInput* input)
{
	int counter = 0;
	for (auto& i : inputs) {
		if (i == input) {
			break;
		}
		counter++;
	}
	if (counter >= inputs.size()) return;
	inputs.erase(inputs.begin() + counter);
	setChanged();
	notifyObservers();
}

void AbstractMidiDevice::removeAllMidiInputs()
{
	inputs.clear();
	setChanged();
	notifyObservers();
}

void AbstractMidiDevice::addMidiOutput(MidiOutput* output)
{
    outputs.push_back(output);
    setChanged();
    notifyObservers();
}

void AbstractMidiDevice::removeMidiOutput(MidiOutput* output)
{
	int counter = 0;
	for (auto& o : outputs) {
		if (o == output) {
			break;
		}
		counter++;
	}
	if (counter >= outputs.size()) return;
	outputs.erase(outputs.begin() + counter);
    setChanged();
    notifyObservers();
}

void AbstractMidiDevice::removeAllMidiOutputs()
{
    outputs.clear();
    setChanged();
    notifyObservers();
}

AbstractMidiDevice::AbstractMidiDevice() {
	
}
