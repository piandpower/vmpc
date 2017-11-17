#include <audio/system/MixerInputConnection.hpp>

#include <audio/mixer/AudioMixerStrip.hpp>
#include <audio/system/AudioOutput.hpp>
#include <audio/system/MixerConnectedAudioSystem.hpp>

using namespace ctoot::audio::system;
using namespace std;

MixerInputConnection::MixerInputConnection(MixerConnectedAudioSystem* mca, AudioOutput* from, ctoot::audio::mixer::AudioMixerStrip* to, int flags) : AudioConnection(flags)
{
	if (from == nullptr || to == nullptr) {
		//        throw new ::java::lang::IllegalArgumentException(u"MixerInputConnection constructor null argument"_j);
		return;
	}
	this->from = from;
	this->to = to;
	try {
		to->setInputProcess(from);
	}
	catch (exception* e) {
		e->what();
		return;
	}
}

void MixerInputConnection::close()
{
	try {
		to->setInputProcess(nullptr);
	}
	catch (exception* e) {
		e->what();
	}
	from = nullptr;
	to = nullptr;
}

string MixerInputConnection::getOutputName()
{
    return from->getName();
}

string MixerInputConnection::getOutputLocation()
{
    return from->getLocation();
}

string MixerInputConnection::getInputName()
{
    return to->getName();
}

MixerInputConnection::~MixerInputConnection() {
	from = nullptr;
	to = nullptr;
}
