#include "ButtonControl.hpp"
#include <hardware/Button.hpp>

#include "Constants.hpp"
//#include <mpc/gui/ControlPanel.hpp>

#include <math.h>

#include  "../resource.h"

ButtonControl::ButtonControl(IPlugBase* pPlug, IRECT rect, std::weak_ptr<mpc::hardware::Button> button)
	: IPanelControl(pPlug, rect, Constants::LCD_OFF())
{
	this->button = button;
}
std::unordered_map<std::string, IRECT*> ButtonControl::rects;
IRECT ButtonControl::undoseq = IRECT(212 * gui_scale, 652 * gui_scale, 252 * gui_scale, 669 * gui_scale);
IRECT ButtonControl::erase = IRECT(291 * gui_scale, 651 * gui_scale, 332 * gui_scale, 667 * gui_scale);
IRECT ButtonControl::rec = IRECT(208 * gui_scale, 808 * gui_scale, 258 * gui_scale, 836 * gui_scale);
IRECT ButtonControl::overdub = IRECT(288 * gui_scale, 806 * gui_scale, 336 * gui_scale, 836 * gui_scale);
IRECT ButtonControl::stop = IRECT(367 * gui_scale, 804 * gui_scale, 416 * gui_scale, 835 * gui_scale);
IRECT ButtonControl::play = IRECT(446 * gui_scale, 802 * gui_scale, 495 * gui_scale, 834 * gui_scale);
IRECT ButtonControl::playstart = IRECT(525 * gui_scale, 800 * gui_scale, 573 * gui_scale, 833 * gui_scale);
IRECT ButtonControl::mainscreen = IRECT(382 * gui_scale, 307 * gui_scale, 422 * gui_scale, 325 * gui_scale);
IRECT ButtonControl::openwindow = IRECT(510 * gui_scale, 308 * gui_scale, 551 * gui_scale, 325 * gui_scale);
IRECT ButtonControl::taptemponoterepeat = IRECT(247 * gui_scale, 556 * gui_scale, 298 * gui_scale, 590 * gui_scale);
IRECT ButtonControl::prevstepevent = IRECT(212 * gui_scale, 723 * gui_scale, 252 * gui_scale, 740 * gui_scale);
IRECT ButtonControl::nextstepevent = IRECT(292 * gui_scale, 721 * gui_scale, 332 * gui_scale, 740 * gui_scale);
IRECT ButtonControl::gotoRect = IRECT(372 * gui_scale, 719 * gui_scale, 411 * gui_scale, 736 * gui_scale);
IRECT ButtonControl::prevbarstart = IRECT(450 * gui_scale, 716 * gui_scale, 489 * gui_scale, 734 * gui_scale);
IRECT ButtonControl::nextbarend = IRECT(528 * gui_scale, 718 * gui_scale, 566 * gui_scale, 735 * gui_scale);
IRECT ButtonControl::f1 = IRECT(144 * gui_scale, 262 * gui_scale, 176 * gui_scale, 273 * gui_scale);
IRECT ButtonControl::f2 = IRECT(225 * gui_scale, 261 * gui_scale, 257 * gui_scale, 273 * gui_scale);
IRECT ButtonControl::f3 = IRECT(310 * gui_scale, 263 * gui_scale, 341 * gui_scale, 273 * gui_scale);
IRECT ButtonControl::f4 = IRECT(394 * gui_scale, 263 * gui_scale, 423 * gui_scale, 272 * gui_scale);
IRECT ButtonControl::f5 = IRECT(474 * gui_scale, 261 * gui_scale, 505 * gui_scale, 272 * gui_scale);
IRECT ButtonControl::f6 = IRECT(554 * gui_scale, 262 * gui_scale, 586 * gui_scale, 272 * gui_scale);
IRECT ButtonControl::notevariationafter = IRECT(88 * gui_scale, 568 * gui_scale, 129 * gui_scale, 584 * gui_scale);
IRECT ButtonControl::rect0 = IRECT(170 * gui_scale, 460 * gui_scale, 205 * gui_scale, 475 * gui_scale);
IRECT ButtonControl::rect1 = IRECT(91 * gui_scale, 413 * gui_scale, 123 * gui_scale, 424 * gui_scale);
IRECT ButtonControl::rect2 = IRECT(174 * gui_scale, 414 * gui_scale, 205 * gui_scale, 425 * gui_scale);
IRECT ButtonControl::rect3 = IRECT(253 * gui_scale, 415 * gui_scale, 287 * gui_scale, 425 * gui_scale);
IRECT ButtonControl::rect4 = IRECT(91 * gui_scale, 365 * gui_scale, 121 * gui_scale, 377 * gui_scale);
IRECT ButtonControl::rect5 = IRECT(173 * gui_scale, 364 * gui_scale, 204 * gui_scale, 377 * gui_scale);
IRECT ButtonControl::rect6 = IRECT(253 * gui_scale, 365 * gui_scale, 288 * gui_scale, 378 * gui_scale);
IRECT ButtonControl::rect7 = IRECT(88 * gui_scale, 315 * gui_scale, 123 * gui_scale, 329 * gui_scale);
IRECT ButtonControl::rect8 = IRECT(170 * gui_scale, 315 * gui_scale, 205 * gui_scale, 329 * gui_scale);
IRECT ButtonControl::rect9 = IRECT(251 * gui_scale, 315 * gui_scale, 286 * gui_scale, 331 * gui_scale);
IRECT ButtonControl::shift = IRECT(90 * gui_scale, 462 * gui_scale, 127 * gui_scale, 476 * gui_scale);
IRECT ButtonControl::enter = IRECT(253 * gui_scale, 460 * gui_scale, 288 * gui_scale, 475 * gui_scale);
IRECT ButtonControl::banka = IRECT(942 * gui_scale, 259 * gui_scale, 985 * gui_scale, 278 * gui_scale);
IRECT ButtonControl::bankb = IRECT(1026 * gui_scale, 260 * gui_scale, 1068 * gui_scale, 278 * gui_scale);
IRECT ButtonControl::bankc = IRECT(1110 * gui_scale, 257 * gui_scale, 1150 * gui_scale, 277 * gui_scale);
IRECT ButtonControl::bankd = IRECT(1192 * gui_scale, 258 * gui_scale, 1232 * gui_scale, 277 * gui_scale);
IRECT ButtonControl::fulllevel = IRECT(777 * gui_scale, 178 * gui_scale, 817 * gui_scale, 197 * gui_scale);
IRECT ButtonControl::sixteenlevels = IRECT(861 * gui_scale, 180 * gui_scale, 900 * gui_scale, 195 * gui_scale);
IRECT ButtonControl::nextseq = IRECT(778 * gui_scale, 263 * gui_scale, 817 * gui_scale, 279 * gui_scale);
IRECT ButtonControl::trackmute = IRECT(860 * gui_scale, 261 * gui_scale, 900 * gui_scale, 278 * gui_scale);

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
		r.second->T += 55 * gui_scale;
		r.second->B += 55 * gui_scale;
	}
}

void ButtonControl::OnMouseDown(int x, int y, IMouseMod* pMod) {
	button.lock()->push();
}

void ButtonControl::OnMouseUp(int x, int y, IMouseMod* pMod) {
	button.lock()->release();
}

bool ButtonControl::Draw(IGraphics* g) {
	//return IPanelControl::Draw(g);
	return false;
}

ButtonControl::~ButtonControl() {
}
