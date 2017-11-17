#pragma once
#include <midi/core/AbstractMidiDevice.hpp>
#include <synth/MidiSynth.hpp>
#include <midi/core/MidiOutput.hpp>

#include <vector>
#include <string>

namespace ctoot {

	namespace audio {
		namespace system {
			struct AudioOutput;
		}
	}

	namespace synth {
		class SynthChannel;
		class BasicMidiSynth
			: public ctoot::midi::core::AbstractMidiDevice
			, public ctoot::synth::MidiSynth
		{

		private:
			std::vector<ctoot::audio::system::AudioOutput*> audioOutputs{};
			std::vector<std::shared_ptr<SynthChannel>> synthChannels{};
			std::string location{ "" };

		public:
			void setLocation(std::string location) override;
			virtual std::string getLocation();

		public:
			virtual void setChannel(int chan, std::shared_ptr<SynthChannel> synthChannel);

		public:
			virtual std::vector<std::shared_ptr<SynthChannel>>* getChannels();
			virtual std::weak_ptr<SynthChannel> getChannel(int chan);

		public:
			virtual void transportChannel(ctoot::midi::core::MidiMessage* msg, SynthChannel* synthChannel);
			virtual std::weak_ptr<SynthChannel> mapChannel(int chan);
			virtual void transportChannel(ctoot::midi::core::MidiMessage* msg, int chan);

		public:
			void transport(ctoot::midi::core::MidiMessage* msg, int timestamp) override;
			void closeMidi() override;

		public:
			virtual void addAudioOutput(ctoot::audio::system::AudioOutput* output);
			virtual void removeAudioOutput(ctoot::audio::system::AudioOutput* output);

		public:
			std::vector<ctoot::audio::system::AudioOutput*> getAudioOutputs() override;
			std::vector<ctoot::audio::system::AudioInput*> getAudioInputs() override;
			void closeAudio() override;
			std::string getName() override;


			BasicMidiSynth(std::string name);
			~BasicMidiSynth();

		};

	}
}
