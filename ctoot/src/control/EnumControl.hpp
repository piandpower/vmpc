#pragma once
#include <control/Control.hpp>
#include <cstdint>
#include <string>

#include <boost/any.hpp>

namespace ctoot {
	namespace control {
		class EnumControl
			: public Control
		{
		private:
			boost::any value;

		public:
			virtual void setValue(boost::any value);
			virtual boost::any getValue();
			virtual std::vector<boost::any> getValues() = 0;
			virtual bool isValueSupported(boost::any value);

		public:
			std::string toString();
			std::string getValueString() override;
			void setIntValue(int value) override;
			int getIntValue() override;
			virtual int getWidthLimit();
			virtual bool hasLabel();

			EnumControl(int id, std::string name, boost::any value);

		};
	};
};