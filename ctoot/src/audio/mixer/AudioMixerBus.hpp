#pragma once
//#include <audio/mixer/AudioMixer.hpp>
#include <audio/core/AudioBuffer.hpp>
#include <audio/core/AudioProcess.hpp>
#include <audio/core/ChannelFormat.hpp>
#include <audio/mixer/BusControls.hpp>
#include <audio/mixer/MixerControlsIds.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {
			class AudioMixer;
			class AudioMixerBus
			{

			private:
				AudioMixer* mixer{ nullptr };
				ctoot::audio::core::AudioBuffer* buffer{ nullptr };
				ctoot::audio::core::AudioProcess* output{ nullptr };
				ctoot::audio::core::AudioProcess* meter{ nullptr };
				bool isFx{ false };
				std::string name{ "" };
				ctoot::audio::core::ChannelFormat* channelFormat{ nullptr };

			public:
				virtual void setOutputProcess(ctoot::audio::core::AudioProcess* output);
				virtual void setMeterProcess(ctoot::audio::core::AudioProcess* meter);
				virtual ctoot::audio::core::AudioBuffer* getBuffer();
				virtual std::string getName();
				virtual void silence(int nFrames);
				virtual void write(int nFrames);
				virtual void close();

			public:
				AudioMixerBus(AudioMixer* mixer, std::weak_ptr<BusControls> busControls);
				~AudioMixerBus();
			};
		}
	}
}
