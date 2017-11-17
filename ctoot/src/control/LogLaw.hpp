#pragma once
#include <control/AbstractLaw.hpp>

namespace ctoot {
	namespace control {

		class LogLaw
			: public AbstractLaw
		{

		public:
			typedef AbstractLaw super;

		private:
			double logMin{}, logMax{};
			double logSpan{};

		public:
			int intValue(float userVal) override;
			float userValue(int intVal) override;

			LogLaw(float min, float max, std::string units);

		};

	}
}
