#pragma once

namespace ctoot {
	namespace synth {
		namespace modules {
			namespace filter {

				class FilterType
				{
					static const int  OFF{ 0 };
					static const int  LOW{ 1 };
					static const int  HIGH{ 2 };
					static const int  NOTCH{ 3 };
					static const int  BAND{ 4 };
					static const int  PEAK{ 5 };
				};

			}
		}
	}
}
