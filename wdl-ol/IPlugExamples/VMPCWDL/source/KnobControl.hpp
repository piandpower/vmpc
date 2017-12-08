#pragma once
#include <IControl.h>

#include <memory>

//#include <observer/Observer.hpp>

namespace mpc {
	namespace hardware {
		class Pot;
	}
}

class KnobControl
	: public IPanelControl
	//, public moduru::observer::Observer
	
	{

private:
	IBitmap knobs{};
	//std::weak_ptr<mpc::audiomidi::AudioMidiServices> ams{};
	std::weak_ptr<mpc::hardware::Pot> pot;
	int knobIndex{ 0 };
	int knobType{ 0 }; // 0 = rec, 1 = vol

public:
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) override;

	/*
	* Implement Observer
	*/
	public:
		//void update(moduru::observer::Observable* o, boost::any arg) override;

public:
	KnobControl(IPlugBase* pPlug, int type, IBitmap knobs, std::weak_ptr<mpc::hardware::Pot> pot, int startIndex);
	~KnobControl();

};
