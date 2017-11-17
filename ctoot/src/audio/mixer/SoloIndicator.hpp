#pragma once

#include <control/BooleanControl.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class SoloIndicator
				: public control::BooleanControl
			{

			public:
				SoloIndicator();
				~SoloIndicator();

			};

		}
	}
}
