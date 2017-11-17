#include <midi/core/DefaultMidiSystem.hpp>

#include <midi/core/MidiSystemObserver.hpp>
#include <midi/core/MidiDevice.hpp>

#include <midi/core/LegacyMidiOutput.hpp>

using namespace ctoot::midi::core;
using namespace std;

DefaultMidiSystem::DefaultMidiSystem() 
{
	observer = new MidiSystemObserver(this);
}

void DefaultMidiSystem::setEnabled(bool enabled) {
}

void DefaultMidiSystem::work(int nFrames) {
	for (auto& o : getMidiOutputs()) {
		auto candidate = dynamic_cast<LegacyMidiOutput*>(o);
		if (candidate != nullptr) {
			candidate->processQueue();
		}
	}
}

void DefaultMidiSystem::addMidiDevice(MidiDevice* device)
{
    devices.push_back(device);
    setChanged();
    notifyObservers();
    device->addObserver(observer);
}

void DefaultMidiSystem::removeMidiDevice(MidiDevice* device)
{
	device->deleteObserver(observer);
	for (int i = 0; i < devices.size(); i++) {
		if (devices[i] == device) {
			devices.erase(devices.begin() + i);
			break;
		}
	}
	setChanged();
	notifyObservers();
}

vector<MidiDevice*> DefaultMidiSystem::getMidiDevices()
{
    return devices;
}

vector<MidiInput*> DefaultMidiSystem::getMidiInputs()
{
	vector<MidiInput*> inputs;
	for (auto& device : devices) {
		for (auto& i : device->getMidiInputs())
			inputs.push_back(i);
	}
	return inputs;
}

vector<MidiOutput*> DefaultMidiSystem::getMidiOutputs()
{
	vector<MidiOutput*> outputs;
    for (auto& device : devices) {
		for (auto& o : device->getMidiOutputs())
			outputs.push_back(o);
    }
    return outputs;
}

void DefaultMidiSystem::close()
{
	try {
		for (auto& device : devices) {
			device->closeMidi();
		}
	}
	catch (std::exception& e) {
		auto msg = e.what();
		auto msgStr = string(msg);
	}
}

DefaultMidiSystem::~DefaultMidiSystem() {
	devices.clear();
	delete observer;
}
