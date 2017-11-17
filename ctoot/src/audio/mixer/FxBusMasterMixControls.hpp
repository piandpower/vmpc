#pragma once
#include <audio/mixer/MixControls.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class FxBusMasterMixControls
				: public MixControls
			{

			public:
				typedef MixControls super;
				bool canBeInsertedBefore() override;
				bool canBeMovedBefore() override;
				bool canBeMoved() override;

				FxBusMasterMixControls(bool hasMixControls, std::weak_ptr<MixerControls> mixerControls, int stripId, std::weak_ptr<BusControls> busControls, bool isMaster);
				bool hasMixControls;

			};

		}
	}
}
