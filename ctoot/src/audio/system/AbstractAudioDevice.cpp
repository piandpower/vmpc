#include <audio/system/AbstractAudioDevice.hpp>
#include <audio/system/AudioInput.hpp>
#include <audio/system/AudioOutput.hpp>

using namespace ctoot::audio::system;
using namespace std;

AbstractAudioDevice::AbstractAudioDevice(string name)
{
	this->name = name;
}

vector<AudioInput*> AbstractAudioDevice::getAudioInputs()
{
	return inputs;
}

vector<AudioOutput*> AbstractAudioDevice::getAudioOutputs()
{
    return outputs;
}

string AbstractAudioDevice::getName()
{
    return name;
}

string AbstractAudioDevice::toString()
{
	return name;
}

void AbstractAudioDevice::addAudioInput(AudioInput* input)
{
	inputs.push_back(input);
	setChanged();
	notifyObservers(input);
}

void AbstractAudioDevice::removeAudioInput(AudioInput* input)
{
	int counter = 0;
	for (auto& i : inputs) {
		if (i == input)
			break;
		counter++;
	}
	if (counter >= inputs.size()) return;
	inputs.erase(inputs.begin() + counter);
	setChanged();
	notifyObservers(input);
}

void AbstractAudioDevice::addAudioOutput(AudioOutput* output)
{
	outputs.push_back(output);
	setChanged();
	notifyObservers(output);
}

void AbstractAudioDevice::removeAudioOutput(AudioOutput* output)
{
	int counter = 0;
	for (auto& o : outputs) {
		if (o == output)
			break;
		counter++;
	}
	if (counter >= outputs.size()) return;
	outputs.erase(outputs.begin() + counter);
	setChanged();
	notifyObservers(output);
}

AbstractAudioDevice::~AbstractAudioDevice() {
	inputs.clear();
	outputs.clear();
}
