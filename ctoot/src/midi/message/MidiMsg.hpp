#pragma once

#include <midi/core/MidiMessage.hpp>

namespace ctoot {
	namespace midi {
		namespace message {

			class MidiMsg
			{

			public:
				static int getStatus(ctoot::midi::core::MidiMessage* msg);
				static std::vector<char>* getMessage(ctoot::midi::core::MidiMessage* msg);
				static int getLength(ctoot::midi::core::MidiMessage* msg);

				MidiMsg();

			};

		}
	}
}