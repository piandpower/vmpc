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

mpc::sampler::MixerChannel* ApsMixer::getMixVariables(int pad)
{
	auto params = new mpc::sampler::MixerChannel();
	params->setFxPath(getFxPath(pad));
	params->setLevel(getLevel(pad));
	params->setPanning(getPanning(pad));
	params->setVolumeIndividualOut(getIndividualLevel(pad));
	params->setOutput(getIndividualOutput(pad));
	params->setFxSendLevel(getSendLevel(pad));
	return params;
}

int ApsMixer::getFxPath(int pad)
{
    return fxPaths[pad - 35];
}

int ApsMixer::getLevel(int pad)
{
	return levels[pad - 35];
}

int ApsMixer::getPanning(int pad)
{
    return pannings[pad - 35];
}

int ApsMixer::getIndividualLevel(int pad)
{
    return iLevels[pad - 35];
}

int ApsMixer::getIndividualOutput(int pad)
{
    return iOutputs[pad - 35];
}

int ApsMixer::getSendLevel(int pad)
{
    return sendLevels[pad - 35];
}

vector<char> ApsMixer::getBytes()
{
    return saveBytes;
}
