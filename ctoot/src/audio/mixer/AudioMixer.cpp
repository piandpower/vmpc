#include <audio/mixer/AudioMixer.hpp>
#include <audio/core/AudioBuffer.hpp>
#include <audio/core/AudioControlsChain.hpp>
#include <audio/core/AudioProcess.hpp>
//#include <audio/core/Taps.hpp>
//#include <audio/mixer/AudioMixer_DefaultMixerInterconnection.hpp>
#include <audio/mixer/MixerControlsObserver.hpp>
#include <audio/mixer/AudioMixerBus.hpp>
#include <audio/mixer/AudioMixerStrip.hpp>
#include <audio/mixer/BusControls.hpp>
#include <audio/mixer/Mutation.hpp>
#include <audio/mixer/MixerControls.hpp>
#include <audio/mixer/MixerControlsIds.hpp>
#include <audio/server/AudioServer.hpp>
#include <control/CompoundControl.hpp>
#include <control/Control.hpp>

#include <time.h>
#include <chrono>
#include <thread>

using namespace std;
using namespace ctoot::audio::server;
using namespace ctoot::audio::mixer;

AudioMixer::AudioMixer(shared_ptr<MixerControls> controls, shared_ptr<AudioServer> server)
{
	this->controls = controls;
	this->server = server;
	//Taps::setAudioServer(server);
	sharedAudioBuffer = server->createAudioBuffer("Mixer (shared)");
	createBusses(controls);
	createStrips(controls);
	observer = make_unique<MixerControlsObserver>(this);
	controls->addObserver(observer.get());
}

shared_ptr<MixerControls> AudioMixer::getMixerControls()
{
    return controls;
}

ctoot::audio::core::AudioBuffer* AudioMixer::getSharedBuffer()
{
    return sharedAudioBuffer;
}

ctoot::audio::core::AudioBuffer* AudioMixer::createBuffer(string name)
{
    return server.lock()->createAudioBuffer(name);
}

void AudioMixer::removeBuffer(ctoot::audio::core::AudioBuffer* buffer)
{
    server.lock()->removeAudioBuffer(buffer);
}

bool AudioMixer::isMutating()
{
	return !mutationQueue.size_approx() == 0;
}

void AudioMixer::waitForMutations()
{
	processMutations();
	/* vMPC doesn't need dynamic mutations, only during init.
	while (isMutating()) {
        if(isEnabled() && server->isRunning()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5));
        } else {
            processMutations();
        }
    }
	if(isEnabled() && server->isRunning()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
	*/
}

weak_ptr<AudioMixerStrip> AudioMixer::getStrip(string name)
{
    waitForMutations();
    return getStripImpl(name);
}

weak_ptr<AudioMixerStrip> AudioMixer::getStripImpl(string name)
{
	for (auto& strip : strips) {
		if (strip->getName().compare(name) == 0) {
			return strip;
		}
	}
	return weak_ptr<AudioMixerStrip>();
}

vector<shared_ptr<AudioMixerStrip>> AudioMixer::getStrips()
{
    waitForMutations();
	return strips;
}

weak_ptr<AudioMixerStrip> AudioMixer::getUnusedChannelStrip()
{
	for (auto& strip : channelStrips) {
		if (strip->getInputProcess() == nullptr)
			return strip;
	}
	return weak_ptr<AudioMixerStrip>();
}

void AudioMixer::work(int nFrames)
{
    if(!enabled) return;
	processMutations();
	silenceStrips(groupStrips);
	silenceStrips(fxStrips);
    silenceStrips(auxStrips);
	mainStrip->silence();
	evaluateStrips(channelStrips, nFrames);
	evaluateStrips(groupStrips, nFrames);
    evaluateStrips(fxStrips, nFrames);
    evaluateStrips(auxStrips, nFrames);
	mainStrip->processBuffer(nFrames);
    writeBusBuffers(nFrames);
}

void AudioMixer::processMutations()
{
	Mutation* m = nullptr;
	mutationQueue.try_dequeue(m);
	if (m == nullptr) return;
	processMutation(m);
}

void AudioMixer::processMutation(Mutation* m)
{
	auto control = m->getControl();
	auto mcast = dynamic_pointer_cast<core::AudioControlsChain>(control);
	if (mcast == nullptr) return;
	switch (m->getOperation()) {
	case Mutation::ADD:
		createStrip(mcast);
		break;
	case Mutation::REMOVE:
		removeStrip(mcast);
		break;
	}
	delete m;
}

void AudioMixer::evaluateStrips(vector<shared_ptr<AudioMixerStrip>> strips, int nFrames)
{
	for (auto& strip : strips)
		strip->processBuffer(nFrames);
}

void AudioMixer::silenceStrips(vector<shared_ptr<AudioMixerStrip>> strips)
{
	for (auto& strip : strips)
		strip->silence();
}

void AudioMixer::writeBusBuffers(int nFrames)
{
	for (auto& bus : busses)
		bus->write(nFrames);
}

void AudioMixer::createBusses(shared_ptr<MixerControls> mixerControls)
{
	busses.clear();
	auxBusses.clear();
	fxBusses.clear();
	shared_ptr<AudioMixerBus> bus;

	for (auto& bc : mixerControls->getAuxBusControls()) {
		bus = createBus(bc);
		busses.push_back(bus);
		auxBusses.push_back(bus);
	}

	for (auto& bc : mixerControls->getFxBusControls()) {
		bus = createBus(bc);
		busses.push_back(bus);
		fxBusses.push_back(bus);
	}

	mainBus = createBus(mixerControls->getMainBusControls());
	busses.push_back(mainBus);
}

shared_ptr<AudioMixerBus> AudioMixer::createBus(weak_ptr<BusControls> busControls)
{
	return make_shared<AudioMixerBus>(this, busControls);
}

shared_ptr<AudioMixerBus> AudioMixer::getBus(string name)
{
	for (auto& bus : busses) {
		if (bus->getName().compare(name) == 0) {
			return bus;
		}
	}
	return nullptr;
}

shared_ptr<AudioMixerBus> AudioMixer::getMainBus()
{
    return mainBus;
}

shared_ptr<AudioMixerStrip> AudioMixer::getMainStrip()
{
	if (mainStrip == nullptr) {
		//printf("getMainStrip() called before mainStrip set");
		return nullptr;
	}
	return mainStrip;
}

void AudioMixer::createStrips(shared_ptr<MixerControls> mixerControls)
{
	for (auto& control : mixerControls->getControls()) {
		try {
			if (dynamic_pointer_cast<core::AudioControlsChain>(control) != nullptr) {
				createStrip(dynamic_pointer_cast<core::AudioControlsChain>(control));
			}
		}
		catch (exception e) {
			continue;
		}
	}
}

shared_ptr<AudioMixerStrip> AudioMixer::createStrip(shared_ptr<ctoot::audio::core::AudioControlsChain> controls)
{

	// AudioMixerStrip2 uses a specialised createProcess() method
	//AudioMixerStrip* strip = new AudioMixerStrip2(this, controls);
	auto strip = make_shared<AudioMixerStrip>(this, controls);

	switch (controls->getId()) {
	case MixerControlsIds::CHANNEL_STRIP:
		channelStrips.push_back(strip);
		break;
	case MixerControlsIds::GROUP_STRIP:
		groupStrips.push_back(strip);
		break;
	case MixerControlsIds::FX_STRIP:
		fxStrips.push_back(strip);
		break;
	case MixerControlsIds::AUX_STRIP:
		auxStrips.push_back(strip);
		break;
	case MixerControlsIds::MAIN_STRIP:
		if (mainStrip == nullptr) {
			mainStrip = strip;
		}
		else {
			//string error("Only one main strip allowed");
			//printf("ERROR: %s", error);
		}
		break;
	}

	try {
		strips.push_back(strip);
		strip->open();
	} catch (exception e) {
		//printf("Mixer failed to open strip %s\n", strip->getName());
		//printf(e.what());
	}

    return strip;
}

void AudioMixer::removeStrip(shared_ptr<ctoot::audio::core::AudioControlsChain> controls)
{
	int i = 0;
	for (auto& strip : strips) {
		if (strip->getName().compare(controls->getName()) == 0) {
			strip->close();
			strips.erase(strips.begin() + i);
			switch (controls->getId()) {
			case MixerControlsIds::CHANNEL_STRIP:
				channelStrips.erase(strips.begin() + i);
				break;
			case MixerControlsIds::GROUP_STRIP:
				groupStrips.erase(strips.begin() + i);
				break;
			case MixerControlsIds::FX_STRIP:
				fxStrips.erase(strips.begin() + i);
				break;
			case MixerControlsIds::AUX_STRIP:
				auxStrips.erase(strips.begin() + i);
				break;
			case MixerControlsIds::MAIN_STRIP:
				mainStrip = nullptr;
				break;
			}
			return;
		}
		i++;
	}
}

void AudioMixer::close()
{
	enabled = false;
    controls->deleteObserver(observer.get());
	observer.reset();

	for (auto& s : strips) {
		s->close();
		s.reset();
	}
	strips.clear();

	channelStrips.clear();
	groupStrips.clear();
	fxStrips.clear();
	auxStrips.clear();
	
	for (auto& b : busses) {
		b->close();
		b.reset();
	}
    busses.clear();

	auxBusses.clear();
	fxBusses.clear();
	server.lock()->removeAudioBuffer(sharedAudioBuffer);
	server.reset();
	//mutationQueue.clear();
	controls.reset();
}

bool AudioMixer::isEnabled()
{
    return enabled;
}

void AudioMixer::setEnabled(bool enabled)
{
    this->enabled = enabled;
}

/*
MixerInterconnection* AudioMixer::createInterconnection(string name)
{
    return new AudioMixer_DefaultMixerInterconnection(this, name);
}
*/

AudioMixer::~AudioMixer() {
	sharedAudioBuffer = nullptr;
}
