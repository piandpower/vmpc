#pragma once
#include <vector>
#include <string>
#include <thread>
#include <memory>

namespace mpc {

	namespace ui {
		class UserDefaults;
	}

	namespace ctootextensions {
		class MpcSoundPlayerChannel;
		class MpcBasicSoundPlayerChannel;
		class MpcMultiMidiSynth;
	}

	namespace audiomidi {
		class EventHandler;
		class AudioMidiServices;
		class MpcMidiInput;
		class MpcMidiPorts;
	}

	namespace sequencer {
		class Sequencer;
	}

	namespace sampler {
		class Sampler;
	}

	class Mpc
	{
	
	private:
		std::shared_ptr<mpc::ui::UserDefaults> userDefaults;
		std::shared_ptr<mpc::audiomidi::EventHandler> eventHandler;
		std::shared_ptr<mpc::sequencer::Sequencer> sequencer;
		std::shared_ptr<mpc::sampler::Sampler> sampler;
		std::shared_ptr<mpc::audiomidi::AudioMidiServices> audioMidiServices{};

	public:
		void init();
		std::weak_ptr<mpc::sequencer::Sequencer> getSequencer();
		std::weak_ptr<mpc::sampler::Sampler> getSampler();
		void loadSound(bool replace);
		void loadProgram();
		void importLoadedProgram();
		void startMidi();
		mpc::ctootextensions::MpcSoundPlayerChannel* getDrum(int i);
		mpc::ctootextensions::MpcBasicSoundPlayerChannel* getBasicPlayer();
		std::weak_ptr<mpc::audiomidi::AudioMidiServices> getAudioMidiServices();
		std::vector<mpc::ctootextensions::MpcSoundPlayerChannel*> getDrums();
		std::weak_ptr<mpc::audiomidi::EventHandler> getEventHandler();

	public:
		Mpc();
		~Mpc();

	public:
		static std::vector<char> akaiAsciiChar;
		static std::vector<std::string> akaiAscii;

	};

}
