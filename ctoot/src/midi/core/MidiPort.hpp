#pragma once

#include <string>

namespace ctoot {
	namespace midi {
		namespace core {


			struct MidiPort

			{
				virtual std::string getName() = 0;

			};

		}
	}
}
