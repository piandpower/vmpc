#pragma once

namespace ctoot {
	namespace synth {
		namespace modules {
			namespace filter {

				class FilterVariables
				{
				public:
					virtual void setSampleRate(int rate) = 0;
					virtual float getCutoff() = 0;
					virtual float getResonance() = 0;
				};

			}
		}
	}
}
