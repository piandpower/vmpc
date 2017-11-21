#include "DataWheelControl.hpp"

#include <controls/KbMouseController.hpp>

#include "Constants.hpp"
//#include <mpc/gui/Gui.hpp>

#include "../resource.h"

//using namespace mpc::gui;

DataWheelControl::DataWheelControl(IPlugBase* pPlug, IBitmap dataWheels, mpc::controls::KbMouseController* kbmc)
	: IPanelControl(pPlug, *Constants::DATAWHEEL_RECT(), Constants::LCD_OFF())
{
	this->dataWheels = dataWheels;
	this->kbmc = kbmc;
}

void DataWheelControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
	kbmc->turnDataWheel(dY);
	SetDirty(false);
}

bool DataWheelControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	g->DrawBitmap(&dataWheels, GetRECT(), 0, kbmc->getDataWheelIndex() * GetRECT()->H(), &tmp);
	lastDrawnIndex = kbmc->getDataWheelIndex();
	return true;
}

bool DataWheelControl::IsDirty() {
	return kbmc->getDataWheelIndex() != lastDrawnIndex;
}

DataWheelControl::~DataWheelControl() {
	//GetGUI()->ReleaseBitmap(&dataWheels);
}
