#pragma once
#include "Component.hpp"
#include <string>

namespace mpc {
	namespace lcdgui {

		class Background
			: public Component
		{

		private:
			std::string name{ "" };

		public:
			void setName(std::string name);
			void Draw(std::vector< std::vector<bool> >* pixels) override;

			Background();
			~Background();

		};

	}

}
