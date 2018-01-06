#include "PadControl.hpp"
#include <hardware/HwPad.hpp>

#include "Constants.hpp"

#include <math.h>

#include  "../resource.h"

using namespace std;

PadControl::PadControl(IPlugBase* pPlug, IRECT rect, std::weak_ptr<mpc::hardware::HwPad> pad, IBitmap* padhit)
	: IBitmapControl(pPlug, rect.L, rect.T, padhit)
{
	this->pad = pad;
	pad.lock()->addObserver(this);
}

void PadControl::static_fadeOut(void * args)
{
	static_cast<PadControl*>(args)->fadeOut();
}

void PadControl::fadeOut() {
	fading = true;
	this_thread::sleep_for(chrono::milliseconds(20));
	while (padhitBrightness > 0) {
		this_thread::sleep_for(chrono::milliseconds(2));
		padhitBrightness -= 4;
		if (padhitBrightness < 0) padhitBrightness = 0;
		SetDirty();
	}
	fading = false;
}

void PadControl::update(moduru::observer::Observable* o, boost::any arg) {
	int velocity = boost::any_cast<int>(arg);
	if (velocity != 255) {
		padhitBrightness = velocity + 25;
		SetDirty();
	}
	else {
		if (fading) return;
		if (fadeOutThread.joinable()) fadeOutThread.join();
		fadeOutThread = thread(&PadControl::static_fadeOut, this);
	}
}

int PadControl::getVelo(int x, int y) {
	float centX = GetDrawRECT()->MW();
	float centY = GetDrawRECT()->MH();
	float distX = x - centX;
	float distY = y - centY;
	float powX = pow(distX, 2);
	float powY = pow(distY, 2);
	float dist = sqrt(powX + powY);
	if (dist > 46) dist = 46;
	int velo = 127 - (dist * (127.0 / 48.0));
	return velo;
}

void PadControl::OnMouseDown(int x, int y, IMouseMod* pMod) {
	pad.lock()->push(getVelo(x, y));
}

void PadControl::OnMouseDblClick(int x, int y, IMouseMod* pMod) {
	pad.lock()->push(getVelo(x, y));
}

void PadControl::OnMouseUp(int x, int y, IMouseMod* pMod) {
	pad.lock()->release();
}

bool PadControl::Draw(IGraphics* g) {
	LICE_IBitmap* srcData = (LICE_IBitmap*)(mBitmap->mData);
	int pixelCount = mBitmap->H * mBitmap->W;
	int pixelCounter = 0;
	for (int i = 0; i < mBitmap->H; i++) {
		for (int j = 0; j < mBitmap->W; j++) {
			LICE_pixel_chan* p = (LICE_pixel_chan*) &srcData->getBits()[pixelCounter++];
			p[LICE_PIXEL_A] = padhitBrightness;
		}
	}
	IChannelBlend tmp = IChannelBlend::kBlendAdd;
	return g->DrawBitmap(mBitmap, GetDrawRECT(), 1, &tmp);
}

PadControl::~PadControl() {
	if (fadeOutThread.joinable()) fadeOutThread.join();
}
