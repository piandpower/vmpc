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

SliderControl::SliderControl(IPlugBase* pPlug, IBitmap sliders, std::weak_ptr<mpc::hardware::Slider> slider, int startIndex, InputCatcherControl* ipc)
	: IPanelControl(pPlug, *Constants::SLIDER_RECT(), Constants::LCD_OFF())
{
	this->sliders = sliders;
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
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	IRECT cropRect(0, sliderIndex * 247, sliders.W, (sliderIndex * 247) + 247);
	auto bm = g->CropBitmap(&sliders, &cropRect);
	auto bm1 = g->ScaleBitmap(&bm, floor(sliders.W * gui_scale), floor(247 * gui_scale));
	g->DrawBitmap(&bm1, GetRECT(), 0, 0, &tmp);
	g->ReleaseBitmap(&bm);
	g->ReleaseBitmap(&bm1);
	return true;
}

SliderControl::~SliderControl() {
}
