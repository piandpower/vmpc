#pragma once
#include "Component.hpp"

#include <vector>

namespace mpc {
	namespace lcdgui {
		class EnvGraph
			: public Component
		{

		private:
			std::vector<std::vector<int>> coordinates{};

		public:
			virtual void setCoordinates(std::vector<std::vector<int>> ia);
			void Draw(std::vector<std::vector<bool> >* pixels) override;

		public:
			EnvGraph();
			EnvGraph(std::vector<std::vector<int>> coordinates);
			~EnvGraph();

		};

	}
}
