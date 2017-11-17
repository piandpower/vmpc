#pragma once
#include <midi/core/MidiPort.hpp>
#include <midi/core/MidiInput.hpp>

namespace ctoot {
	namespace midi {
		namespace core {

			struct MidiOutput
				: public virtual MidiPort
			{
				virtual void addConnectionTo(MidiInput* to) = 0;
				virtual void removeConnectionTo(MidiInput* to) = 0;

			};

		}
	}
}
