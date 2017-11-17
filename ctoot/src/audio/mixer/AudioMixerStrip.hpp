#pragma once
#include <audio/core/AudioProcessChain.hpp>
#include <audio/mixer/AudioMixer.hpp>

#include <audio/core/MetaInfo.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class AudioMixerStrip
				: public core::AudioProcessChain
			{

			public:
				typedef AudioProcessChain super;

			public:
				AudioMixer* mixer{ nullptr };

			private:
                ctoot::audio::core::AudioBuffer* buffer{ nullptr };
				std::shared_ptr<ctoot::audio::core::MetaInfo> metaInfo{ nullptr };
				AudioProcess* input{ nullptr };
				AudioProcess* directOutput{ nullptr };
				bool isChannel{ false };
                ctoot::audio::core::ChannelFormat* channelFormat{ nullptr };
				int nmixed{ 1 };

			public:
				virtual AudioProcess* getInputProcess();
				virtual void setInputProcess(AudioProcess* input);
				virtual AudioProcess* getDirectOutputProcess();
				virtual void setDirectOutputProcess(AudioProcess* output);
				virtual void silence();

			public:
				virtual ctoot::audio::core::AudioBuffer* createBuffer();

			private:
				static const int silenceCount{ 1000 };
				int silenceCountdown{};

			public:
				virtual bool processBuffer(int nFrames);
				virtual void checkMetaInfo(std::weak_ptr<ctoot::audio::core::MetaInfo> info);
				AudioProcess* createProcess(ctoot::audio::core::AudioControls* controls) override;
				void open() override;

			public:
				virtual int mix(ctoot::audio::core::AudioBuffer* bufferToMix, std::vector<float> gain, int nFrames);
				void close() override;

				AudioMixerStrip(AudioMixer* mixer, std::shared_ptr<ctoot::audio::core::AudioControlsChain> controlsChain);
				~AudioMixerStrip();

			};

		}
	}
}
