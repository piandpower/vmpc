#pragma once
#include <IControl.h>

#include <observer/Observer.hpp>

#include <thread>
#include <memory>

namespace mpc {
	namespace hardware {
		class HwPad;
	}
}

class PadControl
	: public IBitmapControl
	, public moduru::observer::Observer
	{

private:
	std::weak_ptr<mpc::hardware::HwPad> pad;
	int padhitBrightness = 0;

private:
	int getVelo(int x, int y);

public:
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDown(int x, int y, IMouseMod* pMod) override;
	void OnMouseDblClick(int x, int y, IMouseMod* pMod) override;
	void OnMouseUp(int x, int y, IMouseMod* pMod) override;

	public:
		void update(moduru::observer::Observable* o, boost::any arg) override;

	private:
		bool fading = false;
		std::thread fadeOutThread;
		void fadeOut();
		static void static_fadeOut(void * args);

public:
	PadControl(IPlugBase* pPlug, IRECT rect, std::weak_ptr<mpc::hardware::HwPad> pad, IBitmap* padhit);
	~PadControl();

};
