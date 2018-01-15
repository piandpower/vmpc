#pragma once
#include <vector>
#include <string>
#include <thread>
#include <memory>

#include <lcdgui/LayeredScreen.hpp>
#include <ui/Uis.hpp>
#include <Logger.hpp>
#include <disk/ProgramLoader.hpp>
#include "DiskController.hpp"

namespace mpc {

	namespace ui {
		class UserDefaults;
		class Uis;
	}

	namespace hardware {
		class Hardware;
	}

	namespace disk {
		class Stores;
		class AbstractDisk;
	}

	namespace controls {
		class Controls;
		class AbstractControls;
		class GlobalReleaseControls;
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
		std::thread loadSoundThread{};
		std::unique_ptr<mpc::disk::ProgramLoader> programLoader{};

	private:
		std::shared_ptr<ui::Uis> uis;
		std::shared_ptr<lcdgui::LayeredScreen> layeredScreen;
		std::shared_ptr<controls::Controls> controls;

	private:
		std::shared_ptr<audiomidi::EventHandler> eventHandler;
		std::shared_ptr<sequencer::Sequencer> sequencer;
		std::shared_ptr<sampler::Sampler> sampler;
		std::shared_ptr<audiomidi::AudioMidiServices> audioMidiServices;
		std::vector<audiomidi::MpcMidiInput*> mpcMidiInputs;

	private:
		std::unique_ptr<DiskController> diskController{};

	private:
		std::shared_ptr<hardware::Hardware> hardware;

	public:
		void init(std::string mode, int sampleRate);
		void powerOn();
		//void startMidi();

	public:
		std::weak_ptr<ui::Uis> getUis();
		std::weak_ptr<lcdgui::LayeredScreen> getLayeredScreen();
		std::weak_ptr<controls::Controls> getControls();
		controls::AbstractControls* getActiveControls();
		controls::GlobalReleaseControls* getReleaseControls();
		std::weak_ptr<hardware::Hardware> getHardware();

	public:
		std::weak_ptr<sequencer::Sequencer> getSequencer();
		std::weak_ptr<sampler::Sampler> getSampler();
		ctootextensions::MpcSoundPlayerChannel* getDrum(int i);
		ctootextensions::MpcBasicSoundPlayerChannel* getBasicPlayer();
		std::weak_ptr<audiomidi::AudioMidiServices> getAudioMidiServices();
		std::vector<ctootextensions::MpcSoundPlayerChannel*> getDrums();
		std::weak_ptr<audiomidi::EventHandler> getEventHandler();
		mpc::ctootextensions::MpcMultiMidiSynth* getMms();
		std::weak_ptr<mpc::audiomidi::MpcMidiPorts> getMidiPorts();
		mpc::audiomidi::MpcMidiInput* getMpcMidiInput(int i);


	public:
		void loadSound(bool replace);
		void loadProgram();
		void importLoadedProgram();

	private:
		void runLoadSoundThread(int size);
		static void static_loadSound(void* this_p, int size);

	public:
		std::weak_ptr<mpc::disk::AbstractDisk> getDisk();
		std::weak_ptr<mpc::disk::Stores> getStores();

	public:
		Mpc();
		~Mpc();

	public:
		static std::vector<char> akaiAsciiChar;
		static std::vector<std::string> akaiAscii;

	};

}
