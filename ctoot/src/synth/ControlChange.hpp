#pragma once
#include <cstdint>

namespace ctoot {
	namespace synth {

		class ControlChange
		{

		private:
			int controller{};
			int value{};

		public:
			virtual int getController();
			virtual int getValue();

			ControlChange(int controller, int value);
		};
	};
};