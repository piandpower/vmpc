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

KnobControl::KnobControl(IPlugBase* pPlug, int type, IBitmap* knobs, std::weak_ptr<mpc::hardware::Pot> pot, int startIndex)
	: IBitmapControl(pPlug, type == 0 ? Constants::RECKNOB_RECT()->L : Constants::VOLKNOB_RECT()->L, type == 0 ? Constants::RECKNOB_RECT()->T : Constants::VOLKNOB_RECT()->T, knobs)
{
	knobType = type;
	knobIndex = startIndex;
	this->pot = pot;
	knobIndex = pot.lock()->getValue();
	clampIndex(knobIndex);
	SetDirty(false);
}

void KnobControl::OnMouseDrag(int x, int y, int dX, int dY, IMouseMod* pMod) {
	std::string val = std::to_string(pot.lock()->getValue());
	pot.lock()->setValue(pot.lock()->getValue() + dY);
	knobIndex = pot.lock()->getValue();
	clampIndex(knobIndex);
	SetDirty(false);
}

bool KnobControl::Draw(IGraphics* g) {
	IRECT r(mDrawRECT.L, mDrawRECT.T, mDrawRECT.R, mDrawRECT.B - 1);
	return g->DrawBitmap(mBitmap, &r, knobIndex, &mBlend);
}

KnobControl::~KnobControl() {
}
