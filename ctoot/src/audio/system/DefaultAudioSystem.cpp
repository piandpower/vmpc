#include <audio/system/DefaultAudioSystem.hpp>
#include <audio/system/AudioDevice.hpp>
#include <audio/system/AudioSystemObserver.hpp>

using namespace ctoot::audio::system;
using namespace std;

DefaultAudioSystem::DefaultAudioSystem() 
{
	autoConnect = true;
	observer = new AudioSystemObserver(this);
}

void DefaultAudioSystem::addAudioDevice(AudioDevice* device)
{
    //checkUniqueDeviceName(device);
	//devices.push_back(device);
    //setChanged();
    //notifyObservers(device);
    //device->addObserver(observer);
}

void DefaultAudioSystem::checkUniqueDeviceName(AudioDevice* device)
{
	for (auto& d : devices) {
		if (d->getName().compare(device->getName()) == 0) {
			string exc = "An AudioDevice named " + device->getName() + " already exists";
			throw std::invalid_argument(exc.c_str());
		}
	}
}

void DefaultAudioSystem::removeAudioDevice(AudioDevice* device)
{
	device->deleteObserver(observer);
	int index = 0;
	for (auto& d : devices) {
		if (d == device) break;
		index++;
	}
	if (index >= devices.size()) return;
	devices.erase(devices.begin() + index);
	setChanged();
	notifyObservers(device);
}

vector<AudioDevice*> DefaultAudioSystem::getAudioDevices()
{
	return devices;
}

vector<AudioInput*> DefaultAudioSystem::getAudioInputs()
{
	vector<AudioInput*> inputs;
	for (auto& device : devices) {
		{
			for (auto& i : device->getAudioInputs())
				inputs.push_back(i);
		}
	}
	return inputs;
}

vector<AudioOutput*> DefaultAudioSystem::getAudioOutputs()
{
	vector<AudioOutput*> outputs;
	for (auto& device : devices) {
		{
			for (auto& output : device->getAudioOutputs())
				outputs.push_back(output);
		}
	}
	return outputs;
}

void DefaultAudioSystem::setAutoConnect(bool autoConnect)
{
    this->autoConnect = autoConnect;
}

void DefaultAudioSystem::close()
{
	for (auto& device : devices)
		device->closeAudio();
}

DefaultAudioSystem::~DefaultAudioSystem() {
	if (observer != nullptr) {
		delete observer;
		observer = nullptr;
	}
	for (auto& d : devices) {
		if (d != nullptr) {
			delete d;
			d = nullptr;
		}
	}
	//devices.clear();
}
