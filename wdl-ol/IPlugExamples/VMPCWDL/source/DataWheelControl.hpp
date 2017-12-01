#pragma once
#include <IControl.h>

#include <memory>

#include <observer/Observer.hpp>

namespace mpc {
	namespace  hardware {
		class DataWheel;
	}
}

	
class DataWheelControl
	: public IPanelControl
	, public moduru::observer::Observer
{

private:
	IBitmap dataWheels{};
	std::weak_ptr<mpc::hardware::DataWheel> dataWheel;
	int dataWheelIndex = 0;

	/*
	* Implement IPanelControl
	*/
public:
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;

	/*
	* Implement Observer
	*/
public:
	void update(moduru::observer::Observable* o, boost::any arg) override;

public:
	DataWheelControl(IPlugBase* pPlug, IBitmap dataWheels, std::weak_ptr<mpc::hardware::DataWheel> dataWheel);
	~DataWheelControl();

};
