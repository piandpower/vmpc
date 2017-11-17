#include <synth/synths/multi/MultiMidiSynthObserver.hpp>
#include <synth/SynthChannel.hpp>
#include <synth/SynthChannelControls.hpp>
//#include <synth/SynthChannelServices.hpp>
#include <synth/synths/multi/MultiMidiSynth.hpp>
#include <synth/synths/multi/MultiSynthControls.hpp>

#include <ctootextensions/MpcSoundPlayerChannel.hpp>
#include <ctootextensions/MpcBasicSoundPlayerChannel.hpp>
#include <ctootextensions/MpcSoundPlayerControls.hpp>
#include <ctootextensions/MpcBasicSoundPlayerControls.hpp>

using namespace ctoot::synth::synths::multi;
using namespace std;

MultiMidiSynthObserver::MultiMidiSynthObserver(MultiMidiSynth *mms, weak_ptr<MultiSynthControls> controls)
    : mms(mms)
    , controls(controls)
{
}


void MultiMidiSynthObserver::update(moduru::observer::Observable* obs, boost::any a)
{
	try {
		auto chan = boost::any_cast<int>(a);
		if (chan < 0 || chan > 15) return;

		//temp limitation! remove once things are better.
		if (chan > 4) return;

		auto channelControls = controls.lock()->getChannelControls(chan).lock();

		if (channelControls) {
			//auto synthChannel = ctoot::synth::SynthChannelServices::createSynthChannel(channelControls);
			shared_ptr<SynthChannel> synthChannel = nullptr;
			if (chan < 4) {
				synthChannel = make_shared<mpc::ctootextensions::MpcSoundPlayerChannel>(dynamic_pointer_cast<mpc::ctootextensions::MpcSoundPlayerControls>(channelControls));
			}
			else {
				synthChannel = make_shared<mpc::ctootextensions::MpcBasicSoundPlayerChannel>(dynamic_pointer_cast<mpc::ctootextensions::MpcBasicSoundPlayerControls>(channelControls));
			}
			if (!synthChannel) {
				// "No SynthChannel for SynthControls " + channelControls->getName();
			}
			else {
				synthChannel->setLocation(mms->getLocation() + " Channel " + to_string(1 + chan));
				synthChannel->addObserver(channelControls.get());
			}
			mms->setChannel(chan, synthChannel);
		}
		else {
			mms->setChannel(chan, shared_ptr<SynthChannel>());
		}
	}
	catch (boost::bad_any_cast e) {
		e.what();
	}
}

MultiMidiSynthObserver::~MultiMidiSynthObserver() {
}
