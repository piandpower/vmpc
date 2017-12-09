#pragma once
#include <IControl.h>

#include <memory>

namespace mpc {
	namespace hardware {
		class HwPad;
	}
}

class PadControl
	: public IPanelControl	
	{

private:
	std::weak_ptr<mpc::hardware::HwPad> pad;

private:
	int getVelo(int x, int y);

public:
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDown(int x, int y, IMouseMod* pMod) override;
	void OnMouseDblClick(int x, int y, IMouseMod* pMod) override;
	void OnMouseUp(int x, int y, IMouseMod* pMod) override;

public:
	PadControl(IPlugBase* pPlug, IRECT rect, std::weak_ptr<mpc::hardware::HwPad> pad);
	~PadControl();

};
