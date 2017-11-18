#pragma once
#include <IControl.h>

#include <vector>
#include <memory>

class LCDControl
	: public IPanelControl {

private:
	std::vector<std::vector<bool>>* pixels;
	//IBitmap lcd{};

public:
	bool Draw(IGraphics* pGraphics) override;
	//void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;

	LCDControl(IPlugBase* pPlug, std::vector<std::vector<bool>>* pixels);
	~LCDControl();

};
