#include "DataWheelControl.hpp"

//#include <mpc/controls/AbstractControls.hpp>

#include "Constants.hpp"
//#include <mpc/gui/Gui.hpp>

#include "../resource.h"

//using namespace mpc::gui;

DataWheelControl::DataWheelControl(IPlugBase* pPlug, IBitmap dataWheels)
	: IPanelControl(pPlug, *Constants::DATAWHEEL_RECT(), Constants::LCD_OFF())
{
	dataWheelIndex = 0;
	this->dataWheels = dataWheels;
}

void DataWheelControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
	turn(dY);
}

void DataWheelControl::turn(int increment) {
	dataWheelIndex += increment;
	if (dataWheelIndex < 0)
		dataWheelIndex = 99;

	if (dataWheelIndex > 99)
		dataWheelIndex = 0;
	//target->turnWheel(increment);
	SetDirty(true);
}

//void DataWheelControl::setTarget(mpc::controls::AbstractControls* target) {
//	this->target = target;
//}

bool DataWheelControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	g->DrawBitmap(&dataWheels, GetRECT(), 0, dataWheelIndex * GetRECT()->H(), &tmp);
	return true;
}

DataWheelControl::~DataWheelControl() {
	//GetGUI()->ReleaseBitmap(&dataWheels);
}
