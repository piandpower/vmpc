#pragma once
#include <observer/Observer.hpp>

#include <memory>
#include <vector>
#include <string>

namespace mpc {

	class Mpc;

	namespace sampler {
		class Sampler;
	}

	namespace lcdgui {
		class Field;
		class Label;
	}


	namespace ui {

		namespace sampler {

			class SoundGui;

			class SoundObserver
				: public moduru::observer::Observer
			{

			private:
				mpc::Mpc* mpc;
				SoundGui* soundGui{ nullptr };
				std::weak_ptr<mpc::sampler::Sampler> sampler{ };
				std::weak_ptr<mpc::lcdgui::Field> soundNameField{};
				std::weak_ptr<mpc::lcdgui::Label> typeLabel{};
				std::weak_ptr<mpc::lcdgui::Label> rateLabel{};
				std::weak_ptr<mpc::lcdgui::Label> sizeLabel{};
				std::string csn{ "" };
				std::weak_ptr<mpc::lcdgui::Field> sndField{};
				std::weak_ptr<mpc::lcdgui::Field> convertField{};
				std::vector<std::string> convertNames{};
				std::weak_ptr<mpc::lcdgui::Field> newFsField{};
				std::weak_ptr<mpc::lcdgui::Field> qualityField{};
				std::weak_ptr<mpc::lcdgui::Field> newBitField{};
				std::weak_ptr<mpc::lcdgui::Field> newNameField{};
				std::vector<std::string> qualityNames{};
				std::vector<std::string> bitNames{};
				std::weak_ptr<mpc::lcdgui::Field> stereoSourceField{};
				std::weak_ptr<mpc::lcdgui::Field> newLNameField{};
				std::weak_ptr<mpc::lcdgui::Field> newRNameField{};
				std::weak_ptr<mpc::lcdgui::Field> lSourceField{};
				std::weak_ptr<mpc::lcdgui::Field> rSourceField{};
				std::weak_ptr<mpc::lcdgui::Field> newStNameField{};

			private:
				void displaySnd();
				void displayLSource();
				void displayRSource();
				void displayNewStName();
				void displayStereoSource();
				void displayNewLName();
				void displayNewRName();
				void displayNewFs();
				void displayQuality();
				void displayNewBit();
				void displayNewName();
				void displayConvert();
				void displayDeleteSoundSnd();
				void displaySoundName();
				void displayType();
				void displayRate();
				void displaySize();

			public:
				void update(moduru::observer::Observable* o, boost::any arg) override;

				SoundObserver(mpc::Mpc* mpc);
				~SoundObserver();

			};

		}
	}
}
