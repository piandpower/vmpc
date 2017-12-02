#pragma once

#include <memory>
#include <vector>

namespace mpc {
	class Mpc;
	namespace disk {
		class Stores;
		class AbstractDisk;
	}

	class DiskController
	{

	public:
		static const int MAX_DISKS{ 7 };

	private:
		mpc::Mpc* mpc;
		std::shared_ptr<mpc::disk::Stores> stores{};
		std::vector<std::shared_ptr<mpc::disk::AbstractDisk>> disks{};

	public:
		void initDisks();
		std::weak_ptr<mpc::disk::Stores> getStores();
		std::weak_ptr<mpc::disk::AbstractDisk> getDisk();
		std::weak_ptr<mpc::disk::AbstractDisk> getDisk(int i);

		DiskController(mpc::Mpc* mpc);
		~DiskController();

	};

}
