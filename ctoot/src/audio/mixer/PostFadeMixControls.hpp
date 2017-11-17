#pragma once
#include <audio/mixer/MixControls.hpp>
#include <audio/mixer/MainMixControls.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class PostFadeMixControls
				: public MixControls
			{

			public:
				typedef MixControls super;

			private:
				MainMixControls* mainMixControls{ nullptr };

			public:
				bool isEnabled() override;
				float getGain() override;

				PostFadeMixControls(std::weak_ptr<MixerControls> mixerControls, int stripId, std::weak_ptr<BusControls> busControls, MainMixControls* mainMixControls);
				~PostFadeMixControls();
			};
		}
	}
}
