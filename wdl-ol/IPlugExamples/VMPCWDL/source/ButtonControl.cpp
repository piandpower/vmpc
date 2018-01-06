#include "ButtonControl.hpp"
#include <hardware/Button.hpp>

#include "Constants.hpp"

#include "FTControl.hpp"
#include <controls/KbMapping.hpp>

#include <math.h>
#include <algorithm>
#include  "../resource.h"

#include <Logger.hpp>
#include <thirdp/wrpkey/key.hxx>

ButtonControl::ButtonControl(IPlugBase* pPlug, IGraphics* gui, IRECT rect, std::weak_ptr<mpc::hardware::Button> button)
	: IPanelControl(pPlug, rect, Constants::LCD_OFF())
{
	this->button = button;
	mpc::controls::KbMapping kbMapping;
	auto buttonLabel = button.lock()->getLabel();
	int keyCode = kbMapping.getKeyCodeFromLabel(buttonLabel);
	if (buttonLabel.compare("bankc") == 0) {
		//MLOG("bankc keyCode: " + std::to_string(keyCode));
	}
	auto labelString = kbMapping.getKeyCodeString(keyCode);
	std::transform(labelString.begin(), labelString.end(), labelString.begin(), ::toupper);
	int labelWidth = FTControl::getStringWidth(labelString);
	int yMouseCursorOffset = Constants::KBLABEL_FONT_SIZE;
	int labelOffsetX = rect.MW() - (labelWidth / 2) - ((Constants::KBLABEL_OUTLINE_SIZE));
	int labelOffsetY = rect.MH() - (((Constants::KBLABEL_FONT_SIZE / 2) - Constants::KBLABEL_OUTLINE_SIZE + yMouseCursorOffset));
	kbLabel = new FTControl(pPlug, labelOffsetX, labelOffsetY, labelString);
	kbLabel->Hide(true);
	gui->AttachControl(kbLabel);
}
std::unordered_map<std::string, IRECT*> ButtonControl::rects;
IRECT ButtonControl::undoseq = IRECT(212, 652, 252, 669);
IRECT ButtonControl::erase = IRECT(291, 651, 332, 667);
IRECT ButtonControl::rec = IRECT(208, 808, 258, 836);
IRECT ButtonControl::overdub = IRECT(288, 806, 336, 836);
IRECT ButtonControl::stop = IRECT(367, 804, 416, 835);
IRECT ButtonControl::play = IRECT(446, 802, 495, 834);
IRECT ButtonControl::playstart = IRECT(525, 800, 573, 833);
IRECT ButtonControl::mainscreen = IRECT(382, 307, 422, 325);
IRECT ButtonControl::openwindow = IRECT(510, 308, 551, 325);
IRECT ButtonControl::taptemponoterepeat = IRECT(247, 556, 298, 590);
IRECT ButtonControl::prevstepevent = IRECT(212, 723, 252, 740);
IRECT ButtonControl::nextstepevent = IRECT(292, 721, 332, 740);
IRECT ButtonControl::gotoRect = IRECT(372, 719, 411, 736);
IRECT ButtonControl::prevbarstart = IRECT(450, 716, 489, 734);
IRECT ButtonControl::nextbarend = IRECT(528, 718, 566, 735);
IRECT ButtonControl::f1 = IRECT(144, 262, 176, 273);
IRECT ButtonControl::f2 = IRECT(225, 261, 257, 273);
IRECT ButtonControl::f3 = IRECT(310, 263, 341, 273);
IRECT ButtonControl::f4 = IRECT(394, 263, 423, 272);
IRECT ButtonControl::f5 = IRECT(474, 261, 505, 272);
IRECT ButtonControl::f6 = IRECT(554, 262, 586, 272);
IRECT ButtonControl::notevariationafter = IRECT(88, 568, 129, 584);
IRECT ButtonControl::rect0 = IRECT(170, 460, 205, 475);
IRECT ButtonControl::rect1 = IRECT(91, 413, 123, 424);
IRECT ButtonControl::rect2 = IRECT(174, 414, 205, 425);
IRECT ButtonControl::rect3 = IRECT(253, 415, 287, 425);
IRECT ButtonControl::rect4 = IRECT(91, 365, 121, 377);
IRECT ButtonControl::rect5 = IRECT(173, 364, 204, 377);
IRECT ButtonControl::rect6 = IRECT(253, 365, 288, 378);
IRECT ButtonControl::rect7 = IRECT(88, 315, 123, 329);
IRECT ButtonControl::rect8 = IRECT(170, 315, 205, 329);
IRECT ButtonControl::rect9 = IRECT(251, 315, 286, 331);
IRECT ButtonControl::shift = IRECT(90, 462, 127, 476);
IRECT ButtonControl::enter = IRECT(253, 460, 288, 475);
IRECT ButtonControl::banka = IRECT(942, 259, 985, 278);
IRECT ButtonControl::bankb = IRECT(1026, 260, 1068, 278);
IRECT ButtonControl::bankc = IRECT(1110, 257, 1150, 277);
IRECT ButtonControl::bankd = IRECT(1192, 258, 1232, 277);
IRECT ButtonControl::fulllevel = IRECT(777, 178, 817, 197);
IRECT ButtonControl::sixteenlevels = IRECT(861, 180, 900, 195);
IRECT ButtonControl::nextseq = IRECT(778, 263, 817, 279);
IRECT ButtonControl::trackmute = IRECT(860, 261, 900, 278);

void ButtonControl::initRects() {
	if (rects.size() != 0) return;

	rects.emplace("rec", &rec);
	rects.emplace("overdub", &overdub);
	rects.emplace("stop", &stop);
	rects.emplace("play", &play);
	rects.emplace("playstart", &playstart);
	rects.emplace("mainscreen", &mainscreen);
	rects.emplace("prevstepevent", &prevstepevent);
	rects.emplace("nextstepevent", &nextstepevent);
	rects.emplace("goto", &gotoRect);
	rects.emplace("prevbarstart", &prevbarstart);
	rects.emplace("nextbarend", &nextbarend);
	rects.emplace("tap", &taptemponoterepeat);
	rects.emplace("nextseq", &nextseq);
	rects.emplace("trackmute", &trackmute);
	rects.emplace("openwindow", &openwindow);
	rects.emplace("fulllevel", &fulllevel);
	rects.emplace("sixteenlevels", &sixteenlevels);
	rects.emplace("f1", &f1);
	rects.emplace("f2", &f2);
	rects.emplace("f3", &f3);
	rects.emplace("f4", &f4);
	rects.emplace("f5", &f5);
	rects.emplace("f6", &f6);
	rects.emplace("shift", &shift);
	rects.emplace("enter", &enter);
	rects.emplace("undoseq", &undoseq);
	rects.emplace("erase", &erase);
	rects.emplace("after", &notevariationafter);
	rects.emplace("banka", &banka);
	rects.emplace("bankb", &bankb);
	rects.emplace("bankc", &bankc);
	rects.emplace("bankd", &bankd);
	for (auto r : rects) {
		r.second->T += 55;
		r.second->B += 55;
	}
}

void ButtonControl::OnMouseDown(int x, int y, IMouseMod* pMod) {
	button.lock()->push();
	kbLabel->Hide(true);
}

void ButtonControl::OnMouseUp(int x, int y, IMouseMod* pMod) {
	button.lock()->release();
}

void ButtonControl::OnMouseOver(int x, int y, IMouseMod *pMod) {
	if (mouseEntered) return;
	mouseEntered = true;
	kbLabel->Hide(false);
}

void ButtonControl::OnMouseOut() {
	mouseEntered = false;
	kbLabel->Hide(true);
}

bool ButtonControl::Draw(IGraphics* g) {
	return false;
}

ButtonControl::~ButtonControl() {
}
