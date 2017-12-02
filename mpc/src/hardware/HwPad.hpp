#pragma once

#include <observer/Observable.hpp>

namespace mpc {

	class Mpc;

	namespace controls {
		class AbstractControls;
	}

	namespace hardware {

		class HwPad
			: public moduru::observer::Observable
		{

		private:
			mpc::Mpc* mpc;
			int index;
			std::weak_ptr<mpc::controls::AbstractControls> controls;

		public:
			int getIndex();
			void push(int velo);

		public:
			HwPad(mpc::Mpc* mpc, int index);
			~HwPad();

		};

	}
}
