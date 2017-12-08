#pragma once

namespace mpc {
	namespace controls {
		namespace disk {
			namespace dialog {

				class DeleteFolderControls;

				class DeleteFolderThread {

				public:
					void run(); //override;

					DeleteFolderThread(DeleteFolderControls *dfc);
					DeleteFolderControls *dfc;

				private:
					friend class DeleteFolderControls;

				};

			}
		}
	}
}
