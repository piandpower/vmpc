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
	LICE_MemBitmap lbm(496, 120);
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	ls.lock()->Draw();
	auto pColor = Constants::LCD_ON();
	auto lcdon = LICE_RGBA(pColor->R, pColor->G, pColor->B, pColor->A);
	pColor = Constants::LCD_OFF();
	auto lcdoff = LICE_RGBA(pColor->R, pColor->G, pColor->B, pColor->A);
	pColor = Constants::LCD_HALF_ON();
	auto lcdhalfon = LICE_RGBA(pColor->R, pColor->G, pColor->B, pColor->A);
	auto pixels = ls.lock()->getPixels();
	for (int x = 0; x < 248; x++) {
		for (int y = 0; y < 60; y++) {
			if (pixels->at(x).at(y) == true) {
				LICE_DrawRect(&lbm, x * 2, y * 2, 1, 1, lcdhalfon, 1.0f);
				LICE_PutPixel(&lbm, x * 2, y * 2, lcdon, 1.0f, LICE_BLIT_MODE_COPY);
			}
			else {
				LICE_DrawRect(&lbm, x*2, y*2, 1, 1, lcdoff, 1.0f);
			}
		}
	}
	LICE_ScaledBlit(&lbm, &lbm, 0, 0, GetDrawRECT()->W(), GetDrawRECT()->H(), 0, 0, 496, 120, 1.0f, LICE_BLIT_FILTER_BILINEAR | LICE_BLIT_MODE_COPY);
	IBitmap foo(&lbm);
	auto res = g->DrawBitmap(&foo, GetDrawRECT(), 1, &tmp);
	g->ReleaseBitmap(&foo);
	return res;
}

LCDControl::~LCDControl() {
}
