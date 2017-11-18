#include "LCDControl.hpp"

//#include <mpc/gui/Gui.hpp>
#include "Constants.hpp"
//#include <mpc/gui/ControlPanel.hpp>

#include  "../resource.h"

LCDControl::LCDControl(IPlugBase* pPlug, std::vector<std::vector<bool>>* pixels)
	: IPanelControl(pPlug, *Constants::LCD_RECT(), Constants::LCD_OFF())
{
	this->pixels = pixels;
	//knobIndex = startIndex;
	//knobType = type;
	//this->knobs = knobs;
}

bool LCDControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	//g->DrawBitmap(&knobs, GetRECT(), 0, knobIndex * GetRECT()->H(), &tmp);
	for (int x = 0; x < 248; x++) {
		for (int y = 0; y < 60; y++) {
			if (pixels->at(x).at(y) == true) {
				//g->DrawPoint(Constants::LCD_ON(), x + this->GetRECT()->L, y + this->GetRECT()->T, &tmp);
				IRECT r((x*2) + this->GetRECT()->L, (y*2) + this->GetRECT()->T, (x*2) + this->GetRECT()->L + 1, (y*2) + this->GetRECT()->T + 1);
				g->DrawRect(Constants::LCD_ON(), &r);
			}
		}
	}
	return true;
}

LCDControl::~LCDControl() {
	//GetGUI()->ReleaseBitmap(&knobs);
}
