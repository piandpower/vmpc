#include "Hardware.hpp"

#include <Mpc.hpp>

#include "Button.hpp"
#include "DataWheel.hpp"
#include "HwPad.hpp"

using namespace mpc::hardware;
using namespace std;

Hardware::Hardware(mpc::Mpc* mpc)
{
	vector<string> buttonLabels{ "left", "right", "up", "down",	"rec", "overdub", "stop", "play", "playstart", "mainscreen", "prevstepevent", "nextstepevent",	"goto",	"prevbarstart",	"nextbarend", "tap", "nextseq",	"trackmute", "openwindow", "fulllevel", "sixteenlevels", "f1", "f2", "f3", "f4", "f5", "f6", "shift", "enter", "undoseq", "erase", "after", "banka", "bankb", "bankc", "bankd" };
	/*
	static std::vector<int> bankKeys",
	static std::vector<int> padKeys",
	static std::vector<int> altNumPad",
	*/
	for (auto& l : buttonLabels)
		buttons.push_back(std::make_shared<Button>(mpc, l));

	for (int i = 0; i <= 9; i++)
		buttons.push_back(std::make_shared<Button>(mpc, std::to_string(i)));
	
	for (int i = 0; i <= 15; i++)
		pads.push_back(std::make_shared<HwPad>(mpc, i));

	dataWheel = make_shared<DataWheel>(mpc);

}

weak_ptr<HwPad> Hardware::getPad(int index) {
	return pads.at(index);
}

weak_ptr<Button> Hardware::getButton(std::string label) {
	for (auto b : buttons)
		if (b->getLabel().compare(label) == 0) return b;
	return weak_ptr<Button>();
}

weak_ptr<DataWheel> Hardware::getDataWheel() {
	return dataWheel;
}

Hardware::~Hardware() {
}
