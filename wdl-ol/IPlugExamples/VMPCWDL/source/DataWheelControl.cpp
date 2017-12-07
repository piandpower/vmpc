#include "DataWheelControl.hpp"

#include <hardware/DataWheel.hpp>

#include "Constants.hpp"

#include "../resource.h"

DataWheelControl::DataWheelControl(IPlugBase* pPlug, IBitmap dataWheels, std::weak_ptr<mpc::hardware::DataWheel> dataWheel)
	: IPanelControl(pPlug, *Constants::DATAWHEEL_RECT(), Constants::LCD_OFF())
{
	this->dataWheels = dataWheels;
	this->dataWheel = dataWheel;
}

static inline void clampIndex(int& dataWheelIndex) {
	if (dataWheelIndex < 0) {
		while (dataWheelIndex < 0)
			dataWheelIndex += 100;
	}
	else if (dataWheelIndex > 99) {
		while (dataWheelIndex > 99)
			dataWheelIndex -= 100;
	}
}

void DataWheelControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
	dataWheel.lock()->turn(dY);
}

bool DataWheelControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	g->DrawBitmap(&dataWheels, GetRECT(), 0, dataWheelIndex * GetRECT()->H(), &tmp);
	return true;
}

void DataWheelControl::update(moduru::observer::Observable* o, boost::any arg) {
	int increment = boost::any_cast<int>(arg);
	dataWheelIndex += increment;
	clampIndex(dataWheelIndex);
	SetDirty();
}

DataWheelControl::~DataWheelControl() {
}
