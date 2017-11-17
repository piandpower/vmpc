#pragma once
#include <midi/core/MidiDevice.hpp>

#include <string>

namespace ctoot {
	namespace midi {
		namespace core {

			class AbstractMidiDevice
				: public virtual MidiDevice
			{

			public:
				std::vector<MidiInput*> inputs;
				std::vector<MidiOutput*> outputs;

			private:
				std::string name{ "" };

			public:
				std::vector<MidiInput*> getMidiInputs() override;
				std::vector<MidiOutput*> getMidiOutputs() override;
				std::string getName() override;
				std::string toString();

			public:
				virtual void addMidiInput(MidiInput* input);
				virtual void removeMidiInput(MidiInput* input);
				virtual void removeAllMidiInputs();
				virtual void addMidiOutput(MidiOutput* output);
				virtual void removeMidiOutput(MidiOutput* output);
				virtual void removeAllMidiOutputs();

			public:
				AbstractMidiDevice(std::string name);
				AbstractMidiDevice();

			};

		}
	}
}
