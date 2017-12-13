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
}

bool LCDControl::IsDirty() {
	if (ls.lock()) return ls.lock()->IsDirty();
	return false;
}

bool LCDControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	ls.lock()->Draw();
	auto pixels = ls.lock()->getPixels();
	for (int x = 0; x < 248; x++) {
		for (int y = 0; y < 60; y++) {
			if (pixels->at(x).at(y) == true) {
				IRECT r((x*2 * SCALE) + GetRECT()->L, (y*2 * SCALE) + GetRECT()->T, (x*2 * SCALE) + GetRECT()->L + (1* SCALE), (y*2 * SCALE) + GetRECT()->T + (1*SCALE));
				g->DrawRect(Constants::LCD_HALF_ON(), &r);
				g->DrawPoint(Constants::LCD_ON(), (x * 2 * SCALE) + GetRECT()->L, (y * 2 * SCALE) + GetRECT()->T);
			}
		}
	}
	return true;
}

LCDControl::~LCDControl() {
	//GetGUI()->ReleaseBitmap(&knobs);
}
