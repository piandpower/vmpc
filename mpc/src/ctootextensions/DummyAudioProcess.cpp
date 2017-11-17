#include <ctootextensions/DummyAudioProcess.hpp>

using namespace mpc::ctootextensions;

DummyAudioProcess::DummyAudioProcess()
{
}

int DummyAudioProcess::processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames)
{
    return AUDIO_OK;
}