#include <audio/mixer/MixerControlsObserver.hpp>
#include <audio/mixer/Mutation.hpp>
#include <audio/mixer/AudioMixer.hpp>

using namespace ctoot::audio::mixer;

MixerControlsObserver::MixerControlsObserver(AudioMixer* am)
{
	this->am = am;
}

void MixerControlsObserver::update(boost::any a)
{
	Mutation* m = nullptr;
	try {
		m = boost::any_cast<Mutation*>(a);
	}
	catch (boost::bad_any_cast e) {
		e.what();
		return;
	}
	if(m != nullptr) {
        if(am->isEnabled() && am->server.lock()->isRunning()) {
            am->mutationQueue.try_enqueue(m);
		} else {
            am->processMutation(m);
        }
    }
}
