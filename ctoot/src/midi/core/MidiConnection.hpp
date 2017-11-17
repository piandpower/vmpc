#pragma once

namespace ctoot {
	namespace midi {
		namespace core {

			struct MidiOutput;
			struct MidiInput;

			class MidiConnection
			{

			public:
				static const int PLAYBACK{ 1 };
				static const int SYSTEM{ 2 };
				static const int OUTPUT_LOCKED{ 4 };
				static const int INPUT_LOCKED{ 8 };

			private:
				MidiOutput* from{ nullptr };
				MidiInput* to{ nullptr };
				int flags{ 0 };

			public:
				virtual void connectTo(MidiInput* to);
				virtual void connectFrom(MidiOutput* from);
				virtual void close();
				virtual MidiOutput* getMidiOutput();
				virtual MidiInput* getMidiInput();
				virtual bool isSystem();
				virtual bool isPlayback();

				MidiConnection(MidiOutput* from, MidiInput* to, int flags);
				MidiConnection(MidiOutput* from, MidiInput* to) : MidiConnection(from, to, 0) {};

				~MidiConnection();

			};

		}
	}
}
