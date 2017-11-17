#pragma once

namespace ctoot {
	namespace synth {
		namespace modules {
			namespace filter {

				class FilterIds
				{

				public:
					static const int FILTER_BASE_ID{ 32 };
					static const int MOOG_LPF_ID{ 33 };
					static const int STATE_VARIABLE_FILTER_ID{ 34 };
					static const int FORMANT_FILTER_ID{ 35 };
					static const int DUAL_STATE_VARIABLE_FILTER_ID{ 36 };

				public:
					FilterIds();
				};

			}
		}
	}
}
