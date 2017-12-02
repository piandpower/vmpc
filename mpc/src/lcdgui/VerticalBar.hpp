#pragma once
#include "Component.hpp"

namespace mpc {
	namespace lcdgui {

		class VerticalBar
			: public Component
		{

		private:
			int value{ 100 };
			bool color = true;

		public:
			virtual void setValue(int value);
			virtual void setColor(bool on);
			//bool Draw(IGraphics* g1) override;

			VerticalBar();

		};

	}
}
