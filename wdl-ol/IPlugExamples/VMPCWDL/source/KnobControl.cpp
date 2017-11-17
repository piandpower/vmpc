#include "KnobControl.hpp"

//#include <mpc/audiomidi/AudioMidiServices.hpp>

//#include <mpc/gui/Gui.hpp>
#include "Constants.hpp"
//#include <mpc/gui/ControlPanel.hpp>

#include  "../resource.h"

KnobControl::KnobControl(IPlugBase* pPlug, int type, IBitmap knobs)
	: IPanelControl(pPlug, type == 0 ? *Constants::RECKNOB_RECT() : *Constants::VOLKNOB_RECT(), Constants::LCD_OFF())
{
	//knobIndex = startIndex;
	//knobType = type;
	this->knobs = knobs;
}

void KnobControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
	
	knobIndex -= dY;
	
	if (knobIndex < 0)
		knobIndex = 0;

	if (knobIndex > 99)
		knobIndex = 99;

	if (knobType == 1) {
		//ams.lock()->setMasterLevel(knobIndex);
	}
	SetDirty(false);
}

bool KnobControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	g->DrawBitmap(&knobs, GetRECT(), 0, knobIndex * GetRECT()->H(), &tmp);
	return true;
}

int KnobControl::getKnobIndex() {
	return knobIndex;
}

KnobControl::~KnobControl() {
	//GetGUI()->ReleaseBitmap(&knobs);
}
