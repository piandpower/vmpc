#include <sampler/MonitorOutput.hpp>

#include <sampler/Sampler.hpp>
////////#include <lcdgui/LayeredScreen.hpp>
//#include <lcdgui/LayeredScreen.hpp>
//#include <ui/sampler/SamplerGui.hpp>
#include <sampler/Sampler.hpp>
#include <audio/core/ChannelFormat.hpp>

using namespace mpc::sampler;
using namespace std;

MonitorOutput::MonitorOutput(Sampler* sampler)
{
	this->sampler = sampler;
}

string MonitorOutput::getName()
{
    return "monitor";
}

void MonitorOutput::open()
{
    closed = false;
}

int32_t MonitorOutput::processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames)
{
	/*
		if (sampler->monitorBufferL == nullptr || sampler->monitorBufferR == nullptr || lGui->getSamplerGui()->getMonitor() == 0 || lGui->getMainFrame().lock()->getLayeredScreen().lock()->getCurrentScreenName().compare("sample") != 0 || closed) {
		buffer->makeSilence();
		return AUDIO_SILENCE;
	}
	auto mode = lGui->getSamplerGui()->getMode();
	auto leftPairs = buffer->getChannelFormat()->getLeft();
	auto rightPairs = buffer->getChannelFormat()->getRight();
	auto left = buffer->getChannel(leftPairs[0]);
	auto right = buffer->getChannel(rightPairs[0]);
	for (int i = 0; i < buffer->getSampleCount(); i++) {
		(*left)[i] = mode != 1 ? (*sampler->monitorBufferL)[i] : (*sampler->monitorBufferR)[i];
		(*right)[i] = mode != 0 ? (*sampler->monitorBufferR)[i] : (*sampler->monitorBufferL)[i];
	}
	*/
	return AUDIO_OK;
}

void MonitorOutput::close()
{
    closed = true;
}

string MonitorOutput::getLocation()
{
    return "sampler";
}

MonitorOutput::~MonitorOutput() {
}
