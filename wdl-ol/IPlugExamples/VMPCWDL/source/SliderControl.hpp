#pragma once
#include <IControl.h>

#include "InputCatcherControl.hpp"

#include <memory>

namespace mpc {
	namespace hardware {
		class Slider;
	}
}

class SliderControl
	: public IPanelControl
	
	{

private:
	InputCatcherControl* ipc;
	IBitmap sliders{};
	std::weak_ptr<mpc::hardware::Slider> slider;
	int sliderIndex{ 0 };

public:
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;
	bool OnKeyDown(int x, int y, int key) override;
	bool OnKeyUp(int x, int y, int key) override;

public:
	SliderControl(IPlugBase* pPlug, IBitmap sliders, std::weak_ptr<mpc::hardware::Slider> slider, int startIndex, InputCatcherControl* ipc);
	~SliderControl();

};
