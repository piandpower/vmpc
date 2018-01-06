#include "DataWheelControl.hpp"

#include <hardware/DataWheel.hpp>

#include "Constants.hpp"

#include "../resource.h"

DataWheelControl::DataWheelControl(IPlugBase* pPlug, IBitmap* dataWheels, std::weak_ptr<mpc::hardware::DataWheel> dataWheel, InputCatcherControl* ipc)
	: IBitmapControl(pPlug, Constants::DATAWHEEL_RECT()->L, Constants::DATAWHEEL_RECT()->T, dataWheels)
{
	this->dataWheel = dataWheel;
	this->ipc = ipc;
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

bool DataWheelControl::OnKeyDown(int x, int y, int c) {
	return ipc->OnKeyDown(x, y, c);
}


bool DataWheelControl::OnKeyUp(int x, int y, int c) {
	return ipc->OnKeyUp(x, y, c);
}

void DataWheelControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
	dataWheel.lock()->turn(dY);
}

bool DataWheelControl::Draw(IGraphics* g) {
	IRECT r(mDrawRECT.L, mDrawRECT.T, mDrawRECT.R, mDrawRECT.B - 1);
	return g->DrawBitmap(mBitmap, &r, dataWheelIndex, &mBlend);
}

void DataWheelControl::update(moduru::observer::Observable* o, boost::any arg) {
	int increment = boost::any_cast<int>(arg);
	dataWheelIndex += increment;
	clampIndex(dataWheelIndex);
	SetDirty();
}

DataWheelControl::~DataWheelControl() {
}
