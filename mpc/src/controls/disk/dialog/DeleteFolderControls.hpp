#pragma once
#include <controls/disk/AbstractDiskControls.hpp>

namespace mpc {
	namespace controls {
		namespace disk {
			namespace dialog {

				class DeleteFolderControls
					: public mpc::controls::disk::AbstractDiskControls
				{

				public:
					typedef mpc::controls::disk::AbstractDiskControls super;
					void function(int i) override;

					DeleteFolderControls(mpc::Mpc* mpc);

				private:
					friend class DeleteFolderThread;

				};

			}
		}
	}
}
