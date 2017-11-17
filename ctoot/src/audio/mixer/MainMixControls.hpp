#pragma once
#include <audio/mixer/MixVariables.hpp>
#include <audio/mixer/MixControls.hpp>
#include <control/EnumControl.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class MainMixControls
				: public MixControls

			{

			public:
				typedef MixControls super;

			private:
				EnumControl* routeControl{ nullptr };

			public:
				EnumControl* createRouteControl(int stripId) override;

			public:
				EnumControl* getRouteControl() override;

				MainMixControls(std::weak_ptr<MixerControls> mixerControls, int stripId, std::shared_ptr<BusControls> busControls, bool isMaster);
				~MainMixControls();

			private:
				friend class RouteControl;
			};

		}
	}
}
