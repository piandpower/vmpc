#include "HorizontalBar.hpp"

#include <cmath>

using namespace mpc::lcdgui;

HorizontalBar::HorizontalBar(MRECT rect, int value)
{
	this->value = value;
}

void HorizontalBar::setValue(int value)
{
    this->value = value;
	SetDirty();
}

void HorizontalBar::Draw(std::vector<std::vector<bool> >* pixels) {
		/*
		if (value > 2) {
		int x = Constants::LCD_RECT()->L + rect.L;
		int y = Constants::LCD_RECT()->T + rect.T;
		int x2 = x + ((int) (floor((value - 2) / 2.55)) * 2) + 1;
		IRECT r(x, y, x2, y + 11);
		g->DrawRect(Constants::LCD_ON(), &r);
		g->FillIRect(Constants::LCD_ON(), &r);
	}
	*/
		dirty = false;
}

HorizontalBar::~HorizontalBar() {
}
