#pragma once
#include "Component.hpp"
//#include <IControl.h>

#include <memory>
#include <vector>
#include <string>

namespace mpc {
	class Mpc;
	namespace ui {
		namespace sampler {
			class MixerGui;
		}
	}

	namespace lcdgui {

		class Field;
		class VerticalBar;
		class MixerKnobBackground;
		class MixerFaderBackground;
		class Knob;

		class MixerStrip
		{

		private:
			std::vector<std::string> abcd;
			std::vector<std::string> letters;
			std::vector<std::weak_ptr<Component>> mixerStrip;
			int columnIndex;
			std::weak_ptr<Field> tf0;
			std::weak_ptr<Field> tf1;
			std::weak_ptr<Field> tf2;
			std::weak_ptr<Field> tf3;
			std::weak_ptr<Field> tf4;
			std::vector<int> xPos0indiv;
			std::vector<int> xPos1indiv;
			int yPos0indiv;
			int yPos1indiv;
			std::vector<int> xPos0fx;
			std::vector<int> xPos1fx;
			int yPos0fx;
			int yPos1fx;
			std::weak_ptr<VerticalBar> verticalBar;
			std::weak_ptr<Knob> knob;
			std::weak_ptr<MixerKnobBackground> mixerKnobBackground;
			std::weak_ptr<MixerFaderBackground> mixerFaderBackground;
			int selection;
			mpc::ui::sampler::MixerGui* mixGui{ nullptr };

		public:
			std::vector<std::weak_ptr<Field>> jta;

		public:
			std::vector<std::weak_ptr<Component>> getMixerStrip();
			void setValueA(int i);
			void setValueB(int i);
			void initFields();
			void setColors();
			void setSelection(int i);
			void setValueAString(std::string str);

			MixerStrip(int columnIndex, int bank, mpc::Mpc* mpc);
			~MixerStrip();

		};

	}
}
