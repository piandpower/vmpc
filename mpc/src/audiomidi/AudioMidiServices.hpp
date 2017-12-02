#pragma once
#include <observer/Observable.hpp>
#include <Mpc.hpp>
#include <audio/server/AudioServer.hpp>
#include <audio/mixer/AudioMixer.hpp>
#include <sequencer/FrameSeq.hpp>

#include <observer/Observable.hpp>

#include <set>
#include <memory>

namespace ctoot {
	namespace audio {

		namespace core {
			class AudioFormat;
		}

		namespace server {
			class NonRealTimeAudioServer;
			class CompoundAudioClient;
			class PluginAudioServer;
			class RtAudioServer;
			class UnrealAudioServer;
		}

		namespace system {
			class DefaultAudioSystem;
		}
	}

	namespace midi {
		namespace core {
			class DefaultConnectedMidiSystem;
		}
	}

	namespace synth {
		class SynthRack;
		class SynthRackControls;
		class SynthChannelControls;
	}
}

namespace mpc {

	namespace ctootextensions {
		class MpcMixerControls;
		class MpcMultiMidiSynth;
		class MpcMultiSynthControls;
		class Voice;
		class DummyAudioProcess;
	}

	namespace audiomidi {

		class MpcMidiPorts;
		class DirectToDiskSettings;
		class ExportAudioProcessAdapter;

		class AudioMidiServices
			: public moduru::observer::Observable
		{

		private:
			bool disabled{ true };
			bool bouncePrepared{ false };
			bool bouncing{ false };
			std::vector<std::string> directSoundOutDevNames;
			std::vector<std::string> directSoundInDevNames;
			std::vector<std::string> asioDeviceNames;
			std::vector<std::string> coreAudioOutDeviceNames;
			std::vector<std::string> coreAudioInDeviceNames;

			std::shared_ptr<ctoot::audio::core::AudioFormat> format{};
			std::vector<std::shared_ptr<mpc::ctootextensions::Voice>> voices{};
			std::shared_ptr<mpc::ctootextensions::Voice> basicVoice{};
			std::vector<std::shared_ptr<ctoot::synth::SynthChannelControls>> synthChannelControls{};
			int serverIndex{ -1 };
			std::shared_ptr<ctoot::audio::server::AudioServer> server{};
			std::shared_ptr<ctoot::audio::server::NonRealTimeAudioServer> offlineServer{};
			std::shared_ptr<ctoot::audio::system::DefaultAudioSystem> audioSystem{};
			std::shared_ptr<ctoot::midi::core::DefaultConnectedMidiSystem> midiSystem{};
			std::shared_ptr<ctoot::audio::mixer::AudioMixer> mixer{};
			std::shared_ptr<ctoot::synth::SynthRackControls> synthRackControls{};
			std::shared_ptr<ctoot::synth::SynthRack> synthRack{};
			std::shared_ptr<ctootextensions::MpcMultiSynthControls> msc{};
			std::shared_ptr<ctootextensions::MpcMixerControls> mixerControls{};
			std::weak_ptr<ctootextensions::MpcMultiMidiSynth> mms{};
			std::shared_ptr<ctoot::audio::server::CompoundAudioClient> cac{};
			//std::shared_ptr<MpcMidiPorts> mpcMidiPorts{};
			Mpc* mpc{ nullptr };
			std::vector<std::string> serverNames{};
			std::vector<std::string> inputNames{};
			std::vector<std::string> outputNames{};
			std::vector<int> selectedInputs{};
			std::vector<int> selectedOutputs{};
			std::vector<std::shared_ptr<ExportAudioProcessAdapter>> exportProcesses{};
			std::shared_ptr<mpc::ctootextensions::DummyAudioProcess> dummyProcess{};
			std::vector<ctoot::audio::server::IOAudioProcess*> inputProcesses{};
			std::vector<ctoot::audio::server::IOAudioProcess*> outputProcesses{};
			std::shared_ptr<mpc::sequencer::FrameSeq> frameSeq{};
			std::vector<int> oldPrograms{};
			int requestedBufferSize{ -1 };

		public:
			void init();
			ctoot::audio::server::UnrealAudioServer* getUnrealAudioServer();

		private:

			void initServerNames();

			void destroySynth();
			void destroyDiskWriter();
			void setupServer(int index);
			void setupMidi();
			void setupMixer();
			void setGroupLevel(int i);
			void openIO(int serverIndex);
			void createSynth();
			void loadIOConfig();

		public:
			void saveConfig();
			std::vector<int> getSelectedInputs();
			std::vector<int> getSelectedOutputs();
			void setSelectedInput(int virtualInput, int hardwareInput);
			void setSelectedOutput(int virtualOutput, int hardwareOutput);
			std::weak_ptr<ctoot::audio::server::AudioServer> getAudioServer();
			ctoot::audio::server::NonRealTimeAudioServer* getOfflineServer();
			std::weak_ptr<ctoot::audio::server::PluginAudioServer> getPluginAudioServer();
			void setMasterLevel(int i);
			std::vector<std::string> getInputNames();
			std::vector<std::string> getOutputNames();
			std::weak_ptr<ctootextensions::MpcMultiMidiSynth> getMms();
			void initializeDiskWriter();
			void closeIO();
			void disable();

		public:
			void connectVoices();
			//std::weak_ptr<MpcMidiPorts> getMidiPorts();
			int getServerIndex();
			int getServerCount();
			std::string getServerName(int i);
			std::string getCurrentServerName();
			void create(int serverIndex, int bufferSize);
			bool isAsio();
			void openAsioControlPanel();
			bool isCoreAudio();
			bool isDirectSound();
			bool isStandalone();
			void destroyServices();
			void setDriver(int i);
			void prepareBouncing(DirectToDiskSettings* settings);
			void startBouncing();
			void stopBouncing();
			std::weak_ptr<mpc::sequencer::FrameSeq> getFrameSequencer();
			bool isBouncePrepared();
			bool isBouncing();
			bool isDisabled();
			ctoot::audio::server::IOAudioProcess* getAudioInput(int input);
			void setBufferSize(int size);
			int getBufferSize();

		public:
			int getDirectSoundInputDevCount();
			int getDirectSoundOutputDevCount();

		public:
			void startTestMode();

		public:
			AudioMidiServices(Mpc* mpc);
			~AudioMidiServices();

		};

	}
}
