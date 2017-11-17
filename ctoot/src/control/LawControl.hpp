#pragma once

#include <control/Control.hpp>
#include <control/ControlLaw.hpp>

namespace ctoot {
	namespace control {

		class LawControl
			: public Control
		{

		public:
			typedef Control super;

		private:
			ControlLaw* law{ nullptr };
			float value{ 0 };
			float precision{ 0 };

		public:
			virtual int calculateDecimalPlaces();

		public:
			virtual ControlLaw* getLaw();
			virtual float getValue();
			std::string getValueString() override;
			virtual void setValue(float value);
			virtual float getPrecision();
			void setIntValue(int value) override;
			int getIntValue() override;
			virtual std::vector<std::string>* getPresetNames();
			virtual void applyPreset(std::string name);

		public:
			LawControl(int id, std::string name, ControlLaw law, float precision, float initialValue);
			virtual ~LawControl();

		};

	}
}
