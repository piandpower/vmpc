#pragma once

#include <observer/Observable.hpp>

namespace mpc {
	class Mpc;
	namespace hardware {

		class Slider
			: public moduru::observer::Observable
		{

		private:
			mpc::Mpc* mpc;
			int value = 0;

		public:
			void setValue(int i);
			int getValue();

		public:
			Slider(mpc::Mpc* mpc);
			~Slider();

		};

	}
}
