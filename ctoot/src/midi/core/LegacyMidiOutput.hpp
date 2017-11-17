#pragma once
#include <midi/core/DefaultMidiOutput.hpp>

//#include "RtMidi.h"

namespace ctoot {
	namespace midi {
		namespace core {

			class MidiDevice;
			class LegacyDeviceAdaptor;

			class LegacyMidiOutput
				: public DefaultMidiOutput
			{

			private:
				//::javax::sound::midi::Transmitter* tx{};
				std::string name;
				//RtMidiIn* tx;

			private:
				//static std::vector<RtMidiIn*> transmitters;

			public:
				virtual void closeOutput();
				void addConnectionTo(MidiInput* input) override;
				void processQueue();

			public:
				LegacyMidiOutput(LegacyDeviceAdaptor* adaptor, std::string name);

			protected:
				LegacyDeviceAdaptor* adaptor;

			private:
				//friend class LegacyDevices;
				//friend class LegacyDeviceAdaptor;
				//friend class LegacyMidiInput;
				//friend class LegacyMidiOutput_DeviceMidiOutput_1;
			};

		}
	}
}
