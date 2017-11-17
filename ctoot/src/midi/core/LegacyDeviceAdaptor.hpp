#pragma once

#include <midi/core/AbstractMidiDevice.hpp>

#include <string>

namespace ctoot {
	namespace midi {
		namespace core {

			class LegacyMidiInput;
			class LegacyMidiOutput;

			class LegacyDeviceAdaptor
				: public AbstractMidiDevice
			{

			public:
				typedef AbstractMidiDevice super;

			private:
				LegacyMidiInput* input{ nullptr };
				LegacyMidiOutput* output{ nullptr };

			protected:
				std::string name{""};

			public:
				static std::string simpleName(std::string name);

			public:
				virtual void open();
				void closeMidi() override;
				virtual bool isOpen();
				virtual std::string getProvider();
				virtual std::string getDescription();

				LegacyDeviceAdaptor(std::string name, bool isInput);

			private:
				friend class LegacyDevices;
				friend class LegacyMidiInput;
				friend class LegacyMidiOutput;
			};

		}
	}
}
