#pragma once
#include <observer/Observer.hpp>

#include <memory>
#include <vector>
#include <string>

namespace mpc {

	class Mpc;

	namespace lcdgui {
		class MixerStrip;
		class LayeredScreen;
		class Field;
	}


	namespace ctootextensions {
		class MpcSoundPlayerChannel;
	}

	namespace sequencer {
		class Sequencer;
		class Sequence;
		class Track;
	}

	namespace sampler {
		class Sampler;
		class Program;
		class Pad;
		class MixerChannel;
	}
	
	namespace ui {
		namespace sampler {

			class SamplerGui;
			class MixerGui;
			class MixerSetupGui;

			class MixerObserver
				: public moduru::observer::Observer
			{

			private:
				mpc::Mpc* mpc;
				std::weak_ptr<mpc::sampler::Sampler> sampler{};
				std::vector<std::string> fxPathNames{};
				std::vector<std::string> stereoNames{};
				std::vector<std::string> stereoNamesSlash{};
				std::vector<std::string> monoNames{};
				MixerGui* mixGui{ nullptr };
				std::weak_ptr<mpc::lcdgui::LayeredScreen> ls{};
				SamplerGui* samplerGui{ nullptr };
				MixerSetupGui* mixerSetupGui{ nullptr };
				int bank{};
				mpc::ctootextensions::MpcSoundPlayerChannel* mpcSoundPlayerChannel{ nullptr };
				std::weak_ptr<mpc::sampler::Program> program{};
				mpc::sampler::Pad* pad{ nullptr };
				std::weak_ptr<mpc::sampler::MixerChannel> mixerChannel{};
				std::vector<mpc::lcdgui::MixerStrip*> mixerStrips{};
				std::weak_ptr<mpc::lcdgui::Field> noteField{};
				std::weak_ptr<mpc::lcdgui::Field> stereoVolumeField{};
				std::weak_ptr<mpc::lcdgui::Field> individualVolumeField{};
				std::weak_ptr<mpc::lcdgui::Field> fxSendLevelField{};
				std::weak_ptr<mpc::lcdgui::Field> panningField{};
				std::weak_ptr<mpc::lcdgui::Field> outputField{};
				std::weak_ptr<mpc::lcdgui::Field> fxPathField{};
				std::weak_ptr<mpc::lcdgui::Field> followStereoField{};
				std::weak_ptr<mpc::lcdgui::Field> masterLevelField{};
				std::weak_ptr<mpc::lcdgui::Field> fxDrumField{};
				std::weak_ptr<mpc::lcdgui::Field> stereoMixSourceField{};
				std::weak_ptr<mpc::lcdgui::Field> indivFxSourceField{};
				std::weak_ptr<mpc::lcdgui::Field> copyPgmMixToDrumField{};
				std::weak_ptr<mpc::lcdgui::Field> recordMixChangesField{};

			private:
				void initPadNameLabels();
				void initMixerStrips();
				void displayChannel();
				void setOutputField();
				void setNoteField();
				void setPanningField();
				void displayMixerStrips();
				void displayMasterLevel();
				void displayFxDrum();
				void displayStereoMixSource();
				void displayIndivFxSource();
				void displayCopyPgmMixToDrum();
				void displayRecordMixChanges();

			public:
				void update(moduru::observer::Observable* o, boost::any arg) override;
				void displayFunctionKeys();

			public:
				MixerObserver(mpc::Mpc* mpc);
				~MixerObserver();

			};

		}
	}
}
