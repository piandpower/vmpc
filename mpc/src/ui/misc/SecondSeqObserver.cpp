#include <ui/misc/SecondSeqObserver.hpp>

#include <Mpc.hpp>
#include <Util.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/Label.hpp>
#include <ui/misc/SecondSeqGui.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Sequencer.hpp>

using namespace mpc::ui::misc;
using namespace std;

SecondSeqObserver::SecondSeqObserver(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	secondSeqGui = mpc->getUis().lock()->getSecondSeqGui();
	secondSeqGui->addObserver(this);
	auto ls = mpc->getLayeredScreen().lock();
	sqField = ls->lookupField("sq");
	sequenceNameLabel = ls->lookupLabel("sequencename");
	displaySq();
}

void SecondSeqObserver::displaySq()
{
	auto sqName = mpc->getSequencer().lock()->getSequence(secondSeqGui->getSq()).lock()->getName();
	sqField.lock()->setTextPadded(secondSeqGui->getSq() + 1, "0");
	sequenceNameLabel.lock()->setText("-" + sqName);
}

void SecondSeqObserver::update(moduru::observer::Observable* o, boost::any a)
{
	string param = boost::any_cast<string>(a);
	if (param.compare("sq") == 0) {
		displaySq();
	}

}

SecondSeqObserver::~SecondSeqObserver() {
	secondSeqGui->deleteObserver(this);
}
