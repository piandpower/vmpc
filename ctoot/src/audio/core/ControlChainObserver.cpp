#include <audio/core/ControlChainObserver.hpp>
#include <audio/core/AudioProcessChain.hpp>
#include <observer/Observable.hpp>
#include <control/ChainMutation.hpp>

using namespace ctoot::audio::core;

ControlChainObserver::ControlChainObserver(AudioProcessChain* apc) {
	this->apc = apc;
}

void ControlChainObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	try {
		boost::any_cast<ctoot::control::ChainMutation*>(arg);
	}
	catch (boost::bad_any_cast e) {
		e.what();
		return;
	}
	apc->mutationQueue.try_enqueue(boost::any_cast<ctoot::control::ChainMutation*>(arg));
}

ControlChainObserver::~ControlChainObserver() {
	apc = nullptr;
}
