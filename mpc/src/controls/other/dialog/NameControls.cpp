#include <controls/other/dialog/NameControls.hpp>

#include <controls/KbMapping.hpp>
#include <Mpc.hpp>
#include <Util.hpp>
//#include <controls/other/dialog/NameControls_keyEvent_2.hpp>
//#include <controls/other/dialog/NameControls_drawUnderline_3.hpp>
//#include <controls/other/dialog/NameControls_keyEvent_1.hpp>
#include <disk/AbstractDisk.hpp>
#include <disk/MpcFile.hpp>
#include <file/aps/ApsSaver.hpp>
#include <ui/NameGui.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/Underline.hpp>
#include <ui/disk/DiskGui.hpp>
#include <ui/disk/window/DirectoryGui.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <ui/sampler/SoundGui.hpp>
//#include <ui/sampler/window/EditSoundGui.hpp>
//#include <ui/sampler/window/SamplerWindowGui.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <ui/vmpc/DirectToDiskRecorderGui.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/Sequencer.hpp>

#include <lang/StrUtil.hpp>

using namespace mpc::controls::other::dialog;
using namespace std;
using namespace moduru::lang;

NameControls::NameControls(mpc::Mpc* mpc) 
	: AbstractOtherControls(mpc)
{
}

void NameControls::left()
{
	init();
	if (stoi(param) == 0) return;
	super::left();
	if (nameGui->isNameBeingEdited()) {
		auto field = ls.lock()->lookupField(ls.lock()->getFocus()).lock();
		field->setInverted(false);
	}
}

void NameControls::right()
{
	init();
	if (stoi(param) == nameGui->getNameLimit() - 1) return;
	super::right();
	if (nameGui->isNameBeingEdited()) {
		auto field = ls.lock()->lookupField(ls.lock()->getFocus()).lock();
		field->setInverted(false);
	}
}

void NameControls::turnWheel(int j)
{
    init();
	
	if (nameGui->isNameBeingEdited()) {
		for (int i = 0; i < 16; i++) {
			if (param.compare(to_string(i)) == 0) {
				nameGui->changeNameCharacter(i, j > 0);
				break;
			}
		}
	}
	else {
		if (j > 0) {
			for (int i = 0; i < 16; i++) {
				if (param.compare(to_string(i)) == 0) {
					nameGui->changeNameCharacter(i, j > 0);
					nameGui->setNameBeingEdited(true);
					initEditColors();
					break;
				}
			}
		}
	}
}

void NameControls::function(int i)
{
    init();
    switch (i) {
    case 3:
        ls.lock()->openScreen(ls.lock()->getPreviousScreenName());
        resetNameGui();
        break;
    }

}

/*
void NameControls::keyEvent(unsigned char e)
{
	init();
	auto lGui = gui.lock();
	if (!(e == KbMapping::left() || e == KbMapping::right() || e == KbMapping::dataWheelBack())) {
		for (int i = 0; i < akaiAsciiChar.size(); i++) {
			if (e == akaiAsciiChar[i]) {
				auto offset = 1;
				if (!nameGui->isNameBeingEdited()) offset = 0;

				if (lGui->getPreviousKeyStroke() == KbMapping::left() || lGui->getPreviousKeyStroke() == KbMapping::right()) offset = 0;

				auto position = stoi(param) + offset;
				if (position > nameGui->getNameLimit() - 1) position = nameGui->getNameLimit() - 1;

				nameGui->setName(string{ akaiAsciiChar[i] }, position);
				if (!nameGui->isNameBeingEdited()) nameGui->setNameBeingEdited(true);

				initEditColors();
				auto lMainFrame = mainFrame.lock();
				lMainFrame->lookupField(to_string(position)).lock()->setText(string{ akaiAsciiChar[i] });
				lMainFrame->setFocus(to_string(position), 2);
			}
		}
	}
	if (e != KbMapping::numPadShift()) lGui->setPreviousKeyStroke(e);
	auto lMainFrame = mainFrame.lock();
	auto lLs = ls.lock();
	if (e == mpc::controls::KbMapping::f5() || e == mpc::controls::KbMapping::numPadEnter()) {
		if (nameGui->getParameterName().compare("outputfolder") == 0) {
			lGui->getD2DRecorderGui()->setOutputFolder(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("directtodiskrecorder");
		}
		else if (nameGui->getParameterName().compare("saveallfile") == 0) {
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("saveallfile");
			return;
		}
		else if (nameGui->getParameterName().compare("saveasound") == 0) {
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("saveasound");
			return;
		}
		else if (nameGui->getParameterName().compare("savingpgm") == 0) {
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("saveaprogram");
			return;
		}
		else if (nameGui->getParameterName().compare("savingaps") == 0) {
			string apsName = nameGui->getName();
			apsName.append(".APS");
			mpc::file::aps::ApsSaver(gui, mpc, mpc::Util::getFileName(apsName));
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			return;
		}
		else if (nameGui->getParameterName().compare("savingmid") == 0) {
			lMainFrame->openScreen("saveasequence");
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			return;
		}

		auto lSequencer = sequencer.lock();

		if (nameGui->getParameterName().find("default") != string::npos) {
			if (lLs->getPreviousScreenName().compare("track") == 0) {
				lSequencer->setDefaultTrackName(nameGui->getName(), lSequencer->getActiveTrackIndex());
				nameGui->setNameBeingEdited(false);
				lLs->setLastFocus("name", "0");
				lMainFrame->openScreen("sequencer");
				return;
			}
			else if (lLs->getPreviousScreenName().compare("sequence") == 0) {
				lSequencer->setDefaultSequenceName(nameGui->getName());
				nameGui->setNameBeingEdited(false);
				lLs->setLastFocus("name", "0");
				lMainFrame->openScreen("sequencer");
				return;
			}
		}

		if (lLs->getPreviousScreenName().compare("saveapsfile") == 0) {
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("saveapsfile");
		}
		else if (lLs->getPreviousScreenName().compare("keeporretry") == 0) {
			sampler.lock()->getPreviewSound().lock()->setName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("keeporretry");
		}
		else if (lLs->getPreviousScreenName().compare("track") == 0) {
			track.lock()->setName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("sequencer");
			return;
		}
		else if (lLs->getPreviousScreenName().compare("saveasequence") == 0) {
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("saveasequence");
		}
		else if (lLs->getPreviousScreenName().compare("sequence") == 0) {
			sequence.lock()->setName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("sequencer");
			return;
		}
		else if (lLs->getPreviousScreenName().compare("midioutput") == 0) {
			sequence.lock()->setDeviceName(swGui->getDeviceNumber() + 1, nameGui->getName().substr(0, 8));
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("sequencer");
			return;
		}
		else if (lLs->getPreviousScreenName().compare("editsound") == 0) {
			lGui->getEditSoundGui()->setNewName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("editsound");
			return;
		}
		else if (lLs->getPreviousScreenName().compare("sound") == 0) {
			mpc->getSampler().lock()->getSound(lGui->getSoundGui()->getSoundIndex()).lock()->setName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("sound");
			return;
		}
		else if (lLs->getPreviousScreenName().compare("resample") == 0) {
			lGui->getSoundGui()->setNewName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("resample");
			return;
		}
		else if (lLs->getPreviousScreenName().compare("stereotomono") == 0) {
			if (nameGui->getParameterName().compare("newlname") == 0) {
				lGui->getSoundGui()->setNewLName(nameGui->getName());
			}
			if (nameGui->getParameterName().compare("newrname") == 0) {
				lGui->getSoundGui()->setNewRName(nameGui->getName());
			}
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("stereotomono");
			return;
		}
		else if (lLs->getPreviousScreenName().compare("copysound") == 0) {
			lGui->getSoundGui()->setNewName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("copysound");
			return;
		}

		if (nameGui->getParameterName().compare("programname") == 0) {
			program.lock()->setName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("program");
			return;
		}
		else if (nameGui->getParameterName().compare("createnewprogram") == 0) {
			lGui->getSamplerWindowGui()->setNewName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("program");
			return;
		}
		else if (nameGui->getParameterName().compare("autochrom") == 0) {
			lGui->getSamplerWindowGui()->setNewName(nameGui->getName());
			nameGui->setNameBeingEdited(false);
			lLs->setLastFocus("name", "0");
			lMainFrame->openScreen("autochromaticassignment");
			lLs->setPreviousScreenName(lGui->getSamplerGui()->getPrevScreenName());
			return;
		}
		else if (nameGui->getParameterName().compare("rename") == 0) {
			bool success;
			auto ext = mpc::Util::splitName(directoryGui->getSelectedFile()->getName())[1];
			if (ext.length() > 0) ext = "." + ext;

			success = mpc->getDisk().lock()->renameSelectedFile(StrUtil::trim(StrUtil::toUpper(nameGui->getName())) + ext);
			if (!success) {
				lMainFrame->createPopup("File name exists !!", 120);
				//invokeLater(NameControls_keyEvent_1(this));
				lLs->setPreviousScreenName("directory");
				return;
			}
			else {
				mpc->getDisk().lock()->flush();
				mpc->getDisk().lock()->initFiles();
				nameGui->setNameBeingEdited(false);
				lLs->setLastFocus("name", "0");
				lMainFrame->openScreen("directory");
				return;
			}
		}
		if (nameGui->getParameterName().compare("newfolder") == 0) {
			auto lDisk = mpc->getDisk().lock();
			bool success = lDisk->newFolder(StrUtil::toUpper(nameGui->getName()));
			if (success) {
				lDisk->flush();
				lDisk->initFiles();
				auto counter = 0;
				for (int i = 0; i < lDisk->getFileNames().size(); i++) {
					if (lDisk->getFileName(i).compare(StrUtil::toUpper(nameGui->getName())) == 0) {
						lGui->getDiskGui()->setFileLoad(counter);
						if (counter > 4) {
							lGui->getDirectoryGui()->setYOffset1(counter - 4);
						}
						else {
							lGui->getDirectoryGui()->setYOffset1(0);
						}
						break;
					}
					counter++;
				}
				lMainFrame->openScreen("directory");
				lLs->setPreviousScreenName("load");
				nameGui->setNameBeingEdited(false);
			}

			if (!success) {
				lMainFrame->createPopup("Folder name exists !!", 120);
				//invokeLater(NameControls_keyEvent_2(this));
			}
			return;
		}
	}
	drawUnderline();
}
*/

void NameControls::drawUnderline()
{
	if (nameGui->isNameBeingEdited()) {
		string focus = ls.lock()->getFocus();
		//if (focus == null) return;
		if (focus.length() != 1 && focus.length() != 2) return;
		auto u = ls.lock()->getUnderline();
		for (int i = 0; i < 16; i++) {

			if (i == stoi(focus)) {
				u->setState(i, true);
			}
			else {
				u->setState(i, false);
			}
		}
	}
}

void NameControls::initEditColors()
{
    for (int i = 0; i < 16; i++) {
		auto field = ls.lock()->lookupField(to_string(i)).lock();
		field->setOpaque(true);
		field->setInverted(false);
    }
	//auto focus = ls.lock()->getFocus();
	//ls.lock()->lookupField(focus).lock()->setForeground(true);
	//ls.lock()->lookupField(focus).lock()->setInverted(false);
	//ls.lock()->lookupField(focus).lock()->setOpaque(true);
}

void NameControls::resetNameGui()
{
	nameGui->setNameBeingEdited(false);
	ls.lock()->setLastFocus("name", "0");
}
