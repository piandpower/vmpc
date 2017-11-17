#pragma once

#include <audio/core/SimpleAudioProcess.hpp>
#include <audio/mixer/MixVariables.hpp>
#include <audio/mixer/AudioMixerStrip.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class MixProcess
				: public ctoot::audio::core::SimpleAudioProcess
			{

			public:
				typedef SimpleAudioProcess super;

			public:
				MixVariables* vars{ nullptr };
				std::weak_ptr<AudioMixerStrip> routedStrip{ };

			private:
				float gain{ 1.0f };
				std::vector<float> channelGains{};
				std::vector<float> smoothedChannelGains{};
				float smoothingFactor{ 0 };

			public:
				virtual AudioMixerStrip* getRoutedStrip();

			public:
				int processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) override;

				MixProcess(std::shared_ptr<AudioMixerStrip> strip, MixVariables* vars);
				virtual ~MixProcess();

			};

		}
	}
}
