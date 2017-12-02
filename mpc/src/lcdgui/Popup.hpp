#pragma once
#include "Component.hpp"

#include <string>
#include <memory>

namespace mpc {
	namespace lcdgui {

		class Background;

		class Popup
			: public Component			
		{

		private:
			int pos{ 0 };
			//IRECT textRect;
			std::string text{ "" };
			std::unique_ptr<Background> bg;

		public:
			void setText(std::string text, int pos);
			//bool Draw(IGraphics* mGraphics) override;

			Popup();

		};

	}
}
