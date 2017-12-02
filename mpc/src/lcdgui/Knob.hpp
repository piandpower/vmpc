#pragma once
#include "Component.hpp"

namespace mpc {
	namespace lcdgui {

		class Knob
			: public Component
		{

		private:
			int value{};
			bool color{ true };

		public:
			void setValue(int value);
			void setColor(bool on);
			//bool Draw(IGraphics* g1) override;

			Knob();

		};

	}
}
