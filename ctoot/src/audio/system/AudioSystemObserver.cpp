#include <audio/system/AudioSystemObserver.hpp>
#include <audio/system/DefaultAudioSystem.hpp>

using namespace ctoot::audio::system;

AudioSystemObserver::AudioSystemObserver(DefaultAudioSystem* das)
{
	this->das = das;
}

void AudioSystemObserver::update(moduru::observer::Observable* obs, boost::any arg)
{
	das->setChanged();
	das->notifyObservers(obs);
}

AudioSystemObserver::~AudioSystemObserver() {
	das = nullptr;
}
