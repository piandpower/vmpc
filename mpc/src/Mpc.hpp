#pragma once
#include <vector>
#include <string>
#include <thread>
#include <memory>

namespace mpc {

	namespace ui {
		class UserDefaults;
		class Uis;
	}

	namespace hardware {
		class Hardware;
	}

	namespace controls {
		class Controls;
		class AbstractControls;
	}

	namespace lcdgui {
		class LayeredScreen;
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
		std::shared_ptr<ui::Uis> uis;
		std::shared_ptr<lcdgui::LayeredScreen> layeredScreen;
		std::shared_ptr<controls::Controls> controls;

	private:
		std::shared_ptr<audiomidi::EventHandler> eventHandler;
		std::shared_ptr<sequencer::Sequencer> sequencer;
		std::shared_ptr<sampler::Sampler> sampler;
		std::shared_ptr<audiomidi::AudioMidiServices> audioMidiServices;

	private:
		std::shared_ptr<hardware::Hardware> hardware;

	public:
		void init();
		//void startMidi();

	public:
		std::weak_ptr<ui::Uis> getUis();
		std::weak_ptr<lcdgui::LayeredScreen> getLayeredScreen();
		std::weak_ptr<controls::Controls> getControls();
		controls::AbstractControls* getActiveControls();
		std::weak_ptr<hardware::Hardware> getHardware();

	public:
		std::weak_ptr<sequencer::Sequencer> getSequencer();
		std::weak_ptr<sampler::Sampler> getSampler();
		ctootextensions::MpcSoundPlayerChannel* getDrum(int i);
		ctootextensions::MpcBasicSoundPlayerChannel* getBasicPlayer();
		std::weak_ptr<audiomidi::AudioMidiServices> getAudioMidiServices();
		std::vector<ctootextensions::MpcSoundPlayerChannel*> getDrums();
		std::weak_ptr<audiomidi::EventHandler> getEventHandler();

	public:
		void loadSound(bool replace);
		void loadProgram();
		void importLoadedProgram();

	public:
		Mpc();
		~Mpc();

	public:
		static std::vector<char> akaiAsciiChar;
		static std::vector<std::string> akaiAscii;

	};

}
