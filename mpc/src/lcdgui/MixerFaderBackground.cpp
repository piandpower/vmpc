#include <lcdgui/MixerFaderBackground.hpp>

using namespace mpc::lcdgui;

MixerFaderBackground::MixerFaderBackground(MRECT rect) 
{
	this->rect = rect;
}

void MixerFaderBackground::Draw(std::vector<std::vector<bool> >* pixels) {
//{
//	int lcdx = mpc::maingui::Constants::LCD_RECT()->L;
//	int lcdy = mpc::maingui::Constants::LCD_RECT()->T;
//	IRECT tmp(0 + rect.L + lcdx, 0 + rect.T + lcdy, 28 + rect.L + lcdx, 69 + rect.T + lcdy);
//	g->DrawRect(mpc::maingui::Constants::LCD_ON(), &tmp);
//	g->FillIRect(mpc::maingui::Constants::LCD_ON(), &tmp);
	dirty = false;
}

MixerFaderBackground::~MixerFaderBackground() {
}
