#pragma once
#include <control/FloatControl.hpp>
#include <vector>
#include <control/ControlLaw.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class LCRControl
				: public ctoot::control::FloatControl
			{

			public:
				typedef ctoot::control::FloatControl super;

			private:
				static std::vector<std::string>* presetNames_;

			public:
				virtual float getLeft() = 0;
				virtual float getRight() = 0;
				std::vector<std::string>* getPresetNames() override;
				void applyPreset(std::string presetName); // override

				LCRControl(std::string name, ctoot::control::ControlLaw law, float precision, float initialValue);
				virtual ~LCRControl();

			private:
				static std::vector<std::string>*& presetNames();
				//friend class MixControls;
			//	friend class PanControl;
		//		friend class BalanceControl;
	//			friend class FrontRearControl;
//				friend class GainControl;
			};

		}
	}
}
