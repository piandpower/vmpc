#pragma once
#include <midi/message/MidiMsg.hpp>

namespace ctoot {
	namespace midi {
		namespace message {

			class ShortMsg
				: public MidiMsg
			{

			public:
				typedef MidiMsg super;

			private:
				static ctoot::midi::core::MidiMessage* fastShortPrototype_;

			public:
				static bool isShort(ctoot::midi::core::MidiMessage* msg);
				static bool isShort(int status);
				static ctoot::midi::core::MidiMessage* createShort(int status);
				static ctoot::midi::core::MidiMessage* createShort(int status, int data1);
				static ctoot::midi::core::MidiMessage* createShort(int status, int data1, int data2);
				static ctoot::midi::core::MidiMessage* setMessage(ctoot::midi::core::MidiMessage* msg, int status);
				static ctoot::midi::core::MidiMessage* setMessage(ctoot::midi::core::MidiMessage* msg, int status, int data1, int data2);

			private:
				static void createFastPrototypes();

			public:
				static int getData1(ctoot::midi::core::MidiMessage* msg);
				static ctoot::midi::core::MidiMessage* setData1(ctoot::midi::core::MidiMessage* msg, int data1);
				static int getData2(ctoot::midi::core::MidiMessage* msg);
				static ctoot::midi::core::MidiMessage* setData2(ctoot::midi::core::MidiMessage* msg, int data2);
				static int getData1and2(ctoot::midi::core::MidiMessage* msg);
				static int getData1and2(int data1, int data2);

				ShortMsg();

			public:
				static ctoot::midi::core::MidiMessage*& fastShortPrototype();

			};

		};
	};
};