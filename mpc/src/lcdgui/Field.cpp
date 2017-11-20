#include "Field.hpp"
#include "Label.hpp"
//#include <maingui/Constants.hpp>
//#include <maingui/StartUp.hpp>
//////#include <maingui/Gui.hpp>
#include "LayeredScreen.hpp"
//#include <lcdgui/LayeredScreen.hpp>
//#include <ui/NameGui.hpp>
//#include <lcdgui/Field_Blinker.hpp>
//#include <lcdgui/Field_Scroller.hpp>
//#include <lcdgui/TwoDots.hpp>
//#include <ui/sequencer/TrMoveGui.hpp>

#include <lang/StrUtil.hpp>
#include <lang/utf8_decode.h>
#include <gui/BMFParser.hpp>
//#include <Util.hpp>

#include <file/File.hpp>

using namespace mpc::lcdgui;
using namespace std;

Field::Field(LayeredScreen* layeredScreen)
	: TextComp() {
	this->layeredScreen = layeredScreen;
}

void Field::initialize(std::string name, int x, int y, int columns) {
	//ls = mainFrame->getLayeredScreen();
	split = false;
	focusable = true;
	opaque = false;
	//Hide(false);
	this->name = name;
	this->columns = columns;
	setForeground(false);
	
	setSize(columns * TEXT_WIDTH + 1, TEXT_HEIGHT + 2);
	setLocation(x, y);
	setText(text);
	loseFocus(name);
}

void Field::Draw(std::vector<std::vector<bool> >* pixels) {
	auto atlas = bmfParser->getAtlas();
	auto font = bmfParser->getLoadedFont();

	int textx = x;
	int texty = y;
	int atlasx, atlasy;

	char* tempText = new char[text.length() + 1];
	std::strcpy(tempText, text.c_str());
	utf8_decode_init(tempText, text.length());

	int next = utf8_decode_next();
	int charCounter = 0;

	for (int j = 0; j < (TEXT_WIDTH * columns) + 1; j++) {
		for (int k = 0; k < TEXT_HEIGHT + 2; k++) {
			int x1 = textx + j - 1;
			int y1 = texty + k;
			if (x1 < 0 || x1 > 247 || y1 < 0 || y1 > 59) continue;
			(*pixels)[textx + j - 1][texty + k] = focus ? true : false;
		}
	}

	while (next != UTF8_END && next >= 0) {
		moduru::gui::bmfont_char current_char;
		current_char = font.chars[next];
		atlasx = current_char.x;
		atlasy = current_char.y;

		for (int x1 = 0; x1 < current_char.width; x1++) {
			for (int y1 = 0; y1 < current_char.height; y1++) {
				bool on = atlas[atlasx + x1][atlasy + y1 + 1];
				if (on) {
					(*pixels)[textx + x1 + current_char.xoffset][texty + y1 + current_char.yoffset] = focus ? false : true;
				}
			}
		}
		textx += current_char.xadvance;
		next = utf8_decode_next();
	}
	delete tempText;

	//if (name.compare("dummy") == 0) return;
	//bool res = true;
	//if (opaque) {
	//res = IPanelControl::Draw(g);
	//textControl->Draw(g);
	//}
	//else {
	//res = textControl->Draw(g);
	//}
	dirty = false;
}

void Field::setSize(int width, int height) {
	TextComp::setSize(width, height);
	setText(text);
}

void Field::setText(string s)
{
	/*
	if (split) {
		for (int i = 0; i < letters.size(); i++) {
			letters[i]->setText(string(text.substr(i, i + 1)).c_str());
		}
	}
	*/
	TextComp::setText(s);
}

const int Field::BLINKING_RATE;

void Field::takeFocus(string prev)
{
	focus = true;
	//auto lGui = gui.lock();
	//auto lMainFrame = lGui->getMainFrame().lock();
	//auto lMainFrame = mainFrame;
	//auto focusEvent = lMainFrame->getFocus();
	//auto focusField = lls->lookupField(focusEvent);
	//auto lLs = ls.lock();
//	csn = layeredScreen->getCurrentScreenName();
//	if (csn.compare("trim") == 0 || csn.compare("loop") == 0) {
//		if (focusEvent.compare("st") == 0 || focusEvent.compare("to") == 0) {
//			layeredScreen->getTwoDots()->setSelected(0, true);
//		}
//		else if (focusEvent.compare("end") == 0 || focusEvent.compare("endlengthvalue") == 0) {
//			layeredScreen->getTwoDots()->setSelected(1, true);
//		}
//	}
//	if (csn.compare("startfine") == 0 || csn.compare("endfine") == 0 || csn.compare("looptofine") == 0 || csn.compare("loopendfine") == 0) {
//		if (focusEvent.compare("start") == 0) {
//			layeredScreen->getTwoDots()->setSelected(2, true);
//		}
//		else if (focusEvent.compare("end") == 0) {
//			lLs->getTwoDots()->setSelected(2, true);
//		}
//		else if (focusEvent.compare("to") == 0) {
//			lLs->getTwoDots()->setSelected(2, true);
//		}
//		else if (focusEvent.compare("lngth") == 0) {
//			lLs->getTwoDots()->setSelected(3, true);
//		}
//	}
//	if (csn.compare("directory") == 0) {
//		focusField.lock()->setOpaque(true);
//	}
//	if ((!csn.compare("name") == 0)) {
//		focusField.lock()->setOpaque(true);
//	}
//	if (csn.compare("tempochange") == 0) {
//		focusField.lock()->setOpaque(true);
//	}
//	//if (!(csn.compare("name") == 0 && lGui->getNameGui()->isNameBeingEdited())) {	
//		focusField.lock()->setForeground(false);
//	//}
//	if (csn.compare("trmove") == 0) {
//		//if (focusEvent.compare("tr1") == 0 && !lGui->getTrMoveGui()->isSelected())
//		//	lLs->drawFunctionKeyses("trmove_notselected");
//	}
//	if (csn.compare("name") == 0) {
//		//if (!lGui->getNameGui()->isNameBeingEdited()) {
////			focusField.lock()->setOpaque(true);
//		//}
//	}
	SetDirty();
}

void Field::loseFocus(string next)
{
	opaque = false;
	focus = false;
	auto focusEvent = getName();
	setSplit(false);
	//auto lLs = ls.lock();
	//csn = lLs->getCurrentScreenName();
	//if (csn.compare("trim") == 0 || csn.compare("loop") == 0) {
	//	if (focusEvent.compare("st") == 0 || focusEvent.compare("to") == 0) {
	//		lLs->getTwoDots()->setSelected(0, false);
	//	}
	//	else if (focusEvent.compare("end") == 0 || focusEvent.compare("endlengthvalue") == 0) {
	//		lLs->getTwoDots()->setSelected(1, false);
	//	}
	//}
	//else if (csn.compare("startfine") == 0 || csn.compare("endfine") == 0 || csn.compare("looptofine") == 0 || csn.compare("loopendfine") == 0) {
	//	if (focusEvent.compare("start") == 0) {
	//		lLs->getTwoDots()->setSelected(2, false);
	//	}
	//	else if (focusEvent.compare("end") == 0) {
	//		lLs->getTwoDots()->setSelected(2, false);
	//	}
	//	else if (focusEvent.compare("to") == 0) {
	//		lLs->getTwoDots()->setSelected(2, false);
	//	}
	//	else if (focusEvent.compare("lngth") == 0) {
	//		lLs->getTwoDots()->setSelected(3, false);
	//	}
	//}
	//
	////if ((!lGui->getNameGui()->isNameBeingEdited() && csn.compare("name") == 0))
	//	setOpaque(false);

	//if (csn.compare("tempochange") == 0) {
	//	setOpaque(false);
	//	setForeground(false);
	//}
	////if (!(csn.compare("name") == 0 && lGui->getNameGui()->isNameBeingEdited())) {
	//	setForeground(!getForeground());
	////}
	//if (csn.compare("trmove") == 0) {
	////	if (focusEvent.compare("tr1") == 0 && !lGui->getTrMoveGui()->isSelected())
	////		lLs->drawFunctionKeyses("trmove");
	//}
	//else if (csn.compare("directory") == 0 || csn.compare("save") == 0) {
	//	setOpaque(false);
	//}
	//else if (csn.compare("assignmentview") == 0) {
	//	setOpaque(false);
	//}
	SetDirty();
}


void Field::setSplit(bool b)
{
	if (split == b) return;
	split = b;
	if (split) {
		this->setOpaque(false);
		if (letters.size() != 0) {
			for (auto& l : letters)
				delete l;
		}
		letters.clear();
		letters = vector<Label*>(getText().length());
		activeSplit = letters.size() - 1;
		//auto x = getLocation())->x;
		//auto y = npc(this->getLocation())->y;

		for (int i = 0; i < letters.size(); i++) {
			//letters[i] = new Label(GetGUI()->GetPlug());
			//letters[i]->setFocusable(false);
			//GetGUI()->AttachControl(letters[i]);
		}
		setText(this->getText());
		redrawSplit();
		//SetDirty(true);
	}
	else {
		if (letters.size() == 0)
			return;

		this->setOpaque(true);
		//for (int i = 0; i < letters.size(); i++)
			//GetGUI()->DetachControl(letters[i]);
		activeSplit = 0;
		letters.clear();
		//SetDirty(true);
	}
}

void Field::redrawSplit()
{
	for (int i = 0; i < letters.size(); i++) {
		letters[i]->setForeground(i < activeSplit);
		//letters[i]->setBackground(i < activeSplit ? mpc::maingui::Constants::LCD_ON() : mpc::maingui::Constants::LCD_OFF());
		//letters[i]->setOpaque(i < activeSplit);
	}
	//SetDirty(true);
}

bool Field::isSplit()
{
    return split;
}

int Field::getActiveSplit()
{
    return activeSplit;
}

bool Field::setActiveSplit(int i)
{
	if (i < 1 || i + 1 > letters.size()) return false;
	activeSplit = i;
	redrawSplit();
	return true;
}

bool Field::enableTypeMode()
{
    if(typeModeEnabled)
        return false;

    typeModeEnabled = true;
	//oldText = this->getText();
    //setFont(mpc2000fontunderline);
    //setFontColor(gui::Constants::LCD_ON);
	//setTransparency(true);
    setText("");
    return true;
}

int Field::enter()
{
	auto value = INT_MAX;
    if(!typeModeEnabled)
        return value;

    //setFont(mpc2000font);
    //setFontColor(Constants::LCD_OFF);
	//setTransparency(false);
    typeModeEnabled = false;
    try {
	//	string valueString = getText();
	//	value = stoi(valueString);
	}
	catch (std::invalid_argument& e) {
		printf("Field.enter ERROR: %s", e.what());
        return value;
    }
    //setText(oldText.c_str());
    return value;
}

void Field::type(int i)
{
	/*
	string str = getText();
	remove_if(str.begin(), str.end(), isspace);
    if(str.length() == getColumns())
        str = "";
	string newStr = str.append(to_string(i));
	pad_left(newStr, getColumns());
    setText(newStr.c_str());
	*/
}

bool Field::isTypeModeEnabled()
{
    return typeModeEnabled;
}

void Field::disableTypeMode()
{
    if(!typeModeEnabled)
        return;

    typeModeEnabled = false;
    //setFont(Constants::mpc2000xlFont);
    //setFontColor(Constants::LCD_OFF);
	//setTransparency(false);
    //setText(oldText.c_str());
}

/*
void Field::enableScrolling(vector<CLabel> enablers)
{
    if(scroller != nullptr)
        scroller->stopped = true;

	// check if sizeof gets the right amount
    if(getColumns() > sizeof(getText()))
        return;

    //scroller = new Field_Scroller(this, this, enablers);
    //(new ::java::lang::Thread(static_cast< ::java::lang::Runnable* >(scroller)))->start();
    scrolling = true;
}
*/

void Field::startBlinking()
{
    blinking = true;
    //(new ::java::lang::Thread(static_cast< ::java::lang::Runnable* >(new Field_Blinker(this))))->start();
}

void Field::stopBlinking()
{
    blinking = false;
}

bool Field::getBlinking()
{
    return this->blinking;
}

void Field::setFocusable(bool b) {
	focusable = b;
}

bool Field::isFocusable() {
	return focusable;
}

bool Field::hasFocus() {
	return focus;
}

Field::~Field()
{
	for (auto& l : letters) {
		delete l;
	}
}
