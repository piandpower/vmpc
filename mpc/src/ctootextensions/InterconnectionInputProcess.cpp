#include <ctootextensions/InterconnectionInputProcess.hpp>
#include <ctootextensions/MpcMixerInterconnection.hpp>
#include <audio/core/AudioBuffer.hpp>

using namespace mpc::ctootextensions;

InterconnectionInputProcess::InterconnectionInputProcess(MpcMixerInterconnection* mmi, ctoot::audio::core::AudioBuffer* sharedBuffer)
    : mmi(mmi)
    , sharedBuffer(sharedBuffer)
{
}

int InterconnectionInputProcess::processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames)
{
	sharedBuffer->copyFrom(buffer, nFrames);
	if (!mmi->leftEnabled) sharedBuffer->makeSilence(0, nFrames);
	if (!mmi->rightEnabled) sharedBuffer->makeSilence(1, nFrames);
	return AUDIO_OK;
}

InterconnectionInputProcess::~InterconnectionInputProcess() {
}
