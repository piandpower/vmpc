#pragma once
#include <midi/message/PitchMsg.hpp>

namespace ctoot {
	namespace midi {

		namespace core {
			class MidiMessage;
		};

		namespace message {


			class NoteMsg
				: public PitchMsg
			{

			public:
				typedef PitchMsg super;

			private:
				static int noteOff_;

			public:
				static bool isNote(ctoot::midi::core::MidiMessage* msg);
				static bool isNote(int status);
				static ctoot::midi::core::MidiMessage* on(int chan, int note, int vel);
				static ctoot::midi::core::MidiMessage* off(int chan, int note, int vel);
				static ctoot::midi::core::MidiMessage* off(int chan, int note);
				static bool isOn(ctoot::midi::core::MidiMessage* msg);
				static bool isOn(int status, int data2);
				static bool isOff(ctoot::midi::core::MidiMessage* msg);
				static bool isOff(int status, int data2);
				static int getVelocity(ctoot::midi::core::MidiMessage* msg);
				static ctoot::midi::core::MidiMessage* setVelocity(ctoot::midi::core::MidiMessage* msg, int vel);
				static ctoot::midi::core::MidiMessage* louden(ctoot::midi::core::MidiMessage* msg, int velocityDelta);

				NoteMsg();

			private:
				static int& noteOff();
			};
	
		};
	};
};