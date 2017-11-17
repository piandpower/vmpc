#pragma once

#include <vector>
#include <string>

namespace ctoot {
	namespace midi {
		namespace misc {

			class Controller
			{

			public:
				static const int BANK_SELECT{ 0 };
				static const int MODULATION{ 1 };
				static const int BREATH{ 2 };
				static const int CONTROLLER_3{ 3 };
				static const int FOOT{ 4 };
				static const int PORTAMENTO{ 5 };
				static const int DATA_ENTRY{ 6 };
				static const int VOLUME{ 7 };
				static const int BALANCE{ 8 };
				static const int CONTROLLER_9{ 9 };
				static const int PAN{ 10 };
				static const int EXPRESSION{ 11 };
				static const int FX_1{ 12 };
				static const int FX_2{ 13 };
				static const int HOLD_PEDAL{ 64 };
				static const int PORTAMENTO_SWITCH{ 65 };
				static const int SUSTENUTO_PEDAL{ 66 };
				static const int SOFT_PEDAL{ 67 };
				static const int LEGATO{ 68 };
				static const int HOLD_2_PEDAL{ 69 };
				static const int VARIATION{ 70 };
				static const int TIMBRE{ 71 };
				static const int RELEASE{ 72 };
				static const int ATTACK{ 73 };
				static const int BRIGHTNESS{ 74 };
				static const int EXTERNAL_FX{ 91 };
				static const int TREMELO{ 92 };
				static const int CHORUS{ 93 };
				static const int DETUNE{ 94 };
				static const int PHASER{ 95 };
				static const int DATA_INCREMENT{ 96 };
				static const int DATA_DECREMENT{ 97 };
				static const int NON_REGISTERED_FINE{ 98 };
				static const int NON_REGISTERED_COARSE{ 99 };
				static const int REGISTERED_FINE{ 100 };
				static const int REGISTERED_COARSE{ 101 };
				static const int ALL_SOUND_OFF{ 120 };
				static const int ALL_CONTROLLERS_OFF{ 121 };
				static const int LOCAL_CONTROL{ 122 };
				static const int ALL_NOTES_OFF{ 123 };
				static const int OMNI_MODE_OFF{ 124 };
				static const int OMNI_MODE_ON{ 125 };
				static const int MONO_MODE{ 126 };
				static const int POLY_MODE{ 127 };
				static const int SWITCHES{ 301 };
				static const int PITCH_BEND_PSEUDO{ -224 };
				static const int POLY_PRESSURE_PSEUDO{ -160 };
				static const int CHANNEL_PRESSURE_PSEUDO{ -208 };
				static const int PROGRAM_PSEUDO{ -192 };

			private:
				static std::vector<int> PERFORMANCE_CONTROLLERS_;
				static std::vector<int> SOUND_CONTROLLERS_;
				static std::vector<int> MIXER_CONTROLLERS_;
				static std::vector<int> EFFECTS_CONTROLLERS_;
				static std::vector<int> UNDEFINED_CONTROLLERS_;
				static std::vector<int> NO_CONTROLLERS_;
				static std::vector<int> SWITCH_CONTROLLERS_;
				static std::vector<std::string> CATEGORIES_;

			public:
				static std::vector<std::string> getCategories();
				static std::vector<int> getControllers(std::string category);
				static std::string propertyName(int controller);
				static bool is7bit(int controller);
				static int getMinimum(int controller);
				static int getMaximum(int controller);
				static int getDefault(int controller);
				static int getOffset(int controller);
				static int getOrientation(int controller);

				Controller();

			public:
				static std::vector<int>& PERFORMANCE_CONTROLLERS();
				static std::vector<int>& SOUND_CONTROLLERS();
				static std::vector<int>& MIXER_CONTROLLERS();
				static std::vector<int>& EFFECTS_CONTROLLERS();
				static std::vector<int>& UNDEFINED_CONTROLLERS();
				static std::vector<int>& NO_CONTROLLERS();
				static std::vector<int>& SWITCH_CONTROLLERS();

			private:
				static std::vector<std::string>& CATEGORIES();
			};

		}
	}
}
