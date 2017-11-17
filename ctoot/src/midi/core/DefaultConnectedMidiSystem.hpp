#pragma once
#include <midi/core/DefaultMidiSystem.hpp>
#include <midi/core/ConnectedMidiSystem.hpp>

namespace ctoot {
	namespace midi {
		namespace core {

			struct MidiPort;

			class DefaultConnectedMidiSystem
				: public DefaultMidiSystem
				, public virtual ConnectedMidiSystem
			{

			public:
				typedef DefaultMidiSystem super;

			private:
				std::vector<MidiConnection*> connections{};

			public:
				void notifyObservers() override;

			public:
				virtual void checkConnections();

			public:
				std::vector<MidiConnection*> getMidiConnections() override;
				void createMidiConnection(MidiOutput* from, MidiInput* to, int flags) override;
				void closeMidiConnection(MidiOutput* from, MidiInput* to) override;
				void createMidiConnection(std::string* fromPortName, std::string* toPortName, int flags) override;
				void closeMidiConnection(std::string* fromPortName, std::string* toPortName) override;

			public:
				virtual MidiConnection* getConnection(MidiOutput* from, MidiInput* to);
				virtual MidiPort* getPort(std::string* name, bool isOut);

			public:
				DefaultConnectedMidiSystem();
				~DefaultConnectedMidiSystem();

			};

		}
	}
}
