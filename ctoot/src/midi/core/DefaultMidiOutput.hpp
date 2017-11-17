#pragma once
#include <midi/core/MidiOutput.hpp>
#include <midi/core/MidiTransport.hpp>

namespace ctoot {
	namespace midi {

		namespace core {
			
			class MidiMessage;

			class DefaultMidiOutput
				: public virtual MidiOutput
				, public virtual MidiTransport
			{


			private:
				std::string name{ "" };
				std::vector<MidiInput*> tos{};

			public:
				void addConnectionTo(MidiInput* input) override;
				void removeConnectionTo(MidiInput* input) override;
				virtual int getConnectionCount();
				void transport(ctoot::midi::core::MidiMessage* msg, int timestamp) override;
				virtual void setName(std::string name);
				std::string getName() override;
				std::string toString();

				DefaultMidiOutput(std::string name);
			};

		}
	}
}
