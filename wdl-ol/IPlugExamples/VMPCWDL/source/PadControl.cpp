#include "PadControl.hpp"
#include <hardware/HwPad.hpp>

#include "Constants.hpp"
//#include <mpc/gui/ControlPanel.hpp>

#include <math.h>

#include  "../resource.h"

PadControl::PadControl(IPlugBase* pPlug, IRECT rect, std::weak_ptr<mpc::hardware::HwPad> pad)
	: IPanelControl(pPlug, rect, Constants::LCD_OFF())
{
	this->pad = pad;
}

int PadControl::getVelo(int x, int y) {
	float centX = mRECT.MW();
	float centY = mRECT.MH();
	float distX = x - centX;
	float distY = y - centY;
	float powX = pow(distX, 2);
	float powY = pow(distY, 2);
	float dist = sqrt(powX + powY);
	if (dist > 48) dist = 48;
	int velo = 127 - (dist * (127.0 / 48.0));
	return velo;
}

void PadControl::OnMouseDown(int x, int y, IMouseMod* pMod) {
	pad.lock()->push(getVelo(x, y));
}

void PadControl::OnMouseDblClick(int x, int y, IMouseMod* pMod) {
	pad.lock()->push(getVelo(x, y));
}

void PadControl::OnMouseUp(int x, int y, IMouseMod* pMod) {
	pad.lock()->release();
}

bool PadControl::Draw(IGraphics* g) {
	//return IPanelControl::Draw(g);
	return true;
}

PadControl::~PadControl() {
}
