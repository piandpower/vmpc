#include <audio/core/AudioProcessAdapter.hpp>
#include <audio/core/AudioProcess.hpp>

ctoot::audio::core::AudioProcessAdapter::AudioProcessAdapter(AudioProcess* process)
{
	if (process == nullptr) {
		//        throw new ::java::lang::IllegalArgumentException(u"null AudioProcess"_j);
		return;
	}
	this->process = process;
}

void ctoot::audio::core::AudioProcessAdapter::open()
{
    process->open();
}

int ctoot::audio::core::AudioProcessAdapter::processAudio(ctoot::audio::core::AudioBuffer* buf, int nFrames)
{
	return process->processAudio(buf, nFrames);
}

void ctoot::audio::core::AudioProcessAdapter::close()
{
	process->close();
}

ctoot::audio::core::AudioProcessAdapter::~AudioProcessAdapter() {
	process = nullptr;
}
