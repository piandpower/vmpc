#include "KnobControl.hpp"
#include <Logger.hpp>
//#include <mpc/audiomidi/AudioMidiServices.hpp>
#include <hardware/Pot.hpp>

#include "Constants.hpp"
//#include <mpc/gui/ControlPanel.hpp>

#include  "../resource.h"

static inline void clampIndex(int& knobIndex) {
	if (knobIndex < 0) {
		knobIndex = 0;
	}
	else if (knobIndex > 99) {
		knobIndex = 99;
	}
}

KnobControl::KnobControl(IPlugBase* pPlug, int type, IBitmap knobs, std::weak_ptr<mpc::hardware::Pot> pot, int startIndex)
	: IPanelControl(pPlug, type == 0 ? *Constants::RECKNOB_RECT() : *Constants::VOLKNOB_RECT(), Constants::LCD_OFF())
{
	knobType = type;
	knobIndex = startIndex;
	this->knobs = knobs;
	this->pot = pot;
	knobIndex = pot.lock()->getValue();
	clampIndex(knobIndex);
	SetDirty(false);
}

//void KnobControl::update(moduru::observer::Observable* o, boost::any arg) {
	//int increment = boost::any_cast<int>(arg);
	//knobIndex += increment;
	//clampIndex(knobIndex);
	//SetDirty();
//}

void KnobControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
	std::string val = std::to_string(pot.lock()->getValue());
	pot.lock()->setValue(pot.lock()->getValue() + dY);
	knobIndex = pot.lock()->getValue();
	clampIndex(knobIndex);
	SetDirty(false);
}

bool KnobControl::Draw(IGraphics* g) {
	IChannelBlend tmp = IChannelBlend::kBlendNone;
	g->DrawBitmap(&knobs, GetRECT(), 0, knobIndex * GetRECT()->H(), &tmp);
	return true;
}

KnobControl::~KnobControl() {
}
