#include <file/aps/ApsMixer.hpp>

#include <sampler/MixerChannel.hpp>

using namespace mpc::file::aps;
using namespace std;

ApsMixer::ApsMixer(vector<char> loadBytes) 
{
	for (int i = 0; i < 64; i++) {
		fxPaths[i] = loadBytes[(i * 6) + 0];
		levels[i] = loadBytes[(i * 6) + 1];
		pannings[i] = (loadBytes[(i * 6) + 2]);
		iLevels[i] = loadBytes[(i * 6) + 3];
		iOutputs[i] = loadBytes[(i * 6) + 4];
		sendLevels[i] = loadBytes[(i * 6) + 5];
	}
}

ApsMixer::ApsMixer(vector<weak_ptr<mpc::sampler::MixerChannel>> mixer) 
{
	for (int i = 0; i < 64; i++) {
		auto mixerChannel = mixer[i].lock();
		saveBytes[(i * 6) + 0] = (int8_t)(mixerChannel->getFxPath());
		saveBytes[(i * 6) + 1] = (int8_t)(mixerChannel->getLevel());
		saveBytes[(i * 6) + 2] = (int8_t)((mixerChannel->getPanning()));
		saveBytes[(i * 6) + 3] = (int8_t)(mixerChannel->getVolumeIndividualOut());
		saveBytes[(i * 6) + 4] = (int8_t)(mixerChannel->getOutput());
		saveBytes[(i * 6) + 5] = (int8_t)(mixerChannel->getFxSendLevel());
	}
}

mpc::sampler::MixerChannel* ApsMixer::getMixVariables(int noteNumber)
{
	auto params = new mpc::sampler::MixerChannel();
	params->setFxPath(getFxPath(noteNumber));
	params->setLevel(getLevel(noteNumber));
	params->setPanning(getPanning(noteNumber));
	params->setVolumeIndividualOut(getIndividualLevel(noteNumber));
	params->setOutput(getIndividualOutput(noteNumber));
	params->setFxSendLevel(getSendLevel(noteNumber));
	return params;
}

int ApsMixer::getFxPath(int note)
{
    return fxPaths[note - 35];
}

int ApsMixer::getLevel(int note)
{
	return levels[note - 35];
}

int ApsMixer::getPanning(int note)
{
    return pannings[note - 35];
}

int ApsMixer::getIndividualLevel(int note)
{
    return iLevels[note - 35];
}

int ApsMixer::getIndividualOutput(int note)
{
    return iOutputs[note - 35];
}

int ApsMixer::getSendLevel(int note)
{
    return sendLevels[note - 35];
}

vector<char> ApsMixer::getBytes()
{
    return saveBytes;
}
