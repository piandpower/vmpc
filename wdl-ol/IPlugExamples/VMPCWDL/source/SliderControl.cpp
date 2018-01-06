#include "SliderControl.hpp"
#include <hardware/HwSlider.hpp>

#include "Constants.hpp"

#include <Logger.hpp>

#include  "../resource.h"

static inline void clampIndex(int& sliderIndex) {
	if (sliderIndex < 0) {
		sliderIndex = 0;
	}
	else if (sliderIndex > 99) {
		sliderIndex = 99;
	}
}

SliderControl::SliderControl(IPlugBase* pPlug, IBitmap* sliders, std::weak_ptr<mpc::hardware::Slider> slider, int startIndex, InputCatcherControl* ipc)
	: IBitmapControl(pPlug, Constants::SLIDER_RECT()->L, Constants::SLIDER_RECT()->T, sliders)
{
	this->slider = slider;
	this->ipc = ipc;
	sliderIndex = startIndex;
	sliderIndex = slider.lock()->getValue() / 1.27;
	clampIndex(sliderIndex);
}

bool SliderControl::OnKeyDown(int x, int y, int c) {
	return ipc->OnKeyDown(x, y, c);
}


bool SliderControl::OnKeyUp(int x, int y, int c) {
	return ipc->OnKeyUp(x, y, c);
}

void SliderControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
	std::string val = std::to_string(slider.lock()->getValue());
	slider.lock()->setValue(slider.lock()->getValue() + dY);
	sliderIndex = slider.lock()->getValue() / 1.27;
	clampIndex(sliderIndex);
	SetDirty(false);
}

bool SliderControl::Draw(IGraphics* g) {
	IRECT r(mDrawRECT.L, mDrawRECT.T, mDrawRECT.R, mDrawRECT.B - 1);
	return g->DrawBitmap(mBitmap, &r, sliderIndex, &mBlend);
}

SliderControl::~SliderControl() {
}
