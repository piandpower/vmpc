#pragma once
#include "Component.hpp"

namespace mpc {
	namespace lcdgui {

		class Knob
			: public Component
		{

		private:
			MRECT rect;
			int value{};
			bool color{ true };

		public:
			void setValue(int value);
			void setColor(bool on);

		public:
			void Draw(std::vector<std::vector<bool> >* pixels) override;

		public:
			Knob(MRECT rect);
			~Knob();

		};

	}
}
