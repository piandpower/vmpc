#pragma once
#include <control/ControlLaw.hpp>
#include <vector>

namespace ctoot {
	namespace audio {
		namespace fader {

			class FaderLaw
				: public virtual ctoot::control::ControlLaw
			{

			private:
				int resolution{ 1 };
				float halfdB{ 1 };
				float maxdB{ 1 };
				float attenuationCutoffFactor{ 1};
				std::vector<float> floatValues;
				int binarySearch(std::vector<float> buf, float key, int min, int max);

			public:
				int getResolution();
				float getMaximum();
				float getMinimum();
				std::string getUnits();
				int intValue(float v);
				float userValue(int v);

			private:
				std::vector<float>* createFloatValues();

			public:
				virtual float calculateFloatValue(int v);

				FaderLaw(int resolution, float halfdB, float maxdB, float attenuationCutoffFactor);
				FaderLaw(int resolution);

			public:
				/*
				static FaderLaw* SEMI_LOG();
				static FaderLaw* LOG();
				static FaderLaw* BROADCAST();
				static float* ATTENUATION_CUTOFF();
				static FaderLaw* defaultLaw();
				*/
			};

		}
	}
}
