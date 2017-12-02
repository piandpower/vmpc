#include "LoadASequenceFromAllObserver.hpp"

#include <Mpc.hpp>

#include <lcdgui/Field.hpp>
#include <ui/disk/DiskGui.hpp>
#include <ui/disk/window/DiskWindowGui.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Sequencer.hpp>
#include <lcdgui/Label.hpp>
#include <lcdgui/Field.hpp>

using namespace mpc::ui::disk;
using namespace std;

LoadASequenceFromAllObserver::LoadASequenceFromAllObserver(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	diskGui = mpc->getUis().lock()->getDiskGui();
	diskWindowGui = mpc->getUis().lock()->getDiskWindowGui();
	diskGui->deleteObservers();
	diskGui->addObserver(this);
	diskWindowGui->deleteObservers();
	diskWindowGui->addObserver(this);
	auto ls = mpc->getLayeredScreen().lock();
	fileField = ls->lookupField("file");
	fileLabel = ls->lookupLabel("file0");
	loadIntoField = ls->lookupField("loadinto");
	loadIntoLabel = ls->lookupLabel("loadinto0");
	displayFile();
	displayLoadInto();
}

void LoadASequenceFromAllObserver::displayFile()
{
	fileField.lock()->setTextPadded(diskGui->getFileLoad() + 1, "0");
	auto candidate = diskGui->getSequencesFromAllFile()->at(diskGui->getFileLoad());
	auto name = candidate ? candidate->getName() : "(Unused)";
	fileLabel.lock()->setText("-" + name);
}

void LoadASequenceFromAllObserver::displayLoadInto()
{
	loadIntoField.lock()->setTextPadded(diskWindowGui->getLoadInto() + 1, "0");
	loadIntoLabel.lock()->setText("-" + mpc->getSequencer().lock()->getSequence(diskWindowGui->getLoadInto()).lock()->getName());
}

void LoadASequenceFromAllObserver::update(moduru::observer::Observable* o, boost::any a)
{
	string param = boost::any_cast<string>(a);
	if (param.compare("fileload") == 0) {
		displayFile();
	}
	else if (param.compare("loadinto") == 0) {
		displayLoadInto();
	}
}

LoadASequenceFromAllObserver::~LoadASequenceFromAllObserver() {
}
