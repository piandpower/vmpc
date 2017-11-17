#include <audio/mixer/MixProcess.hpp>
#include <audio/mixer/AudioMixerStrip.hpp>

using namespace ctoot::audio::mixer;
using namespace ctoot::audio::core;
using namespace std;

MixProcess::MixProcess(shared_ptr<AudioMixerStrip> strip, MixVariables* vars)
{
	if (strip == nullptr) {
		return;
	}
	routedStrip = strip;
	this->vars = vars;
	auto format = vars->getChannelFormat();
	smoothingFactor = vars->getSmoothingFactor();
	channelGains = vector<float>(format->getCount());
	smoothedChannelGains = vector<float>(format->getCount());
}

AudioMixerStrip* MixProcess::getRoutedStrip()
{
    return routedStrip.lock().get();
}

int MixProcess::processAudio(AudioBuffer* buffer, int nFrames)
{
	if (!vars->isEnabled() && vars->isMaster()) {
		buffer->makeSilence();
	}
	else if (vars->isEnabled()) {
		gain = vars->getGain();
		if (gain > 0.0f || vars->isMaster()) {
			vars->getChannelGains(&channelGains);
			for (auto c = 0; c < channelGains.size(); c++) {
				smoothedChannelGains[c] += smoothingFactor * (channelGains[c] - smoothedChannelGains[c]);
			}
			getRoutedStrip()->mix(buffer, smoothedChannelGains, nFrames);
		}
	}
	return AUDIO_OK;
}

MixProcess::~MixProcess() {
}
