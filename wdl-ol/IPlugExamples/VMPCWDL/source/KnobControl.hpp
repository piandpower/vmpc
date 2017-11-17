#pragma once
#include <IControl.h>

#include <memory>

class KnobControl
	: public IPanelControl {

private:
	IBitmap knobs{};
	//std::weak_ptr<mpc::audiomidi::AudioMidiServices> ams{};
	int knobIndex{ 0 };
	int knobType{ 0 }; // 0 = rec, 1 = vol

public:
	int getKnobIndex();
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;

	KnobControl(IPlugBase* pPlug, int type, IBitmap knobs);
	~KnobControl();

};
