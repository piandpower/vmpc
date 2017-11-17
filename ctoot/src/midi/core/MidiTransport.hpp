#pragma once
#include <midi/core/MidiMessage.hpp>

namespace ctoot {
	namespace midi {
		namespace core {

			struct MidiTransport
			{
				virtual void transport(ctoot::midi::core::MidiMessage* msg, int timestamp) = 0;
			};

		}
	}
}
