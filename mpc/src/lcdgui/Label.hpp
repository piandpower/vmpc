#pragma once
#include <string>

#include "TextComp.hpp"

namespace mpc {
	namespace lcdgui {

		class Label
			: public TextComp

		{
		public:
			void initialize(std::string name, std::string text, int x, int y, int columns);

		public:
			Label();
			~Label() override;

		};

	}
}
