#include "HorizontalBar.hpp"

#include <cmath>

using namespace mpc::lcdgui;

HorizontalBar::HorizontalBar(MRECT rect, int value)
{
	this->rect = rect;
	this->value = value;
}

void HorizontalBar::setValue(int value)
{
    this->value = value;
	SetDirty();
}

void HorizontalBar::Draw(std::vector<std::vector<bool> >* pixels) {
	if (IsHidden()) return;
	//if (value > 2) {
		int x = rect.L;
		int y = rect.T;
		int x2 = x + ((int)(floor((value - 1) / 2.55))) + 1;
		MRECT r(x, y, x2, y + 6);
		for (int i = rect.L; i < rect.R; i++) {
			for (int j = y; j < y + 6; j++) {
				pixels->at(i).at(j) = false;
			}
		}
		for (int i = x; i < x2; i++) {
			for (int j = y; j < y + 6; j++) {
				pixels->at(i).at(j) = true;
			}
		}
	//}
	dirty = false;
}

HorizontalBar::~HorizontalBar() {
}
