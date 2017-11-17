#pragma once
#include <audio/mixer/MixProcess.hpp>
#include <audio/mixer/MixVariables.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class MainMixProcess
				: public MixProcess
			{

			public:
				typedef MixProcess super;

			public:
				std::weak_ptr<AudioMixerStrip> nextRoutedStrip{};

			private:
				moduru::observer::Observer* routeObserver{ nullptr };

			public:
				EnumControl* routeControl{ nullptr };

			public:
				AudioMixerStrip* getRoutedStrip() override;

			public:
				void open() override;
				void close() override;

				MainMixProcess(std::shared_ptr<AudioMixerStrip> strip, MixVariables* vars, AudioMixer* mixer);
				~MainMixProcess();

			};

		}
	}
}
