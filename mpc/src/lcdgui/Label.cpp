#include "Label.hpp"

using namespace mpc::lcdgui;
using namespace std;

Label::Label()
	: TextComp()
{
}

void Label::initialize(string name, string text, int x, int y, int columns) {
	opaque = false;
	//Hide(false);
	this->text = text;
	this->name = name;
	this->columns = columns;
	setForeground(true);
	setText(text);
	setSize(columns * TEXT_WIDTH + 2, TEXT_HEIGHT + 4);
	setLocation(x - (textOffsetX/2), y);
}

Label::~Label() {
}
