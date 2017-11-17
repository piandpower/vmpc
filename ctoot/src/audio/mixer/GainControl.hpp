#pragma once
#include <audio/fader/FaderControl.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class GainControl
				: public audio::fader::FaderControl
			{

			private:
				static std::vector<std::string> presetNames_;

			public:
				void setValue(float value) override;
				std::vector<std::string>* getPresetNames() override;
				void applyPreset(std::string name) override;

				GainControl(bool muted);
				virtual ~GainControl();

			private:
				friend class MixControls;
				friend class LCRControl;
				friend class PanControl;
				friend class BalanceControl;
				friend class FrontRearControl;
			};

		}
	}
}
