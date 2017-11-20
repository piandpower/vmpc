#include "TextComp.hpp"
#include <lang/StrUtil.hpp>

#include <Logger.hpp>

#include <lang/utf8_decode.h>

using namespace mpc::lcdgui;

using namespace moduru::lang;

using namespace std;

#include <gui/BMFParser.hpp>

moduru::gui::BMFParser* TextComp::bmfParser = new moduru::gui::BMFParser();

TextComp::TextComp()
//	: IPanelControl(pPlug, IRECT(0,0,0,0), Constants::TRANS_BLACK())
{
	//Hide(true);
	//textControl = make_unique<ITextControl>(pPlug, IRECT(0,0,0,0), Constants::FONT_ON(), text.c_str());
	//textOffsetX = 2;
}

void TextComp::Draw(std::vector<std::vector<bool> >* pixels) {
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
	while (next != UTF8_END && next >= 0) {
		moduru::gui::bmfont_char current_char;
		current_char = font.chars[next];
		atlasx = current_char.x;
		atlasy = current_char.y;

		for (int j = 0; j < TEXT_WIDTH - 1; j++) {
			for (int k = 0; k < TEXT_HEIGHT; k++) {
				(*pixels)[textx + j + (charCounter*TEXT_WIDTH)][texty + 1 + k] = false;
			}
		}

		for (int x1 = 0; x1 < current_char.width; x1++) {
			for (int y1 = 0; y1 < current_char.height; y1++) {
				bool on = atlas[atlasx + x1][atlasy + y1 + 1];
				if (on) {
					(*pixels)[textx + x1 + current_char.xoffset][texty + y1 + current_char.yoffset] = true;
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

void TextComp::setSize(int w, int h) {
	columns = w / TEXT_WIDTH;
	this->w = w;
	this->h = h;
	initRECT();
}

void TextComp::setLocation(int x, int y) {
	this->x = x;
	this->y = y;
	initRECT();
}

void TextComp::initRECT() {
	//int lcdX = maingui::Constants::LCD_RECT()->L;
	//int lcdY = maingui::Constants::LCD_RECT()->T;
	//mRECT = IRECT(x + lcdX, y + lcdY, x + w + lcdX, y + h  + lcdY);
	//textControl->SetRECT(IRECT(x + lcdX + textOffsetX, y + lcdY, x + lcdX + textOffsetX + w, y + lcdY + h));
	//SetDirty(false);
}

int TextComp::getX() {
	return x;
}

int TextComp::getY() {
	return y;
}

int TextComp::getW() {
	return w;
}

int TextComp::getH() {
	return h;
}

void TextComp::setForeground(bool on) {
	//mColor = on ? *Constants::LCD_OFF() : *Constants::LCD_ON();
	//f (textControl) textControl->SetText(on ? Constants::FONT_ON() : Constants::FONT_OFF());
	//SetDirty(true);
}

bool TextComp::getForeground() {
	//if (!textControl) return true;
	//return mColor == *Constants::LCD_OFF();
	return true;
}

void TextComp::setOpaque(bool enabled) {
	opaque = enabled;
	//SetDirty(false);
}

string TextComp::getName() {
	return name;
}

string TextComp::getText() {
	return text;
}

int TextComp::getColumns() {
	return columns;
}

void TextComp::setColumns(int i) {
	columns = i;
}

unsigned int TextComp::GetTextEntryLength() {
	return 0;
}

void TextComp::setText(string s)
{
	text = s;
	//bool wasScrolling = scrolling;
	//if (scrolling) setScrolling(false);
	//auto typeName = typeid(*this).name();
	//auto typeStr = string(typeName);
	//text = s;
	//int len = 0;
	//auto s1 = text.c_str();
	//while (*s1) len += (*s1++ & 0xc0) != 0x80;

	//if (len > getColumns()) s = StrUtil::subStr(s, 0, getColumns());
	////if (textControl) {
	////	textControl->SetTextFromPlug(&s[0]);
	////}
	//if (wasScrolling) setScrolling(true);
	SetDirty();
}

void TextComp::setTextPadded(string s, string padding) {
	string padded = StrUtil::padLeft(s, padding, columns);
	setText(padded);
}

void TextComp::setTextPadded(int i, string padding) {
	setTextPadded(to_string(i), padding);
}

void TextComp::setScrolling(bool scrolling) {
	this->scrolling = scrolling;
	if (scrolling) {
		if (text.length() <= columns) {
			scrolling = false;
			return;
		}
		scrollThread = thread(&TextComp::static_scroll, this);
	}
	else {
		if (scrollThread.joinable()) scrollThread.join();
	}
}

void TextComp::scroll() {
	auto origText = this->text;
	int len = 0;
	auto s = origText.c_str();
	while (*s) len += (*s++ & 0xc0) != 0x80;
	int clmns = getColumns();
	bool left = false;
	int offset = 0;
	while (scrolling) {
		if (offset + clmns == len || offset == 0) {
			left = !left;
			const unsigned int repeats = 100;
			unsigned int counter = 0;
			while (scrolling && counter++ < repeats)
				this_thread::sleep_for(chrono::milliseconds(5));
		}
		offset = left ? offset + 1 : offset - 1;
		auto tmp = StrUtil::subStr(origText, offset, offset + clmns);
		//textControl->SetTextFromPlug(&tmp[0]);
		//SetDirty(true);
		const unsigned int repeats = 100;
		unsigned int counter = 0;
		while (scrolling && counter++ < repeats)
			this_thread::sleep_for(chrono::milliseconds(3));
	}
}

void TextComp::static_scroll(void * args) {
	static_cast<TextComp*>(args)->scroll();
}

TextComp::~TextComp() {
	if (scrollThread.joinable()) scrollThread.join();
}
