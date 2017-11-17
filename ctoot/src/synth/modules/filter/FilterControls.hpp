#pragma once

#include <control/CompoundControl.hpp>
#include <synth/modules/filter/FilterVariables.hpp>

#include <string>

namespace ctoot {

	namespace control {
		class ControlLaw;
		class FloatControl;
	}

	namespace synth {
		namespace modules {
			namespace filter {

				class FilterControls
					: public ctoot::control::CompoundControl
					, public virtual FilterVariables
				{

				private:
					static ctoot::control::ControlLaw* SEMITONE_LAW_;
					ctoot::control::FloatControl* cutoffControl{ nullptr };
					ctoot::control::FloatControl* resonanceControl{ nullptr };
					float cutoff{ 0.f }, resonance{ 0.f };

				protected:
					int idOffset{ 0 };

				private:
					int sampleRate{ 44100 };

				public:
					void derive(ctoot::control::Control* c) override;
					virtual void createControls();
					virtual void deriveSampleRateIndependentVariables();
					virtual float deriveResonance();
					virtual void deriveSampleRateDependentVariables();
					virtual float deriveCutoff();
					virtual ctoot::control::FloatControl* createCutoffControl();
					virtual ctoot::control::FloatControl* createResonanceControl();

				public:
					float getCutoff() override;
					float getResonance() override;
					void setSampleRate(int rate) override;

					FilterControls(int id, int instanceIndex, std::string name, int idOffset);
					virtual ~FilterControls();

				};

			}
		}
	}
}
