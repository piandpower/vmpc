#include <synth/ChannelledSynthControls.hpp>
#include <observer/Observable.hpp>
#include <observer/Observer.hpp>
#include <control/CompoundControl.hpp>
#include <control/Control.hpp>
#include <synth/SynthChannelControls.hpp>

using namespace ctoot::synth;
using namespace ctoot::control;
using namespace std;

ChannelledSynthControls::ChannelledSynthControls(int id, string name) : SynthControls(id, name)
{
	channelControls = vector<weak_ptr<SynthChannelControls>>(16);
}

CompoundControl* ChannelledSynthControls::getGlobalControls()
{
    return globalControls;
}

weak_ptr<SynthChannelControls> ChannelledSynthControls::getChannelControls(int chan)
{
    return channelControls[chan];
}

void ChannelledSynthControls::setGlobalControls(CompoundControl* controls)
{
    globalControls = controls;
	add(shared_ptr<Control>(controls));
}

void ChannelledSynthControls::setChannelControls(int chan, shared_ptr<SynthChannelControls> controls)
{
	channelControls[chan] = controls;
	add(std::move(controls));
}

ChannelledSynthControls::~ChannelledSynthControls() {
}
