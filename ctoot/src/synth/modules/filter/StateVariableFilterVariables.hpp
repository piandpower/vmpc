#pragma once
#include <synth/modules/filter/FilterVariables.hpp>

namespace ctoot {
	namespace synth {
		namespace modules {
			namespace filter {

				class StateVariableFilterVariables
					: public virtual FilterVariables
				{
				public:
					virtual float getModeMix() = 0;
					virtual bool isBandMode() = 0;

				};

			}
		}
	}
}
