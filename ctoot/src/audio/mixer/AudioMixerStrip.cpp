#include <audio/mixer/AudioMixerStrip.hpp>

#include <audio/core/MetaInfo.hpp>
#include <audio/core/AudioBuffer.hpp>
#include <audio/core/AudioControls.hpp>
#include <audio/core/AudioControlsChain.hpp>
#include <audio/core/AudioProcess.hpp>
#include <audio/core/ChannelFormat.hpp>
#include <audio/mixer/AudioMixer.hpp>
#include <audio/mixer/AudioMixerBus.hpp>
#include <audio/mixer/MainMixProcess.hpp>
//#include <audio/mixer/MixProcess.hpp>
#include <audio/mixer/MixVariables.hpp>
#include <audio/mixer/MixerControlsIds.hpp>

using namespace ctoot::audio::mixer;
using namespace ctoot::audio::core;
using namespace std;

AudioMixerStrip::AudioMixerStrip(AudioMixer* mixer, shared_ptr<AudioControlsChain> controlsChain) : AudioProcessChain(controlsChain)
{
	silenceCountdown = AudioMixerStrip::silenceCount;
	this->mixer = mixer;
	buffer = createBuffer();
	channelFormat = buffer->getChannelFormat();
}

AudioProcess* AudioMixerStrip::getInputProcess()
{
    return input;
}

void AudioMixerStrip::setInputProcess(AudioProcess* input)
{

    if(controlChain->getId() != MixerControlsIds::CHANNEL_STRIP) {
//        throw new ::java::lang::IllegalArgumentException(u"No external input to this Strip type"_j);
		return;
    }
    auto oldInput = this->input;
    if(input != nullptr) input->open();

    this->input = input;
    if(input == nullptr) {
		//controlChain->setMetaInfo(weak_ptr<MetaInfo>());
    }
    if(oldInput != nullptr)
        oldInput->close();

}

AudioProcess* AudioMixerStrip::getDirectOutputProcess()
{
    return directOutput;
}

void AudioMixerStrip::open() {
	for (auto& control : controlChain->getControls()) {
		try {
			if (dynamic_pointer_cast<AudioControls>(control) != nullptr) {
				auto p = createProcess(dynamic_pointer_cast<AudioControls>(control).get());
				processes.push_back(p);
				if (p != nullptr) {
					p->open();
				}
			}
		}
		catch (bad_cast e) {
			continue;
		}
	}
	AudioProcessChain::open();
}

void AudioMixerStrip::setDirectOutputProcess(AudioProcess* output)
{
    auto oldOutput = directOutput;
    if(output != nullptr) output->open();

    this->directOutput = output;
	if (oldOutput != nullptr) oldOutput->close();
}

void AudioMixerStrip::silence(int nFrames)
{
    if(nmixed > 0) {
		buffer->makeSilenceFrames(nFrames);
		nmixed = 0;
   }
}

AudioBuffer* AudioMixerStrip::createBuffer()
{
	auto id = controlChain->getId();
    if(id == MixerControlsIds::CHANNEL_STRIP) {
        isChannel = true;
        return mixer->getSharedBuffer();
    } else if(id == MixerControlsIds::GROUP_STRIP) {
        auto buf = mixer->createBuffer(getName());
        buf->setChannelFormat(mixer->getMainBus()->getBuffer()->getChannelFormat());
        return buf;
    } else if(id == MixerControlsIds::MAIN_STRIP) {
        return mixer->getMainBus()->getBuffer();
    }
    return mixer->getBus(getName())->getBuffer();
}

const int AudioMixerStrip::silenceCount;

bool AudioMixerStrip::processBuffer(int nFrames)
{
	auto ret = AUDIO_OK;
	if (isChannel) {
		if (input != nullptr) {
			ret = input->processAudio(buffer, nFrames);
			buffer->getMetaInfo();
			checkMetaInfo(buffer->getMetaInfo());
			if (ret == AUDIO_DISCONNECT) {
				processMutations();
				return false;
			}
			else if (ret == AUDIO_SILENCE && silenceCountdown == 0) {
				return false;
			}
		}
		else {
			processMutations();
			return false;
		}
	}
	processAudio(buffer, nFrames);
	if (isChannel) {
		if (ret == AUDIO_SILENCE) {
			if (buffer->square() > 0.00000001f)
				silenceCountdown = silenceCount;
			else
				silenceCountdown--;
		}
		else {
			silenceCountdown = silenceCount;
		}
	}
	if (directOutput != nullptr) {
		directOutput->processAudio(buffer, nFrames);
	}
	return true;
}

void AudioMixerStrip::checkMetaInfo(weak_ptr<MetaInfo> wInfo)
{
	auto info = wInfo.lock();
	//metaInfo.reset();
	//metaInfo = make_shared<MetaInfo>(info->getSourceLabel(), info->getSourceLocation());
	//controlChain->setMetaInfo(metaInfo);
}

AudioProcess* AudioMixerStrip::createProcess(AudioControls* controls)
{
	if (dynamic_cast<MixVariables*>(controls) != nullptr) {
		auto vars = dynamic_cast<MixVariables*>(controls);
		weak_ptr<AudioMixerStrip> routedStrip;
		if (vars->getName().compare(mixer->getMainBus()->getName()) == 0) {
			routedStrip = mixer->getMainStrip();
			try {
				return new MainMixProcess(routedStrip.lock(), dynamic_cast<MixVariables*>(vars), mixer);
			}
			catch (bad_cast e) {
				printf("\n%s", e.what());
			}
		}
		else {
			routedStrip = mixer->getStripImpl(vars->getName());
			return new MixProcess(routedStrip.lock(), vars);
		}
	}
	return nullptr;
}

int AudioMixerStrip::mix(ctoot::audio::core::AudioBuffer* bufferToMix, vector<float> gain, int nFrames)
{
	if (bufferToMix == nullptr) return 0;
	auto ret = channelFormat->mix(buffer, bufferToMix, gain, nFrames);
	if (ret != 0) nmixed += 1;
	return ret;
}

void AudioMixerStrip::close()
{
    AudioProcessChain::close();
    mixer->removeBuffer(buffer);
}

AudioMixerStrip::~AudioMixerStrip() {
	mixer = nullptr;
	input = nullptr;
	directOutput = nullptr;
	buffer = nullptr;
	//metaInfo.reset();
	channelFormat = nullptr;
}
