#include <audio/server/StereoOutputProcess.hpp>
#include <audio/server/AsioServer.hpp>

#include <audio/core/AudioBuffer.hpp>
#include <audio/core/MetaInfo.hpp>


using namespace ctoot::audio::server;
using namespace std;

StereoOutputProcess::StereoOutputProcess(string name, bool mono, string location) 
	: AudioServerProcess(name, mono) {
}

int StereoOutputProcess::processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) {
	if (!buffer->isRealTime()) return AudioProcess::AUDIO_OK;
	buffer->setChannelFormat(format);
	auto left = buffer->getChannel(0);
	auto right = buffer->getChannel(1);
	if (localBuffer.size() != nFrames * 2)
		localBuffer.resize(nFrames * 2);
	int frameCounter = 0;
	for (int i = 0; i < nFrames * 2; i += 2) {
		localBuffer[i] = (*left)[frameCounter];
		localBuffer[i+1] = (*right)[frameCounter++];
	}
	return AudioProcess::AUDIO_OK;
}
