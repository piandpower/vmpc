#include "SelectedEventBar.hpp"

using namespace mpc::lcdgui;

SelectedEventBar::SelectedEventBar(MRECT rect)
{
	this->rect = rect;
}

void SelectedEventBar::Draw(std::vector<std::vector<bool> >* pixels)
{
//	int x = Constants::LCD_RECT()->L + rect.L;
//	int y = Constants::LCD_RECT()->T + rect.T;
//	IRECT r(x, y, x + 386, y + 17);
//    g->DrawRect(Constants::LCD_ON(), &r);
//	g->FillIRect(Constants::LCD_ON(), &r);
	dirty = false;
}

SelectedEventBar::~SelectedEventBar() {
}
