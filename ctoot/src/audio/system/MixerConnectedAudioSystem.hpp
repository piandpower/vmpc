#pragma once
#include <audio/system/DefaultAudioSystem.hpp>
#include <audio/system/ConnectedAudioSystem.hpp>
#include <audio/mixer/AudioMixer.hpp>

#include <boost/any.hpp>

namespace ctoot {
	namespace audio {
		namespace system {

			class MixerConnectedAudioSystem
				: public DefaultAudioSystem
				, public ConnectedAudioSystem
			{

			public:
				typedef DefaultAudioSystem super;

			private:
				std::vector<AudioConnection*> connections{};
				std::weak_ptr<ctoot::audio::mixer::AudioMixer> mixer{ };
				std::weak_ptr<ctoot::audio::mixer::MixerControls> mixerControls{ };

			public:
				void notifyObservers(boost::any arg) override;
				std::vector<AudioConnection*>* getConnections() override;
				virtual void createConnection(std::string fromPortName, std::string fromPortLocation, std::string toPortName, int flags);

			public:
				virtual void createConnection(AudioOutput* from, ctoot::audio::mixer::AudioMixerStrip* to, int flags);
				virtual AudioConnection* getConnectionFrom(std::string from, std::string fromLocation);
				virtual void createConnectionFrom(AudioOutput* output);
				virtual void closeConnectionFrom(AudioOutput* output);
				virtual void closeConnection(AudioConnection* connection);
				virtual AudioOutput* getOutputPort(std::string name, std::string location);
				virtual std::string nameAndLocation(AudioOutput* output);

			public:
				MixerConnectedAudioSystem(std::shared_ptr<ctoot::audio::mixer::AudioMixer> mixer);
				~MixerConnectedAudioSystem();

			public:
				void addAudioDevice(AudioDevice* device);
				void close();
				std::vector<AudioDevice*> getAudioDevices();
				std::vector<AudioInput*> getAudioInputs();
				std::vector<AudioOutput*> getAudioOutputs();
				void removeAudioDevice(AudioDevice* device);
				void setAutoConnect(bool autoConnect);

			private:
//				friend class MixerInputConnection;
			};

		}
	}
}
