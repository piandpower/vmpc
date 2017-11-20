#include "LCDControl.hpp"

#include <lcdgui/LayeredScreen.hpp>
#include "Constants.hpp"
//#include <mpc/gui/ControlPanel.hpp>

#include <Logger.hpp>

#include  "../resource.h"

LCDControl::LCDControl(IPlugBase* pPlug, std::weak_ptr<mpc::lcdgui::LayeredScreen> ls)
	: IPanelControl(pPlug, *Constants::LCD_RECT(), Constants::LCD_OFF())
{
	this->ls = ls;
	//knobIndex = startIndex;
	//knobType = type;
	//this->knobs = knobs;
}

bool LCDControl::IsDirty() {
	if (ls.lock()) return ls.lock()->IsDirty();
	return false;
}

bool LCDControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	//g->DrawBitmap(&knobs, GetRECT(), 0, knobIndex * GetRECT()->H(), &tmp);
	ls.lock()->Draw();
	auto pixels = ls.lock()->getPixels();
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
