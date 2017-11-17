#pragma once
#include <control/ControlLaw.hpp>

namespace ctoot {
	namespace control {

		class AbstractLaw
			: public virtual ControlLaw
		{

		public:
			static const int resolution{ 1024 };
			float min{};
			float max{};
			std::string units{ "" };

		public:
			int getResolution() override;
			float getMinimum() override;
			float getMaximum() override;
			std::string getUnits() override;

		public:
			AbstractLaw(float min, float max, std::string units);

		};

	}
}
