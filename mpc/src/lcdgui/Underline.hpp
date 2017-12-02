#pragma once
#include "Component.hpp"
//#include <IControl.h>

#include <vector>

namespace mpc {
	namespace lcdgui {
		class Underline
			: public Component
			//: public IPanelControl
		{

		private:
			std::vector<bool> states{};

		public:
			//	bool Draw(IGraphics* g1) override;
			void setState(int i, bool b);

			Underline();

		};

	}
}
