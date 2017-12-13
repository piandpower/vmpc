#pragma once
#include <string>

#include "TextComp.hpp"

namespace mpc {
	namespace lcdgui {

		class Label
			: public TextComp

		{
		private:
			bool rigorousClearing = false;

		public:
			virtual void initialize(std::string name, std::string text, int x, int y, int columns);
			
		public:
			void enableRigorousClearing();

		public:
			void Draw(std::vector<std::vector<bool> >* pixels) override;

		public:
			Label();
			~Label() override;

		};

	}
}
