#pragma once
#include <IControl.h>

#include <vector>
#include <memory>

namespace mpc {
	namespace lcdgui {
		class LayeredScreen;
	}
}

class LCDControl
	: public IPanelControl {

private:
	std::weak_ptr<mpc::lcdgui::LayeredScreen> ls;

public:
	bool Draw(IGraphics* pGraphics) override;
	bool IsDirty() override;
	//void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;

public:
	LCDControl(IPlugBase* pPlug, std::weak_ptr<mpc::lcdgui::LayeredScreen> ls);
	~LCDControl();

};
