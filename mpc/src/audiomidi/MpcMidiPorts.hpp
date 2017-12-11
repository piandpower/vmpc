#pragma once
#include <Mpc.hpp>

#include <vector>

namespace ctoot {
	namespace midi {
		namespace core {
			struct ConnectedMidiSystem;
			struct MidiInput;
			struct MidiOutput;
			class ShortMessage;
		}
	}
}

namespace mpc {
	namespace audiomidi {

		class MpcMidiPorts
		{

		private:
			std::vector<ctoot::midi::core::MidiInput*> midiInputs{};
			std::vector<ctoot::midi::core::MidiOutput*> midiOutputs{};
			ctoot::midi::core::MidiOutput* midiIn1{ nullptr };
			ctoot::midi::core::MidiOutput* midiIn2{ nullptr };
			ctoot::midi::core::MidiInput* midiOutA{ nullptr };
			ctoot::midi::core::MidiInput* midiOutB{ nullptr };
			ctoot::midi::core::MidiInput* mmsInput{ nullptr };
			Mpc* mpc{ nullptr };

		public:
			std::vector<ctoot::midi::core::MidiOutput*> getTransmitters();
			std::vector<ctoot::midi::core::MidiInput*> getReceivers();
			void setMidiIn1(int i);
			void setMidiIn2(int i);
			void setMidiOutA(int i);
			void setMidiOutB(int i);
			std::string getCurrentMidiIn1DeviceName();
			int getCurrentMidiIn1DevIndex();
			std::string getCurrentMidiIn2DeviceName();
			int getCurrentMidiIn2DevIndex();
			std::string getCurrentMidiOutADeviceName();
			int getCurrentMidiOutADevIndex();
			std::string getCurrentMidiOutBDeviceName();
			int getCurrentMidiOutBDevIndex();
			void close();
			void transmitA(ctoot::midi::core::ShortMessage* msg);
			void transmitB(ctoot::midi::core::ShortMessage* msg);

			MpcMidiPorts(std::shared_ptr<ctoot::midi::core::ConnectedMidiSystem> cms, Mpc* mpc);
			~MpcMidiPorts();
		};

	}
}
