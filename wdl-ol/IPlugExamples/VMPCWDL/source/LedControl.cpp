#include "LedControl.hpp"
#include "Constants.hpp"

#include <string>

#include "../resource.h"

using namespace std;

LedControl::LedControl(IPlugBase* pPlug, IGraphics* pGraphics) 
	: IPanelControl(pPlug, *Constants::PLUG_RECT(), Constants::LCD_OFF())
{
	ledGreen = pGraphics->LoadIBitmap(LED_GREEN_ID, LED_GREEN_FN);
	ledRed = pGraphics->LoadIBitmap(LED_RED_ID, LED_RED_FN);
	
	int x, y;
	int ledSize = 10 * gui_scale;
	ledGreen = pGraphics->ScaleBitmap(&ledGreen, ledSize, ledSize);
	ledRed = pGraphics->ScaleBitmap(&ledRed, ledSize, ledSize);

	x = 874 * gui_scale;
	y = 216 * gui_scale;
	sixteenLevels = IRECT(x, y, x + ledSize, y + ledSize);
	x = 791 * gui_scale;
	y = 298 * gui_scale;
	nextSeq = IRECT(x, y, x + ledSize, y + ledSize);
	x = 875 * gui_scale;
	y = 298 * gui_scale;
	trackMute = IRECT(x, y, x + ledSize, y + ledSize);
	x = 103 * gui_scale;
	y = 601 * gui_scale;
	after = IRECT(x, y, x + ledSize, y + ledSize);
	x = 226 * gui_scale;
	y = 686 * gui_scale;
	undoSeq = IRECT(x, y, x + ledSize, y + ledSize);
	x = 214 * gui_scale;
	y = 833 * gui_scale;
	rec = IRECT(x, y, x + ledSize, y + ledSize);
	x = 294 * gui_scale;
	y = 833 * gui_scale;
	overDub = IRECT(x, y, x + ledSize, y + ledSize);
	x = 451 * gui_scale;
	y = 830 * gui_scale;
	play = IRECT(x, y, x + ledSize, y + ledSize);
	x = 791 * gui_scale;
	y = 216 * gui_scale;
	fullLevel = IRECT(x, y, x  + ledSize, y + ledSize);
	x = 958 * gui_scale;
	y = 298 * gui_scale;
	padBankA = IRECT(x, y, x + ledSize, y + ledSize);
	x = 1041 * gui_scale;
	y = 298 * gui_scale;
	padBankB = IRECT(x, y, x + ledSize, y + ledSize);
	x = 1124 * gui_scale;
	y = 297 * gui_scale;
	padBankC = IRECT(x, y, x + ledSize, y + ledSize);
	x = 1206 * gui_scale;
	y = 296 * gui_scale;
	padBankD = IRECT(x, y, x + ledSize, y + ledSize);
}

bool LedControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	if (fullLevelOn) {
		g->DrawBitmap(&ledGreen, &fullLevel, 0, &tmp);
	}
	if (padBankAOn) {
		g->DrawBitmap(&ledGreen, &padBankA, 0, &tmp);
	}
	if (padBankBOn) {
		g->DrawBitmap(&ledGreen, &padBankB, 0, &tmp);
	}
	if (padBankCOn) {
		g->DrawBitmap(&ledGreen, &padBankC, 0, &tmp);
	}
	if (padBankDOn) {
		g->DrawBitmap(&ledGreen, &padBankD, 0, &tmp);
	}
	if (this->afterOn) {
		g->DrawBitmap(&ledGreen, &after, 0, &tmp);
	}
	if (sixteenLevelsOn) { g->DrawBitmap(&ledGreen, &sixteenLevels, 0, &tmp); }
	if (nextSeqOn) { g->DrawBitmap(&ledGreen, &nextSeq, 0, &tmp); }
	if (trackMuteOn) { g->DrawBitmap(&ledGreen, &trackMute, 0, &tmp); }
	if (undoSeqOn) { g->DrawBitmap(&ledGreen, &undoSeq, 0, &tmp); }
	if (recOn) { g->DrawBitmap(&ledRed, &rec, 0, &tmp); }
	if (overDubOn) { g->DrawBitmap(&ledRed, &overDub, 0, &tmp); }
	if (playOn) { g->DrawBitmap(&ledGreen, &play, 0, &tmp); }

	return true;
}

void LedControl::setPadBankA(bool b)
{
    padBankAOn = b;
	SetDirty(true);
}

void LedControl::setPadBankB(bool b)
{
    padBankBOn = b;
	SetDirty(true);
}

void LedControl::setPadBankC(bool b)
{
    padBankCOn = b;
	SetDirty(true);
}

void LedControl::setPadBankD(bool b)
{
    padBankDOn = b;
	SetDirty(true);
}

void LedControl::setFullLevel(bool b)
{
    fullLevelOn = b;
	SetDirty(true);
}

void LedControl::setSixteenLevels(bool b)
{
    sixteenLevelsOn = b;
	SetDirty(true);
}

void LedControl::setNextSeq(bool b)
{
    nextSeqOn = b;
	SetDirty(true);
}

void LedControl::setTrackMute(bool b)
{
    trackMuteOn = b;
	SetDirty(true);
}

void LedControl::setAfter(bool b)
{
    afterOn = b;
	SetDirty(true);
}

void LedControl::setRec(bool b)
{
    recOn = b;
	SetDirty(true);
}

void LedControl::setOverDub(bool b)
{
    overDubOn = b;
	SetDirty(true);
}

void LedControl::setPlay(bool b)
{
    playOn = b;
	SetDirty(true);
}

void LedControl::setUndoSeq(bool b)
{
    undoSeqOn = b;
	SetDirty(true);
}

void LedControl::update(moduru::observer::Observable* o, boost::any arg) {
	string s = boost::any_cast<string>(arg);
	if (s.compare("fulllevelon") == 0) {
		setFullLevel(true);
	}
	else if(s.compare("fullleveloff") == 0) {
		setFullLevel(false);
	}
	else if (s.compare("sixteenlevelson") == 0) {
		setSixteenLevels(true);
	}
	else if (s.compare("sixteenlevelsoff") == 0) {
		setSixteenLevels(false);
	}
	else if (s.compare("nextseqon") == 0) {
		setNextSeq(true);
	}
	else if (s.compare("nextseqoff") == 0) {
		setNextSeq(false);
	}
	else if (s.compare("trackmuteon") == 0) {
		setTrackMute(true);
	}
	else if (s.compare("trackmuteoff") == 0) {
		setTrackMute(false);
	}
	else if (s.compare("padbankaon") == 0) {
		setPadBankA(true);
	}
	else if (s.compare("padbankaoff") == 0) {
		setPadBankA(false);
	}
	else if (s.compare("padbankbon") == 0) {
		setPadBankB(true);
	}
	else if (s.compare("padbankboff") == 0) {
		setPadBankB(false);
	}
	else if (s.compare("padbankcon") == 0) {
		setPadBankC(true);
	}
	else if (s.compare("padbankcoff") == 0) {
		setPadBankC(false);
	}
	else if (s.compare("padbankdon") == 0) {
		setPadBankD(true);
	}
	else if (s.compare("padbankdoff") == 0) {
		setPadBankD(false);
	}
	else if (s.compare("afteron") == 0) {
		setAfter(true);
	}
	else if (s.compare("afteroff") == 0) {
		setAfter(false);
	}
	else if (s.compare("undoseqon") == 0) {
		setUndoSeq(true);
	}
	else if (s.compare("undoseqoff") == 0) {
		setUndoSeq(false);
	}
	else if (s.compare("recon") == 0) {
		setRec(true);
	}
	else if (s.compare("recoff") == 0) {
		setRec(false);
	}
	else if (s.compare("overdubon") == 0) {
		setOverDub(true);
	}
	else if (s.compare("overduboff") == 0) {
		setOverDub(false);
	}
	else if (s.compare("playon") == 0) {
		setPlay(true);
	}
	else if (s.compare("playoff") == 0) {
		setPlay(false);
	}
}

LedControl::~LedControl() {
}
