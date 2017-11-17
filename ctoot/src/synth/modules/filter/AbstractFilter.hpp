#pragma once

namespace ctoot {
	namespace synth {
		namespace modules {
			namespace filter {

				class FilterVariables;

				class AbstractFilter
				{

				public:
					FilterVariables* vars{};
					float fs{ 44100.0 };

				public:
					virtual void setSampleRate(int rate);

					AbstractFilter(FilterVariables* filterVariables);

				};
			}
		}
	}
}
