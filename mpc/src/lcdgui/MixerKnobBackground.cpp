#include <lcdgui/MixerKnobBackground.hpp>

using namespace mpc::lcdgui;

MixerKnobBackground::MixerKnobBackground(MRECT rect) 
{
	this->rect = rect;
}


void MixerKnobBackground::Draw(std::vector<std::vector<bool> >* pixels)
{
//	int lcdx = mpc::maingui::Constants::LCD_RECT()->L;
//	int lcdy = mpc::maingui::Constants::LCD_RECT()->T;
//	IRECT tmp(0 + rect.L + lcdx, 0 + rect.T + lcdy, 28 + rect.L + lcdx, 28 + rect.T + lcdy);
//	g->DrawRect(mpc::maingui::Constants::LCD_ON(), &tmp);
//	g->FillIRect(mpc::maingui::Constants::LCD_ON(), &tmp);
	dirty = false;
}

MixerKnobBackground::~MixerKnobBackground() {
}
