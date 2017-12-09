#pragma once
#include <IControl.h>

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
	IBitmap sliders{};
	std::weak_ptr<mpc::hardware::Slider> slider;
	int sliderIndex{ 0 };

public:
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;

	/*
	* Implement Observer
	*/
	public:
		//void update(moduru::observer::Observable* o, boost::any arg) override;

public:
	SliderControl(IPlugBase* pPlug, IBitmap sliders, std::weak_ptr<mpc::hardware::Slider> slider, int startIndex);
	~SliderControl();

};
