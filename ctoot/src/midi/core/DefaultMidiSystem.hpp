#pragma once
#include <midi/core/MidiSystem.hpp>
#include <audio/server/AudioClient.hpp>

namespace ctoot {
	namespace midi {
		namespace core {

			class DefaultMidiSystem
				: public virtual MidiSystem
				, public ctoot::audio::server::AudioClient
			{
			public:
				void work(int nFrames);
				void setEnabled(bool enabled);

			private:
				std::vector<MidiDevice*> devices{};
				moduru::observer::Observer* observer{ nullptr };

			public:
				void addMidiDevice(MidiDevice* device) override;
				void removeMidiDevice(MidiDevice* device) override;
				std::vector<MidiDevice*> getMidiDevices() override;
				std::vector<MidiInput*> getMidiInputs() override;
				std::vector<MidiOutput*> getMidiOutputs() override;
				void close() override;

				DefaultMidiSystem();
				~DefaultMidiSystem();
			};

		}
	}
}
