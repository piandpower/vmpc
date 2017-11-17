#include <audio/server/StereoOutputProcess.hpp>
#include <audio/server/AsioServer.hpp>

#include <audio/core/AudioBuffer.hpp>
#include <audio/core/MetaInfo.hpp>

#include <Logger.hpp>

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
	localBuffer.clear();
	for (int i = 0; i < nFrames; i++) {
		localBuffer.push_back((*left)[i]);
		localBuffer.push_back((*right)[i]);
	}
	return AudioProcess::AUDIO_OK;
}
