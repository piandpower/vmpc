#include <lcdgui/MixerStrip.hpp>

#include <Mpc.hpp>
#include <ui/Uis.hpp>
#include <lcdgui/LayeredScreen.hpp>
#include <lcdgui/Knob.hpp>
#include <lcdgui/Field.hpp>
#include <lcdgui/MixerFaderBackground.hpp>
#include <lcdgui/MixerKnobBackground.hpp>
#include <lcdgui/VerticalBar.hpp>
#include <ui/sampler/MixerGui.hpp>

using namespace std;
using namespace mpc::lcdgui;

MixerStrip::MixerStrip(int columnIndex, int bank, mpc::Mpc* mpc)
{
	this->columnIndex = columnIndex;
	abcd = vector<string>{ "A", "B", "C", "D" };
	letters = vector<string>{ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p" };
	xPos0indiv = vector<int>{ 5, 20, 35, 50, 65, 80, 95, 110, 125, 140, 155, 170, 185, 200, 215, 230 };
	xPos1indiv = vector<int>{ 12, 27, 42, 57, 72, 87, 102, 117, 132, 147, 162, 177, 192, 207, 222, 237 };
	yPos0indiv = 0;
	yPos1indiv = 4;
	xPos0fx = vector<int>{ 5, 20, 35, 50, 65, 80, 95, 110, 125, 140, 155, 170, 185, 200, 215, 230 };
	xPos1fx = vector<int>{ 11, 26, 41, 56, 71, 86, 101, 116, 131, 146, 161, 176, 191, 206, 221, 236 };
	yPos0fx = 3;
	yPos1fx = 3;
	selection = -1;
	mixGui = mpc->getUis().lock()->getMixerGui();
	auto lLs = mpc->getLayeredScreen().lock();
	verticalBar = lLs->getVerticalBarsMixer()[columnIndex];
	verticalBar.lock()->Hide(false);
	knob = lLs->getKnobs()[columnIndex];
	mixerKnobBackground = lLs->getMixerKnobBackgrounds()[columnIndex];
	mixerFaderBackground = lLs->getMixerFaderBackgrounds()[columnIndex];
	mixerStrip.push_back(verticalBar);
	mixerStrip.push_back(knob);
	mixerStrip.push_back(mixerKnobBackground);
	mixerStrip.push_back(mixerFaderBackground);
	tf0 = lLs->lookupField(letters[columnIndex] + "0");
	tf1 = lLs->lookupField(letters[columnIndex] + "1");
	tf2 = lLs->lookupField(letters[columnIndex] + "2");
	tf3 = lLs->lookupField(letters[columnIndex] + "3");
	tf4 = lLs->lookupField(letters[columnIndex] + "4");
	tf1.lock()->setOpaque(false);
	tf2.lock()->setOpaque(false);
	jta = { tf0, tf1, tf2, tf3, tf4 };
	tf2.lock()->setText(abcd[bank]);
	initFields();
	setColors();
}

vector<weak_ptr<Component>> MixerStrip::getMixerStrip()
{
    return mixerStrip;
}

void MixerStrip::setValueA(int i)
{
    knob.lock()->setValue(i);
}

void MixerStrip::setValueB(int i)
{
    verticalBar.lock()->setValue(i);
}

void MixerStrip::initFields()
{
	if (mixGui->getTab() == 0) {
		knob.lock()->Hide(false);
		tf0.lock()->Hide(false);
		tf0.lock()->setText("");
		tf0.lock()->setOpaque(false);
		tf1.lock()->Hide(true);
		tf2.lock()->Hide(false);
		tf3.lock()->Hide(false);
		tf4.lock()->Hide(false);
	}
	else if (mixGui->getTab() == 1) {
		knob.lock()->Hide(true);
		tf0.lock()->Hide(false);
		tf1.lock()->Hide(false);
		tf0.lock()->setLocation(xPos0indiv[columnIndex] - 1, yPos0indiv);
		tf1.lock()->setLocation(xPos1indiv[columnIndex] - 1, yPos1indiv);
	}
	else if (mixGui->getTab() == 2) {
		knob.lock()->Hide(true);
		tf0.lock()->Hide(false);
		tf1.lock()->Hide(false);
		tf0.lock()->setLocation(xPos0fx[columnIndex], yPos0fx);
		tf1.lock()->setLocation(xPos1fx[columnIndex], yPos1fx);
	}
}

void MixerStrip::setColors()
{
	if (selection == -1) {
		for (auto& tf : jta) {
			tf.lock()->setInverted(false);
		}
		mixerKnobBackground.lock()->Hide(true);
		mixerFaderBackground.lock()->Hide(true);
		knob.lock()->setColor(true);
		verticalBar.lock()->Hide(false);
		verticalBar.lock()->setColor(true);
	}
	else if (selection == 0) {
		for (auto& tf : jta) {
			tf.lock()->setInverted(false);
		}
		jta[0].lock()->setInverted(true);
		jta[1].lock()->setInverted(true);
		jta[2].lock()->setInverted(false);
		jta[3].lock()->setInverted(false);
		jta[4].lock()->setInverted(false);
		mixerKnobBackground.lock()->Hide(false);
		mixerFaderBackground.lock()->Hide(true);
		knob.lock()->setColor(false);
		verticalBar.lock()->Hide(false);
		verticalBar.lock()->setColor(true);
	}
    else if(selection == 1) {
		for (auto& tf : jta) {
			tf.lock()->setInverted(false);
		}
        jta[0].lock()->setInverted(false);
        jta[1].lock()->setInverted(false);
        jta[2].lock()->setInverted(true);
        jta[3].lock()->setInverted(true);
        jta[4].lock()->setInverted(true);
        mixerKnobBackground.lock()->Hide(true);
        mixerFaderBackground.lock()->Hide(false);
        knob.lock()->setColor(true);
		verticalBar.lock()->Hide(false);
		verticalBar.lock()->setColor(false);
    }
}

void MixerStrip::setSelection(int i)
{
    selection = i;
    setColors();
}

void MixerStrip::setValueAString(string str)
{
	if (mixGui->getTab() == 1) {
		if (str.length() == 1) {
			jta[0].lock()->setText(str);
			jta[0].lock()->setLocation(xPos0indiv[columnIndex] + 2, yPos0indiv + 2);
			jta[1].lock()->setText("");
		}
		else if (str.length() == 2) {
			jta[0].lock()->setLocation(xPos0indiv[columnIndex], yPos0indiv);
			jta[0].lock()->setText(str.substr(0, 1));
			jta[1].lock()->setText(str.substr(1, 2));
		}
	}
	else if (mixGui->getTab() == 2) {
		jta[0].lock()->setText(str.substr(0, 1));
		jta[1].lock()->setText(str.substr(1, 2));
	}
}

MixerStrip::~MixerStrip() {
	verticalBar.lock()->Hide(true);
	mixerKnobBackground.lock()->Hide(true);
	mixerFaderBackground.lock()->Hide(true);
	knob.lock()->Hide(true);
}
