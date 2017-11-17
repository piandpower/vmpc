#include <audio/system/MixerConnectedAudioSystem.hpp>

#include <audio/core/AudioControlsChain.hpp>
#include <audio/mixer/AudioMixer.hpp>
#include <audio/mixer/AudioMixerStrip.hpp>
#include <audio/mixer/MixerControls.hpp>
#include <audio/mixer/MixerControlsIds.hpp>
#include <audio/system/AudioConnection.hpp>
#include <audio/system/AudioDevice.hpp>
#include <audio/system/AudioOutput.hpp>
#include <audio/system/MixerInputConnection.hpp>

using namespace ctoot::audio::system;
using namespace std;

MixerConnectedAudioSystem::MixerConnectedAudioSystem(shared_ptr<ctoot::audio::mixer::AudioMixer> mixer) 
{
	this->mixer = mixer;
	mixerControls = mixer->getMixerControls();
}

void MixerConnectedAudioSystem::notifyObservers(boost::any obj)
{
	AudioDevice* adCandidate;
	AudioOutput* aoCandidate;
	try {
		adCandidate = boost::any_cast<AudioDevice*>(obj);
	}
	catch (boost::bad_any_cast e) {
		try {
			aoCandidate = boost::any_cast<AudioOutput*>(obj);
		}
		catch (boost::bad_any_cast e) {
			e.what();
			return;
		}
	}

	if (adCandidate != nullptr) {
		auto device = adCandidate;
		bool added = false;
		for (auto& ad : getAudioDevices()) {
			if (ad == device) {
				added = true;
				break;
			}
		}
		auto outputs = device->getAudioOutputs();
		for (auto& output : outputs) {
			if (added) {
				if (autoConnect) {
					createConnectionFrom(output);
				}
			}
			else {
				closeConnectionFrom(output);
			}
		}
	}
	else if (aoCandidate != nullptr) {
		auto output = boost::any_cast<AudioOutput*>(obj);
		auto added = false;
		for (auto& ao : getAudioOutputs()) {
			if (ao == output) {
				added = true;
				break;
			}
		}
		if (added) {
			if (autoConnect) {
				createConnectionFrom(output);
			}
		}
		else {
			closeConnectionFrom(output);
		}
	}
	super::notifyObservers(obj);
}

vector<AudioConnection*>* MixerConnectedAudioSystem::getConnections()
{
	return &connections;
}

void MixerConnectedAudioSystem::createConnection(string fromPortName, string fromPortLocation, string toPortName, int flags)
{
	auto from = getOutputPort(fromPortName, fromPortLocation);
	if (from == nullptr) {
		//        throw new ::java::lang::IllegalArgumentException(stringBuilder().append(fromPortName)->append(" @ ")
		//            ->append(fromPortLocation)
		//            ->append(" does not exist")->toString());
		return;
	}
	auto to = mixer.lock()->getStrip(toPortName);
	if (to.lock() == nullptr) {
		//        throw new ::java::lang::IllegalArgumentException(stringBuilder().append(toPortName)->append(" does not exist")->toString());
		return;
	}
	createConnection(from, to.lock().get(), flags);
}

void MixerConnectedAudioSystem::createConnection(AudioOutput* from, ctoot::audio::mixer::AudioMixerStrip* to, int flags)
{
	if (getConnectionFrom(from->getName(), from->getLocation()) != nullptr) {
		return;
	}
	AudioConnection* connection = new MixerInputConnection(this, from, to, flags);
	connections.push_back(connection);
	setChanged();
	super::notifyObservers();
}

AudioConnection* MixerConnectedAudioSystem::getConnectionFrom(string from, string fromLocation)
{
	for (auto& c : connections) {
		if (c->getOutputName().compare(from) == 0 && c->getOutputLocation().compare(fromLocation) == 0) {
			return c;
		}
	}
	return nullptr;
}

void MixerConnectedAudioSystem::createConnectionFrom(AudioOutput* output)
{
	shared_ptr<ctoot::audio::core::AudioControlsChain> stripControls;
	shared_ptr<ctoot::audio::core::AudioControlsChain> namedControls;
	try {
		auto strip = mixer.lock()->getUnusedChannelStrip().lock();
		if (strip == nullptr) {
			auto i = -1;
			auto max = 1 + mixerControls.lock()->getControls().size();
			string name;
			do {
				stripControls = mixerControls.lock()->getStripControls(ctoot::audio::mixer::MixerControlsIds::CHANNEL_STRIP, ++i);
				name = to_string(i + 1);
				namedControls = mixerControls.lock()->getStripControls(name);
			} while (stripControls != nullptr && namedControls != nullptr && i < max);
			mixerControls.lock()->createStripControls(ctoot::audio::mixer::MixerControlsIds::CHANNEL_STRIP, i, name);
			strip = mixer.lock()->getStrip(name).lock();
			if (strip == nullptr) {
				//string error = "Failed to create mixer strip for " + nameAndLocation(output);
				return;
			}
		}
		createConnection(output, strip.get(), 0);
	}
	catch (exception* e) {
		e->what();
	}
}

void MixerConnectedAudioSystem::closeConnectionFrom(AudioOutput* output)
{
	auto name = output->getName();
	auto location = output->getLocation();
	auto connection = getConnectionFrom(name, location);
	if (connection == nullptr) {
		//        npc(::java::lang::System::err())->println(stringBuilder().append("Failed to close connection from ")->append(name)
		//            ->append(" @ ")
		//            ->append(location)->toString());
		return;
	}
	closeConnection(connection);
}

void MixerConnectedAudioSystem::closeConnection(AudioConnection* connection)
{
	connection->close();
	int index = 0;
	for (auto& c : connections) {
		if (c == connection)
			break;
		index++;
	}
	if (index >= connections.size()) return;
	connections.erase(connections.begin() + index);
	setChanged();
	super::notifyObservers();
}

AudioOutput* MixerConnectedAudioSystem::getOutputPort(string name, string location)
{
	for (auto& device : devices) {
		auto outputs = device->getAudioOutputs();
		for (auto& output : outputs) {
			if (name.compare(output->getName()) == 0 && location.compare(output->getLocation()) == 0) {
				return output;
			}
		}
	}
	return nullptr;
	//    throw new ::java::lang::IllegalArgumentException(stringBuilder().append("AudioOutput ")->append(name)
	//        ->append(" @ ")
	//        ->append(location)
	//        ->append(" not found")->toString());
}

string MixerConnectedAudioSystem::nameAndLocation(AudioOutput* output)
{
	string result = output->getName();
	result.append(" @ ");
	result.append(output->getLocation());
	return result;
}

void MixerConnectedAudioSystem::addAudioDevice(AudioDevice* device)
{
    DefaultAudioSystem::addAudioDevice(device);
}

void MixerConnectedAudioSystem::close()
{
    DefaultAudioSystem::close();
}

vector<AudioDevice*> MixerConnectedAudioSystem::getAudioDevices()
{
    return DefaultAudioSystem::getAudioDevices();
}

vector<AudioInput*> MixerConnectedAudioSystem::getAudioInputs()
{
    return DefaultAudioSystem::getAudioInputs();
}

vector<AudioOutput*> MixerConnectedAudioSystem::getAudioOutputs()
{
    return DefaultAudioSystem::getAudioOutputs();
}

void MixerConnectedAudioSystem::removeAudioDevice(AudioDevice* device)
{
    DefaultAudioSystem::removeAudioDevice(device);
}

void MixerConnectedAudioSystem::setAutoConnect(bool autoConnect)
{
    DefaultAudioSystem::setAutoConnect(autoConnect);
}

MixerConnectedAudioSystem::~MixerConnectedAudioSystem() {
	for (auto& c : connections) {
		if (c != nullptr) {
			c->close();
			delete c;
		}
	}
	connections.clear();
}
