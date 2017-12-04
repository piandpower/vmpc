#include <controls/other/OthersControls.hpp>

#include <lcdgui/LayeredScreen.hpp>
//#include <ui/other/OthersGui.hpp>

using namespace mpc::controls::other;
using namespace std;

OthersControls::OthersControls(mpc::Mpc* mpc) : AbstractControls(mpc)
{
}

void OthersControls::function(int i)
{
	init();
	switch (i) {
	case 1:
		ls.lock()->openScreen("init");
		break;
	case 2:
		ls.lock()->openScreen("ver");
		break;
	}
}

void OthersControls::turnWheel(int i)
{
    init();
    //if(param.compare("tapaveraging") == 0 && !mpc::controls::KbMouseController::altIsPressed())
    //    lGui->getOthersGui()->setTapAveraging(lGui->getOthersGui()->getTapAveraging() + i);

  //  if(mpc::controls::KbMouseController::altIsPressed())
//        lGui->getOthersGui()->setContrast(lGui->getOthersGui()->getContrast() + i);

}
