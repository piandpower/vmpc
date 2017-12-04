#include "Label.hpp"

using namespace mpc::lcdgui;
using namespace std;

Label::Label()
	: TextComp()
{
}

void Label::initialize(string name, string text, int x, int y, int columns) {
	noLeftMargin = false;
	opaque = true;
	inverted = false;
	Hide(false);
	this->text = text;
	this->name = name;
	this->columns = columns;
	setText(text);
	setSize(columns * TEXT_WIDTH + 1, TEXT_HEIGHT + 2);
	setLocation(x, y);
}

Label::~Label() {
}
