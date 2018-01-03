#pragma once
#include <IControl.h>

//#include <controls/KbMouseController.hpp>

#include <memory>

namespace mpc {
	class Mpc;
}

class InputCatcherControl
	: public IPanelControl {

private:
	mpc::Mpc* mpc;

public:
	bool OnKeyDown(int x, int y, int key) override;
	bool OnKeyUp(int x, int y, int key) override;

public:
	InputCatcherControl(IPlugBase* pPlug, mpc::Mpc* mpc);
	~InputCatcherControl();

};
