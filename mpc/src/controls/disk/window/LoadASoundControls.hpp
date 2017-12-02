#pragma once
#include <controls/disk/AbstractDiskControls.hpp>

namespace mpc {
	namespace controls {
		namespace disk {
			namespace window {

				class LoadASoundControls
					: public mpc::controls::disk::AbstractDiskControls
				{

				public:
					typedef mpc::controls::disk::AbstractDiskControls super;
					virtual void pad(int i);
					void turnWheel(int i) override;
					void function(int i) override;

					LoadASoundControls(mpc::Mpc* mpc);


//				public:
//					void pad(int i, int velo, bool repeat, int tick);

				};

			}
		}
	}
}
