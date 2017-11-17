#pragma once
#include <control/AbstractLaw.hpp>
#include <string>

namespace ctoot {
	namespace control {


		class LinearLaw
			: public AbstractLaw
		{

		public:
			typedef AbstractLaw super;

		private:
			static LinearLaw* UNITY_;

		public:
			int intValue(float v) override;
			float userValue(int i) override;

			LinearLaw(float min, float max, std::string units);


		public:
			static LinearLaw*& UNITY();

		};
	}
}
