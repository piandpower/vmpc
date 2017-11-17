#include <synth/synths/multi/MultiSynthControls.hpp>
#include <control/CompoundControl.hpp>
#include <control/Control.hpp>
#include <synth/SynthChannelControls.hpp>

using namespace ctoot::synth;
using namespace ctoot::synth::synths::multi;
using namespace std;

MultiSynthControls::MultiSynthControls() : ChannelledSynthControls(ID, NAME_)
{
}

const int MultiSynthControls::ID;

string MultiSynthControls::NAME()
{
    return NAME_;
}
string MultiSynthControls::NAME_ = "MultiSynth";

void MultiSynthControls::setChannelControls(int chan, shared_ptr<SynthChannelControls> c)
{
	auto old = getChannelControls(chan).lock();
	
	if (old != nullptr) {
		remove(old);
		old->close();
	}
	if (c != nullptr) {
		auto name = c->getName();
		if (find(name) != nullptr) {
			disambiguate(c.get());
			c->setAnnotation(name);
		}
	}
	ChannelledSynthControls::setChannelControls(chan, c);
	setChanged();
	notifyObservers(chan);
}

bool MultiSynthControls::isPluginParent()
{
    return true;
}
