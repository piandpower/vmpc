#include <lcdgui/Underline.hpp>

using namespace mpc::lcdgui;
using namespace std;

Underline::Underline()
{
	states = vector<bool>(16);
}

/*
bool Underline::Draw(IGraphics* g1)
{
	int x = this->mRECT.L + Constants::LCD_RECT()->L;
	int y = this->mRECT.T + Constants::LCD_RECT()->T;
	for (int i = 0; i < 16; i++) {
		if (!states[i]) {
			continue;
		}
		IRECT rect(x + (i * 12), y + 0, x + (i * 12) + 12, y + 1);
		g1->DrawRect(Constants::LCD_ON(), &rect);
	}
	return true;
}
*/

void Underline::setState(int i, bool b)
{
    states[i] = b;
	//SetDirty(false);
}
