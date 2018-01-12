#pragma once
#include <midi/message/ShortMsg.hpp>

#ifdef __linux__
#include <cstdint>>
#endif // __linux__

namespace ctoot {
	namespace midi {
		namespace message {

			class ChannelMsg
				: public ShortMsg
			{

			public:
				typedef ShortMsg super;
				static bool isChannel(ctoot::midi::core::MidiMessage* msg);
				static bool isChannel(int status);
				static ctoot::midi::core::MidiMessage* createChannel(int status, int chan, int data1, int data2);
				static ctoot::midi::core::MidiMessage* createChannel(int status, int chan, int data1);
				static int getCommand(ctoot::midi::core::MidiMessage* msg);
				static int getCommand(int status);
				static int getChannel(ctoot::midi::core::MidiMessage* msg);
				static int getChannel(int status);
				static ctoot::midi::core::MidiMessage* setChannel(ctoot::midi::core::MidiMessage* msg, int chan);
				static int setChannel(int status, int chan);
				static const int NOTE_OFF{ int32_t(128) };
				static const int NOTE_ON{ int32_t(144) };
				static const int POLY_PRESSURE{ int32_t(160) };
				static const int CONTROL_CHANGE{ int32_t(176) };
				static const int PROGRAM_CHANGE{ int32_t(192) };
				static const int CHANNEL_PRESSURE{ int32_t(208) };
				static const int PITCH_BEND{ int32_t(224) };

				ChannelMsg();

			};

		};
	};
};
