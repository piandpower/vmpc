#pragma once

#include <observer/Observable.hpp>

#include <string>

namespace mpc {

	namespace hardware {

		class Pot
			: public moduru::observer::Observable
		{

		private:
			std::string label{ "" };

		public:
			std::string getLabel();

		public:
			Pot(std::string label);
			~Pot();

		};

	}
}
