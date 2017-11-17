#include <sampler/MixerChannel.hpp>

using namespace mpc::sampler;
using namespace std;

MixerChannel::MixerChannel() 
{
	output = 0;
	volumeIndividualOut = 100;
	panning = 50;
	level = 100;
	fxPath = 0;
	fxSendLevel = 0;
	followStereo = false;
	stereo = true;
}

void MixerChannel::setFollowStereo(bool b)
{
    if(followStereo == b) return;

    followStereo = b;
    setChanged();
    notifyObservers(string("followstereo"));
}

bool MixerChannel::isFollowingStereo()
{
    return followStereo;
}

void MixerChannel::setStereo(bool b)
{
    stereo = b;
}

bool MixerChannel::isStereo()
{
    return stereo;
}

int MixerChannel::getOutput()
{
    return output;
}

void MixerChannel::setOutput(int i)
{
    if(i < 0 || i > 8)
        return;

    output = i;
    setChanged();
    notifyObservers(string("output"));
}

void MixerChannel::setVolumeIndividualOut(int i)
{
    if(i < 0 || i > 100)
        return;

    volumeIndividualOut = i;
    setChanged();
    notifyObservers(string("volumeindividual"));
}

int MixerChannel::getVolumeIndividualOut()
{
    return volumeIndividualOut;
}

void MixerChannel::setPanning(int i)
{
    if(i < 0 || i > 100)
        return;

    panning = i;
    setChanged();
    notifyObservers(string("panning"));
}

int MixerChannel::getPanning()
{
    return panning;
}

void MixerChannel::setLevel(int i)
{
    if(i < 0 || i > 100)
        return;

    level = i;
    setChanged();
    notifyObservers(string("volume"));
}

int MixerChannel::getLevel()
{
    return level;
}

void MixerChannel::setFxPath(int i)
{
    if(i < 0 || i > 4)
        return;

    fxPath = i;
    setChanged();
    notifyObservers(string("fxpath"));
}

int MixerChannel::getFxPath()
{
    return fxPath;
}

void MixerChannel::setFxSendLevel(int i)
{
    if(i < 0 || i > 100)
        return;

    fxSendLevel = i;
    setChanged();
    notifyObservers(string("fxsendlevel"));
}

int MixerChannel::getFxSendLevel()
{
    return fxSendLevel;
}