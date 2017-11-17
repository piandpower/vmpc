#pragma once
#include <midi/message/ChannelMsg.hpp>

namespace ctoot {
	namespace midi {
		namespace message {

			class PitchMsg
				: public ChannelMsg
			{

			public:
				typedef ChannelMsg super;
				static bool isPitch(ctoot::midi::core::MidiMessage* msg);
				static bool isPitch(int status);
				static ctoot::midi::core::MidiMessage* transpose(ctoot::midi::core::MidiMessage* msg, int semitones);
				static int getPitch(ctoot::midi::core::MidiMessage* msg);
				static ctoot::midi::core::MidiMessage* setPitch(ctoot::midi::core::MidiMessage* msg, int pitch);

				PitchMsg();
			};
	
		};
	};
};