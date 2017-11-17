#pragma once
#include <midi/core/MidiMessage.hpp>

#include <vector>

namespace ctoot {
	namespace midi {
		namespace core {

			class ShortMessage
				: public MidiMessage
			{

			public:
				typedef MidiMessage super;
				static const int ACTIVE_SENSING{ 254 };
				static const int CHANNEL_PRESSURE{ 208 };
				static const int CONTINUE{ 251 };
				static const int CONTROL_CHANGE{ 176 };
				static const int END_OF_EXCLUSIVE{ 247 };
				static const int MIDI_TIME_CODE{ 241 };
				static const int NOTE_OFF{ 128 };
				static const int NOTE_ON{ 144 };
				static const int PITCH_BEND{ 224 };
				static const int POLY_PRESSURE{ 160 };
				static const int PROGRAM_CHANGE{ 192 };
				static const int SONG_POSITION_POINTER{ 242 };
				static const int SONG_SELECT{ 243 };
				static const int START{ 250 };
				static const int STOP{ 252 };
				static const int SYSTEM_RESET{ 255 };
				static const int TIMING_CLOCK{ 248 };
				static const int TUNE_REQUEST{ 246 };

			public:
				//::java::lang::Object* clone(); override;
				virtual int getChannel();
				virtual int getCommand();
				virtual int getData1();
				virtual int getData2();

			public:
				int getDataLength(int status);

			public:
				virtual void setMessage(int status);
				virtual void setMessage(int status, int data1, int data2);
				virtual void setMessage(int command, int channel, int data1, int data2);

				ShortMessage();
				ShortMessage(std::vector<char> data);

			public:
				virtual void setMessage(std::vector<char> data, int length);

			};


		}
	}
}
