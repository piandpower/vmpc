#include <maingui/StartUp.hpp>
#include <chrono>
#include <thread>

#include <Mpc.hpp>
//#include <lcdgui/LayeredScreen.hpp>
//#include <hardware/LedPanel.hpp>
//#include <nvram/NvRam.hpp>

#include <sys/Home.hpp>
#include <file/FileUtil.hpp>

using namespace mpc::maingui;
using namespace moduru::file;
using namespace std;

StartUp::StartUp()
{
}

string StartUp::home = moduru::sys::Home::get();
string StartUp::resPath = home + FileUtil::getSeparator() + "vMPC" + FileUtil::getSeparator() + "resources" + FileUtil::getSeparator();
string StartUp::tempPath = home + FileUtil::getSeparator() + "vMPC" + FileUtil::getSeparator() + "temp" + FileUtil::getSeparator();
string StartUp::storesPath = home + FileUtil::getSeparator() + "vMPC" + FileUtil::getSeparator() + "Stores" + FileUtil::getSeparator();

weak_ptr<mpc::ui::UserDefaults> StartUp::getUserDefaults() {
	return userDefaults;
}
//shared_ptr<mpc::ui::UserDefaults> StartUp::userDefaults = shared_ptr<mpc::ui::UserDefaults>(mpc::nvram::NvRam::load());
shared_ptr<mpc::ui::UserDefaults> StartUp::userDefaults = make_shared<mpc::ui::UserDefaults>();

void StartUp::initUserDefaults() {
	userDefaults = make_shared<mpc::ui::UserDefaults>();
}

/*
void StartUp::runBootSequence(weak_ptr<Gui> gui) {
	if (this->gui.lock()) return;
	this->gui = gui;
	auto lGui = gui.lock();
	//lGui->init();
	mainFrame = lGui->getMainFrame();
//	auto lMainFrame = mainFrame.lock();
//	lMainFrame->addLcd();
	//if (startupThread.joinable()) startupThread.join();
	startupThread = spawn();
}
*/

void StartUp::startUp()
{
/*
	auto lMainFrame = mainFrame.lock();
	auto ledPanel = lMainFrame->getLedPanel().lock();
	ledPanel->setPadBankA(true);
	ledPanel->setPadBankB(true);
	ledPanel->setPadBankC(true);
	ledPanel->setPadBankD(true);
	ledPanel->setAfter(true);
	ledPanel->setUndoSeq(true);
	ledPanel->setRec(true);
	ledPanel->setOverDub(true);
	ledPanel->setPlay(true);
	ledPanel->setFullLevel(true);
	ledPanel->setSixteenLevels(true);
	ledPanel->setNextSeq(true);
	ledPanel->setTrackMute(true);

	std::this_thread::sleep_for(chrono::milliseconds((int)(500 * mpc::maingui::Constants::TFACTOR)));
	ls.lock()->openScreen("black");
	std::this_thread::sleep_for(chrono::milliseconds((int)(90 * mpc::maingui::Constants::TFACTOR)));
	ls.lock()->openScreen("mpc2000xl");
	std::this_thread::sleep_for(chrono::milliseconds((int)(100 * mpc::maingui::Constants::TFACTOR)));
	ls.lock()->openScreen("sequencer");

	lMainFrame->createPopup("Wait.......", 190);
	ledPanel->setPadBankB(false);
	ledPanel->setPadBankC(false);
	ledPanel->setPadBankD(false);
	ledPanel->setAfter(false);
	ledPanel->setUndoSeq(false);
	ledPanel->setRec(false);
	ledPanel->setOverDub(false);
	ledPanel->setPlay(false);
	ledPanel->setFullLevel(false);
	ledPanel->setSixteenLevels(false);
	ledPanel->setNextSeq(false);
	ledPanel->setTrackMute(false);
	std::this_thread::sleep_for(chrono::milliseconds((int)(200 * mpc::maingui::Constants::TFACTOR)));
	lMainFrame->createPopup("detecting memory...", 88);
	std::this_thread::sleep_for(chrono::milliseconds((int)(200 * mpc::maingui::Constants::TFACTOR)));
	lMainFrame->createPopup("Wait.......", 190);
	std::this_thread::sleep_for(chrono::milliseconds((int)(200 * mpc::maingui::Constants::TFACTOR)));
	lMainFrame->removePopup();
	gui.lock()->getMpc()->initDisks();
	ls.lock()->openScreen("load");
	std::this_thread::sleep_for(chrono::milliseconds((int)(200 * mpc::maingui::Constants::TFACTOR)));
	ls.lock()->openScreen("sequencer");
	*/
}

thread StartUp::spawn() {
	return thread(&StartUp::startUp, this);
}

StartUp::~StartUp() {
	try {
		if (startupThread.joinable()) startupThread.join();
	}
	catch (exception* e) {
	}
}
