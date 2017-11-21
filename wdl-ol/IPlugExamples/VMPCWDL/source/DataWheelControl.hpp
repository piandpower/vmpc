#pragma once
#include <IControl.h>

namespace mpc {
	namespace controls {
		class KbMouseController;
	}
}

	
class DataWheelControl
	: public IPanelControl
{

private:
	IBitmap dataWheels{};
	mpc::controls::KbMouseController* kbmc{ nullptr };
	int lastDrawnIndex = -1;

public:
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;
	bool IsDirty() override;

public:
	DataWheelControl(IPlugBase* pPlug, IBitmap dataWheels, mpc::controls::KbMouseController* kbmc);
	~DataWheelControl();

};
