#pragma once
#include <IControl.h>

#include <controls/KbMouseController.hpp>

#include <memory>

namespace mpc {
	namespace controls {
		class KbMouseController;
	}
}

class InputCatcherControl
	: public IPanelControl {

private:
	mpc::controls::KbMouseController* kbmc;

public:
	bool OnKeyDown(int x, int y, int key) override;
	bool OnKeyUp(int x, int y, int key) override;

	InputCatcherControl(IPlugBase* pPlug, mpc::controls::KbMouseController* kbmc);
	~InputCatcherControl();

};
