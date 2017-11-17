#pragma once

#include <synth/modules/filter/FilterControls.hpp>
#include <synth/modules/filter/StateVariableFilterVariables.hpp>

namespace ctoot {
	
	namespace control {
		class FloatControl;
		class BooleanControl;
	}

	namespace synth {
		namespace modules {
			namespace filter {

				class StateVariableFilterControls
					: public FilterControls
					, public virtual StateVariableFilterVariables
				{

				private:
					ctoot::control::FloatControl* modeMixControl{ nullptr };
					ctoot::control::BooleanControl* bandModeControl{ nullptr };
					float modeMix{ 0 };
					bool bandMode{ false };

				public:
					void derive(ctoot::control::Control* c) override;
					void createControls() override;
					void deriveSampleRateIndependentVariables() override;
					float deriveResonance() override;
					virtual float deriveModeMix();
					virtual bool deriveBandMode();
					virtual ctoot::control::FloatControl* createModeMixControl();
					virtual ctoot::control::BooleanControl* createBandModeControl();

				public:
					float getModeMix() override;
					bool isBandMode() override;

					StateVariableFilterControls(int instanceIndex, std::string name, int idOffset);
					~StateVariableFilterControls();

				public:
					virtual float getCutoff();
					virtual float getResonance();
					virtual void setSampleRate(int rate);

				};

			}
		}
	}
}
