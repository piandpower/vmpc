#include <controls/disk/window/LoadApsFileControls.hpp>

#include <ui/disk/DiskGui.hpp>
#include <sampler/Sampler.hpp>

using namespace mpc::controls::disk::window;
using namespace std;

LoadApsFileControls::LoadApsFileControls(mpc::Mpc* mpc) 
	: AbstractDiskControls(mpc)
{
}

void LoadApsFileControls::function(int i)
{
	init();
	switch (i) {
	case 3:
		ls.lock()->openScreen("load");
		break;
	case 4:
		apsLoader = make_unique<mpc::file::aps::ApsLoader>(mpc, diskGui->getSelectedFile());
		break;
	}
}
