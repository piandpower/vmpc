#pragma once
#include "Component.hpp"

namespace mpc {
	namespace lcdgui {

		class MixerKnobBackground
			: public Component
		{

		private:
			MRECT rect;

		public:
			void Draw(std::vector<std::vector<bool> >* pixels) override;

		public:
			MixerKnobBackground(MRECT rect);
			~MixerKnobBackground();

		};

	}
}
