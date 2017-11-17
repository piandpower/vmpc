#pragma once
#include <midi/core/MidiInput.hpp>
#include <midi/core/MidiOutput.hpp>
#include <observer/Observable.hpp>
//#include <misc/IObservable.hpp>

#include <string>
#include <vector>

namespace ctoot {
	namespace midi {
		namespace core {

			struct MidiDevice
				: virtual moduru::observer::Observable
			{
				virtual std::string getName() = 0;
				virtual std::vector<MidiInput*> getMidiInputs() = 0;
				virtual std::vector<MidiOutput*> getMidiOutputs() = 0;
				virtual void closeMidi() {}

			};

		}
	}
}
