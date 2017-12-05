#include <lcdgui/VerticalBar.hpp>

using namespace mpc::lcdgui;

VerticalBar::VerticalBar(MRECT rect) 
{
	this->rect = rect;
}

void VerticalBar::setValue(int value)
{
    this->value = value;
	SetDirty();
}

void VerticalBar::setColor(bool on)
{
    this->color = on;
	SetDirty();
}

void VerticalBar::Draw(std::vector<std::vector<bool> >* pixels)
{
/*
	if (value > 2) {
		int valuePixels = (int)((value - 2) / 1.5);
		int lcdx = mpc::maingui::Constants::LCD_RECT()->L;
		int lcdy = mpc::maingui::Constants::LCD_RECT()->T;
		IRECT tmp(0 + rect.L + lcdx, rect.T + lcdy + 64 - valuePixels, 6 + rect.L + lcdx, 64 + rect.T + lcdy);
		g->DrawRect(color ? Constants::LCD_ON() : Constants::LCD_OFF(), &tmp);
		g->FillIRect(color ? Constants::LCD_ON() : Constants::LCD_OFF(), &tmp);
	}
	*/
	dirty = false;
}

VerticalBar::~VerticalBar() {
}

