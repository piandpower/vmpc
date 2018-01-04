#pragma once
#include <IControl.h>

//#include <controls/KbMouseController.hpp>

#include <memory>

namespace mpc {
	class Mpc;
	namespace controls {
		class KbMapping;
	}
}

class InputCatcherControl
	: public IPanelControl {

private:
	mpc::Mpc* mpc;
	mpc::controls::KbMapping* kbMapping;

public:
	bool OnKeyDown(int x, int y, int key) override;
	bool OnKeyUp(int x, int y, int key) override;

public:
	InputCatcherControl(IPlugBase* pPlug, mpc::Mpc* mpc);
	~InputCatcherControl();

};
