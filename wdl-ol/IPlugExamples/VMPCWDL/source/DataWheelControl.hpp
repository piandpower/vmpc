#pragma once
#include <IControl.h>

	
class DataWheelControl
	: public IPanelControl
{

private:
	IBitmap dataWheels{};
	int dataWheelIndex{ 0 };
	//mpc::controls::AbstractControls* target{ nullptr };

public:

	static const int NOTCH_UP{ 1000000 };
	static const int NOTCH_DOWN{ -1000000 };

	//void setTarget(mpc::controls::AbstractControls* target);

	void turn(int increment);

	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;

public:
	DataWheelControl(IPlugBase* pPlug, IBitmap dataWheels);
	~DataWheelControl();

};
