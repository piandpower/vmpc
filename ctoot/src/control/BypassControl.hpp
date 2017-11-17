#pragma once

#include <control/BooleanControl.hpp>
#include <string>

namespace ctoot {
	namespace control {

		class BypassControl
			: public BooleanControl
		{

		public:
			typedef BooleanControl super;

		public:
			BypassControl(int id);

		private:
			friend class CompoundControl;
			friend class ControlColumn;
			friend class ControlRow;
		};
	}
}
