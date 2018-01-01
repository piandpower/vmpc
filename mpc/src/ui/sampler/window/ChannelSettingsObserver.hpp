#pragma once
#include <observer/Observer.hpp>

#include <memory>
#include <vector>
#include <string>

namespace mpc {

	class Mpc;

	namespace lcdgui {
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
	}
	
	namespace ui {
		namespace sampler {

			class SamplerGui;
			class MixerGui;
			class MixerSetupGui;

			class ChannelSettingsObserver
				: public moduru::observer::Observer
			{

			private:
				mpc::Mpc* mpc;
				std::weak_ptr<mpc::sampler::Sampler> sampler{};
				SamplerGui* samplerGui{ nullptr };
				MixerSetupGui* mixerSetupGui{ nullptr };
				MixerGui* mixGui{ nullptr };
				int bank = 0;
				mpc::ctootextensions::MpcSoundPlayerChannel* mpcSoundPlayerChannel{ nullptr };
				std::weak_ptr<mpc::sampler::Program> program{};
				std::weak_ptr<mpc::lcdgui::LayeredScreen> ls{};
				std::weak_ptr<mpc::lcdgui::Field> noteField{};
				std::weak_ptr<mpc::lcdgui::Field> stereoVolumeField{};
				std::weak_ptr<mpc::lcdgui::Field> individualVolumeField{};
				std::weak_ptr<mpc::lcdgui::Field> fxSendLevelField{};
				std::weak_ptr<mpc::lcdgui::Field> panningField{};
				std::weak_ptr<mpc::lcdgui::Field> outputField{};
				std::weak_ptr<mpc::lcdgui::Field> fxPathField{};
				std::weak_ptr<mpc::lcdgui::Field> followStereoField{};
				std::vector<std::string> fxPathNames{};
				std::vector<std::string> stereoNames{};
				std::vector<std::string> stereoNamesSlash{};
				std::vector<std::string> monoNames{};

			private:
				void displayOutputField();
				void displayNoteField();
				void displayPanningField();
				void displayChannel();

			public:
				void update(moduru::observer::Observable* o, boost::any arg) override;

			public:
				ChannelSettingsObserver(mpc::Mpc* mpc);
				~ChannelSettingsObserver();

			};

		}
	}
}
