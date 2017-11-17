#pragma once
#include <cstdint>
#include <control/LawControl.hpp>
#include <control/ControlLaw.hpp>

namespace ctoot {
	namespace audio {
		namespace fader {
			
			class FaderLaw;

			class FaderControl
				: public ctoot::control::LawControl
			{

			public:
				float gain{ 0 };

			public:
				float getGain();
				static FaderLaw* SEMI_LOG();
				static FaderLaw* LOG();
				static FaderLaw* BROADCAST();
				static float ATTENUATION_CUTOFF();
				static FaderLaw* defaultLaw();
				FaderControl(int id, ctoot::control::ControlLaw law, float initialValue);
				virtual ~FaderControl();

			};
		}
	}
}
