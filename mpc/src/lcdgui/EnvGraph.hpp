#pragma once
#include "Component.hpp"
//#include <IControl.h>

#include <vector>

namespace mpc {
	namespace lcdgui {
		class EnvGraph
			: public Component
			//: public IPanelControl
		{

		private:
			std::vector<std::vector<int>> coordinates{};

		public:
			virtual void setCoordinates(std::vector<std::vector<int>> ia);
			//bool Draw(IGraphics* pGraphics) override;

			EnvGraph(std::vector<std::vector<int>> coordinates);

		};

	}
}
