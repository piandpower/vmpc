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

SliderControl::SliderControl(IPlugBase* pPlug, IBitmap sliders, std::weak_ptr<mpc::hardware::Slider> slider, int startIndex)
	: IPanelControl(pPlug, *Constants::SLIDER_RECT(), Constants::LCD_OFF())
{
	sliderIndex = startIndex;
	this->sliders = sliders;
	this->slider = slider;
	sliderIndex = slider.lock()->getValue() / 1.27;
	clampIndex(sliderIndex);
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
	g->DrawBitmap(&sliders, GetRECT(), 0, sliderIndex * 247 * SCALE, &tmp);
	return true;
}

SliderControl::~SliderControl() {
}
