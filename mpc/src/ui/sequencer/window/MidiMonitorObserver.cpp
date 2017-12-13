#include <ui/sequencer/window/MidiMonitorObserver.hpp>

#include <Mpc.hpp>
#include <audiomidi/EventHandler.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <lcdgui/Label.hpp>

using namespace mpc::ui::sequencer::window;
using namespace std;

MidiMonitorObserver::MidiMonitorObserver(mpc::Mpc* mpc) 
{
	this->mpc = mpc;
	auto lEventHandler = mpc->getEventHandler().lock();
	lEventHandler->addObserver(this);
	auto ls = mpc->getLayeredScreen().lock();
	a0 = ls->lookupLabel("0");
	a1 = ls->lookupLabel("1");
	a2 = ls->lookupLabel("2");
	a3 = ls->lookupLabel("3");
	a4 = ls->lookupLabel("4");
	a5 = ls->lookupLabel("5");
	a6 = ls->lookupLabel("6");
	a7 = ls->lookupLabel("7");
	a8 = ls->lookupLabel("8");
	a9 = ls->lookupLabel("9");
	a10 = ls->lookupLabel("10");
	a11 = ls->lookupLabel("11");
	a12 = ls->lookupLabel("12");
	a13 = ls->lookupLabel("13");
	a14 = ls->lookupLabel("14");
	a15 = ls->lookupLabel("15");
	b0 = ls->lookupLabel("16");
	b1 = ls->lookupLabel("17");
	b2 = ls->lookupLabel("18");
	b3 = ls->lookupLabel("19");
	b4 = ls->lookupLabel("20");
	b5 = ls->lookupLabel("21");
	b6 = ls->lookupLabel("22");
	b7 = ls->lookupLabel("23");
	b8 = ls->lookupLabel("24");
	b9 = ls->lookupLabel("25");
	b10 = ls->lookupLabel("26");
	b11 = ls->lookupLabel("27");
	b12 = ls->lookupLabel("28");
	b13 = ls->lookupLabel("29");
	b14 = ls->lookupLabel("30");
	b15 = ls->lookupLabel("31");
}

void MidiMonitorObserver::initTimer(std::weak_ptr<mpc::lcdgui::Label> label)
{
 //   ::java::awt::event::ActionListener* action = new MidiMonitorObserver_initTimer_1(this, label);
    //blinkTimer = new ::javax::swing::Timer(50, action);
//    npc(blinkTimer)->setRepeats(false);
//    npc(blinkTimer)->start();
}

void MidiMonitorObserver::update(moduru::observer::Observable* o, boost::any arg)
{
	string s = boost::any_cast<string>(arg);
	if (s.compare("a0") == 0) {
		a0.lock()->setText(u8"\u00CC");
		initTimer(a0);
	}
	else if (s.compare("a1") == 0) {
		a1.lock()->setText(u8"\u00CC");
		initTimer(a1);
	}
	else if (s.compare("a2") == 0) {
		a2.lock()->setText(u8"\u00CC");
		initTimer(a2);
	}
	else if (s.compare("a3") == 0) {
		a3.lock()->setText(u8"\u00CC");
		initTimer(a3);
	}
	else if (s.compare("a4") == 0) {
		a4.lock()->setText(u8"\u00CC");
		initTimer(a4);
	}
	else if (s.compare("a5") == 0) {
		a5.lock()->setText(u8"\u00CC");
		initTimer(a5);
	}
	else if (s.compare("a6") == 0) {
		a6.lock()->setText(u8"\u00CC");
		initTimer(a6);
	}
	else if (s.compare("a7") == 0) {
		a7.lock()->setText(u8"\u00CC");
		initTimer(a7);
	}
	else if (s.compare("a8") == 0) {
		a8.lock()->setText(u8"\u00CC");
		initTimer(a8);
	}
	else if (s.compare("a9") == 0) {
		a9.lock()->setText(u8"\u00CC");
		initTimer(a9);
	}
	else if (s.compare("a10") == 0) {
		a10.lock()->setText(u8"\u00CC");
		initTimer(a10);
	}
	else if (s.compare("a11") == 0) {
		a11.lock()->setText(u8"\u00CC");
		initTimer(a11);
	}
	else if (s.compare("a12") == 0) {
		a12.lock()->setText(u8"\u00CC");
		initTimer(a12);
	}
	else if (s.compare("a13") == 0) {
		a13.lock()->setText(u8"\u00CC");
		initTimer(a13);
	}
	else if (s.compare("a14") == 0) {
		a14.lock()->setText(u8"\u00CC");
		initTimer(a14);
	}
	else if (s.compare("a15") == 0) {
		a15.lock()->setText(u8"\u00CC");
		initTimer(a15);
	}
	else if (s.compare("b0") == 0) {
		b0.lock()->setText(u8"\u00CC");
		initTimer(b0);
	}
	else if (s.compare("b1") == 0) {
		b1.lock()->setText(u8"\u00CC");
		initTimer(b1);
	}
	else if (s.compare("b2") == 0) {
		b2.lock()->setText(u8"\u00CC");
		initTimer(b2);
	}
	else if (s.compare("b3") == 0) {
		b3.lock()->setText(u8"\u00CC");
		initTimer(b3);
	}
	else if (s.compare("b4") == 0) {
		b4.lock()->setText(u8"\u00CC");
		initTimer(b4);
	}
	else if (s.compare("b5") == 0) {
		b5.lock()->setText(u8"\u00CC");
		initTimer(b5);
	}
	else if (s.compare("b6") == 0) {
		b6.lock()->setText(u8"\u00CC");
		initTimer(b6);
	}
	else if (s.compare("b7") == 0) {
		b7.lock()->setText(u8"\u00CC");
		initTimer(b7);
	}
	else if (s.compare("b8") == 0) {
		b8.lock()->setText(u8"\u00CC");
		initTimer(b8);
	}
	else if (s.compare("b9") == 0) {
		b9.lock()->setText(u8"\u00CC");
		initTimer(b9);
	}
	else if (s.compare("b10") == 0) {
		b10.lock()->setText(u8"\u00CC");
		initTimer(b10);
	}
	else if (s.compare("b11") == 0) {
		b11.lock()->setText(u8"\u00CC");
		initTimer(b11);
	}
	else if (s.compare("b12") == 0) {
		b12.lock()->setText(u8"\u00CC");
		initTimer(b12);
	}
	else if (s.compare("b13") == 0) {
		b13.lock()->setText(u8"\u00CC");
		initTimer(b13);
	}
	else if (s.compare("b14") == 0) {
		b14.lock()->setText(u8"\u00CC");
		initTimer(b14);
	}
	else if (s.compare("b15") == 0) {
		b15.lock()->setText(u8"\u00CC");
		initTimer(b15);
	}
}

MidiMonitorObserver::~MidiMonitorObserver() {
	mpc->getEventHandler().lock()->deleteObserver(this);
}
