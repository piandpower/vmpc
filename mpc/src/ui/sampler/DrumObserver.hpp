#pragma once
#include <observer/Observer.hpp>

#include <memory>

namespace mpc {

	class Mpc;

	namespace sampler {
		class Sampler;
	}

	namespace ctootextensions {
		class MpcSoundPlayerChannel;
	}

	namespace lcdgui {
		class Field;
		class Label;
	}

	namespace ui {

		namespace sampler {

			class SamplerGui;

			class DrumObserver
				: public moduru::observer::Observer
			{

			private:
				SamplerGui* samplerGui{ nullptr };
				std::weak_ptr<mpc::lcdgui::Field> drumField{};
				std::weak_ptr<mpc::lcdgui::Field> padToInternalSoundField{};
				std::weak_ptr<mpc::lcdgui::Field> pgmField{};
				std::weak_ptr<mpc::lcdgui::Field> pgmChangeField{};
				std::weak_ptr<mpc::lcdgui::Field> midiVolumeField{};
				std::weak_ptr<mpc::lcdgui::Field> currentValField{};
				mpc::Mpc* mpc{ nullptr };
				std::weak_ptr<mpc::sampler::Sampler> sampler{};
				mpc::ctootextensions::MpcSoundPlayerChannel* mpcSoundPlayerChannel{ nullptr };
				std::weak_ptr<mpc::lcdgui::Label> valueLabel{};

			private:
				void displayValue();
				void displayDrum();
				void displayPadToInternalSound();
				void displayPgm();
				void displayPgmChange();
				void displayMidiVolume();
				void displayCurrentVal();

			public:
				void update(moduru::observer::Observable* o, boost::any arg) override;

			public:
				DrumObserver(mpc::Mpc* mpc);
				~DrumObserver();

			};

		}
	}
}
