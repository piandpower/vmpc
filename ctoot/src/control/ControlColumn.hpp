#pragma once

#include <control/CompoundControl.hpp>

namespace ctoot {
	namespace control {

		class ControlColumn
			: public CompoundControl
		{

		public:
			typedef CompoundControl super;

		public:
			//void add(shared_ptr<Control> c) override;
			bool isAlwaysVertical() override;
			bool isNeverBordered() override;

			ControlColumn();

		private:
			friend class CompoundControl;
			friend class ControlRow;
			friend class BypassControl;

		};
	}
}
