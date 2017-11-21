#include <sampler/Pad.hpp>

#include <sampler/MixerChannel.hpp>
#include <sampler/Sampler.hpp>

using namespace mpc::sampler;
using namespace std;

Pad::Pad(int number)
{
	this->number = number;
	//note = StartUp::getUserDefaults().lock()->getPadNotes()[number];
	note = number;
	mixerChannel = make_shared<MixerChannel>();
}

void Pad::setNote(int i)
{
    if(i < 34 || i > 98) return;

    //if(mpc::ui::sampler::SamplerGui::isPadAssignMaster()) {
  //      (*Sampler::masterPadAssign())[number] = i;
    //} else {
        note = i;
    //}
    setChanged();
    notifyObservers(string("padnotenumber"));
    setChanged();
    notifyObservers(string("note"));
    setChanged();
    notifyObservers(string("samplenumber"));
}

int Pad::getNote()
{
//	if (mpc::ui::sampler::SamplerGui::isPadAssignMaster())
//		return (*Sampler::masterPadAssign())[number];
	return note;
}

weak_ptr<MixerChannel> Pad::getMixerChannel()
{
    return mixerChannel;
}

int Pad::getNumber()
{
    return number;
}

Pad::~Pad() {
}
