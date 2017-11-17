#pragma once
#include <control/CompoundControl.hpp>

using namespace ctoot::control;

namespace ctoot {
	namespace synth {

		class SynthControls
			: public CompoundControl
		{

		public:
			typedef CompoundControl super;

		public:
			SynthControls(int id, std::string name);
		};

	}
}
