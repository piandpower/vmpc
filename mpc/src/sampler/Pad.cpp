#include <sampler/Pad.hpp>

#include <Mpc.hpp>
#include <ui/Uis.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <ui/sampler/MixerSetupGui.hpp>
#include <StartUp.hpp>
#include <sampler/MixerChannel.hpp>
#include <sampler/Sampler.hpp>

using namespace mpc::sampler;
using namespace std;

Pad::Pad(mpc::Mpc* mpc, int number)
{
	this->mpc = mpc;
	this->number = number;
	note = mpc::StartUp::getUserDefaults().lock()->getPadNotes()[number];
	mixerChannel = make_shared<MixerChannel>();
}

void Pad::setNote(int i)
{
	if (i < 34 || i > 98) return;

	if (mpc->getUis().lock()->getSamplerGui()->isPadAssignMaster()) {
		(*mpc->getSampler().lock()->getMasterPadAssign())[number] = i;
	}
	else {
		note = i;
	}
	setChanged();
	notifyObservers(string("padnotenumber"));
	setChanged();
	notifyObservers(string("note"));
	setChanged();
	notifyObservers(string("samplenumber"));
}

int Pad::getNote()
{
	if (mpc->getUis().lock()->getSamplerGui()->isPadAssignMaster()) {
		return (*mpc->getSampler().lock()->getMasterPadAssign())[number];
	}
	return note;
}

weak_ptr<MixerChannel> Pad::getMixerChannel()
{
	//if (mpc->getUis().lock()->getMixerSetupGui()->
    return mixerChannel;
}

int Pad::getNumber()
{
    return number;
}

Pad::~Pad() {
}
