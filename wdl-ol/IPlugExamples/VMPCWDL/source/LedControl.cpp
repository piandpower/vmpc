#include "LedControl.hpp"
#include "Constants.hpp"

//#include <mpc/gui/StartUp.hpp>

#include "../resource.h"

//#include <moduru/file/File.hpp>

LedControl::LedControl(IPlugBase* pPlug, IGraphics* pGraphics) 
	: IPanelControl(pPlug, *Constants::PLUG_RECT(), Constants::LCD_OFF())
{
	ledGreen = pGraphics->LoadIBitmap(LED_GREEN_ID, LED_GREEN_FN);
	ledRed = pGraphics->LoadIBitmap(LED_RED_ID, LED_RED_FN);
	
	int x, y;
	x = 874;
	y = 216;
	sixteenLevels = IRECT(x, y, x + 10, y + 10);
	x = 791;
	y = 298;
	nextSeq = IRECT(x, y, x + 10, y + 10);
	x = 875;
	y = 298;
	trackMute = IRECT(x, y, x + 10, y + 10);
	x = 103;
	y = 601;
	after = IRECT(x, y, x + 10, y + 10);
	x = 226;
	y = 686;
	undoSeq = IRECT(x, y, x + 10, y + 10);
	x = 214;
	y = 833;
	rec = IRECT(x, y, x + 10, y + 10);
	x = 294;
	y = 833;
	overDub = IRECT(x, y, x + 10, y + 10);
	x = 451;
	y = 830;
	play = IRECT(x, y, x + 10, y + 10);
	x = 791;
	y = 216;
	fullLevel = IRECT(x, y, x  + 10, y + 10);
	x = 958;
	y = 298;
	padBankA = IRECT(x, y, x + 10, y + 10);
	x = 1041;
	y = 298;
	padBankB = IRECT(x, y, x + 10, y + 10);
	x = 1124;
	y = 297;
	padBankC = IRECT(x, y, x + 10, y + 10);
	x = 1206;
	y = 296;
	padBankD = IRECT(x, y, x + 10, y + 10);
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
