#pragma once

#include <control/Control.hpp>

namespace ctoot {
	namespace control {
		class BooleanControl
			: public Control
		{

		public:
			typedef Control super;

		private:
			std::string trueStateLabel{ "" };
			std::string falseStateLabel{ "" };
			bool momentary{ false };
			bool value{ false };

		public:
			virtual bool isMomentary();
			virtual void setValue(bool value);
			virtual void momentaryAction();
			virtual bool getValue();
			virtual std::string getStateLabel(bool state);
			std::string toString();
			std::string getValueString() override;
			void setIntValue(int value) override;
			int getIntValue() override;
			virtual int getWidthLimit();
			BooleanControl(int id, std::string name, bool initialValue, std::string trueStateLabel, std::string falseStateLabel);
			BooleanControl(int id, std::string name, bool initialValue);
			BooleanControl(int id, std::string name, bool initialValue, bool momentary);
			
		};

	}
}
