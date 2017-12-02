#include <file/pgmwriter/Mixer.hpp>

#include <sampler/MixerChannel.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>

using namespace mpc::file::pgmwriter;
using namespace std;

Mixer::Mixer(mpc::sampler::Program* program)
{
	mixerArray = vector<char>(384 + 3);
	for (int i = 0; i < 64; i++) {
		auto pad = program->getPad(i);
		auto mc = pad->getMixerChannel().lock();
		setVolume(i, mc->getLevel());
		setPan(i, mc->getPanning());
		setVolumeIndividual(i, mc->getVolumeIndividualOut());
		setOutput(i, mc->getOutput());
		setEffectsSendLevel(i, mc->getFxSendLevel());
		setEffectsOutput(i, mc->getFxPath());
	}
	mixerArray[384] = 0;
	mixerArray[385] = 64;
	mixerArray[386] = 0;
}

vector<char> Mixer::getMixerArray()
{
    return mixerArray;
}

void Mixer::setEffectsOutput(int pad, int effectsOutputNumber)
{
    mixerArray[(pad * 6) + 0] = (char)(effectsOutputNumber);
}

void Mixer::setVolume(int pad, int volume)
{
    mixerArray[(pad * 6) + 1] = (char)(volume);
}

void Mixer::setPan(int pad, int pan)
{
    mixerArray[(pad * 6) + 2] = (char)(pan);
}

void Mixer::setVolumeIndividual(int pad, int volumeIndividual)
{
    mixerArray[(pad * 6) + 3] = (char)(volumeIndividual);
}

void Mixer::setOutput(int pad, int output)
{
    mixerArray[(pad * 6) + 4] = (char)(output);
}

void Mixer::setEffectsSendLevel(int pad, int effectsSendLevel)
{
    mixerArray[(pad * 6) + 5] = (char)(effectsSendLevel);
}
