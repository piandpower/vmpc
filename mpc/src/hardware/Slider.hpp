#pragma once

#include <observer/Observable.hpp>

namespace mpc {

	namespace hardware {

		class Slider
			: public moduru::observer::Observable
		{

		private:

		public:
			Slider();
			~Slider();

		};

	}
}
