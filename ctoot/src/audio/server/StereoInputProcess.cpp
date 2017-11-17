#include <audio/server/StereoInputProcess.hpp>
#include <audio/server/AudioServer.hpp>

#include <audio/core/AudioBuffer.hpp>
#include <audio/core/MetaInfo.hpp>

using namespace ctoot::audio::server;
using namespace std;

StereoInputProcess::StereoInputProcess(string name, bool mono, string location) 
	: AudioServerProcess(name, mono) {
}

int StereoInputProcess::processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) {
	if (!buffer->isRealTime()) return AUDIO_DISCONNECT;
	buffer->setChannelFormat(format);
	auto left = buffer->getChannel(0);
	auto right = buffer->getChannel(1);
	int sampleCounter = 0;
	for (int i = 0; i < nFrames; i++) {
		(*left)[i] = localBuffer[sampleCounter++];
		(*right)[i] = localBuffer[sampleCounter++];
	}
	return AUDIO_OK;
}
