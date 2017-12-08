#include <controls/disk/dialog/DeleteFolderControls.hpp>

#include <controls/disk/dialog/DeleteFolderThread.hpp>
#include <disk/MpcFile.hpp>
#include <ui/disk/window/DirectoryGui.hpp>

using namespace mpc::controls::disk::dialog;
using namespace std;

DeleteFolderControls::DeleteFolderControls(mpc::Mpc* mpc) 
	: AbstractDiskControls(mpc)
{
}

void DeleteFolderControls::function(int i)
{
	super::function(i);
	switch (i) {
	case 4:
		ls.lock()->createPopup("Delete:" + directoryGui->getSelectedFile()->getName(), 85);
		//invokeLater(DeleteFolderThread(this));
		break;
	}
}
