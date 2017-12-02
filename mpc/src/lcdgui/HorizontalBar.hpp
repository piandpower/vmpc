#pragma once
#include "Component.hpp"

namespace mpc {
	namespace lcdgui {

		class HorizontalBar
			: public Component
		{

		private:
			int value;

		public:
			void setValue(int value);
			//bool Draw(IGraphics* mGraphics) override;

			HorizontalBar(int value);

		};

	}
}
