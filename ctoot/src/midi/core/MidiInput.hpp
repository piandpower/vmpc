#pragma once
#include <midi/core/MidiPort.hpp>
#include <midi/core/MidiTransport.hpp>

namespace ctoot {
	namespace midi {
		namespace core {

			struct MidiInput
				: public virtual MidiPort
				, public virtual MidiTransport
			{

			};

		}
	}
}
