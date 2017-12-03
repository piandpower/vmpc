#pragma once
#include <controls/AbstractControls.hpp>

namespace mpc {
	namespace controls {
		namespace other {

			class OthersControls
				: public mpc::controls::AbstractControls
			{

			public:
				typedef mpc::controls::AbstractControls super;
				void function(int i) override;
				void turnWheel(int increment) override;

				OthersControls(mpc::Mpc* mpc) ;
			};

		}
	}
}
