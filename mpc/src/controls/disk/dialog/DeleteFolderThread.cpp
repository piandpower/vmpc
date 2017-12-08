#include <controls/disk/dialog/DeleteFolderThread.hpp>
#include <controls/disk/dialog/DeleteFolderControls.hpp>
//#include <disk/Disk.hpp>
#include <ui/disk/window/DirectoryGui.hpp>

using namespace mpc::controls::disk::dialog;

DeleteFolderThread::DeleteFolderThread(DeleteFolderControls *dfc)
	: dfc(dfc)
{
}

void DeleteFolderThread::run()
{
/*
	if (dfc->disk->deleteDir(dfc->directoryGui)->getSelectedFile()) {
		dfc->disk->flush();
		dfc->disk->initFiles();
	}
	Thread::sleep(1000);
	dfc->mainFrame->removePopup();
	dfc->mainFrame->openScreen("directory");
	*/
}