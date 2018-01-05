#pragma once
#include <IControl.h>
#include <string>
#include <memory>
#include <unordered_map>

#include "FTControl.hpp"

namespace mpc {
	namespace hardware {
		class Button;
	}
}

class ButtonControl
	: public IPanelControl	
	{

	private:
	std::weak_ptr<mpc::hardware::Button> button;
	FTControl* kbLabel;
	bool mouseEntered = false;

	public:
		static std::unordered_map<std::string, IRECT*> rects;
		static void initRects();

	private:
		static IRECT undoseq;
		static IRECT erase;
		static IRECT rec;
		static IRECT overdub;
		static IRECT stop;
		static IRECT play;
		static IRECT playstart;
		static IRECT mainscreen;
		static IRECT openwindow;
		static IRECT taptemponoterepeat;
		static IRECT prevstepevent;
		static IRECT nextstepevent;
		static IRECT gotoRect;
		static IRECT prevbarstart;
		static IRECT nextbarend;
		static IRECT f1;
		static IRECT f2;
		static IRECT f3;
		static IRECT f4;
		static IRECT f5;
		static IRECT f6;
		static IRECT notevariationafter;
		static IRECT rect0;
		static IRECT rect1;
		static IRECT rect2;
		static IRECT rect3;
		static IRECT rect4;
		static IRECT rect5;
		static IRECT rect6;
		static IRECT rect7;
		static IRECT rect8;
		static IRECT rect9;
		static IRECT shift;
		static IRECT enter;
		static IRECT banka;
		static IRECT bankb;
		static IRECT bankc;
		static IRECT bankd;
		static IRECT fulllevel;
		static IRECT sixteenlevels;
		static IRECT nextseq;
		static IRECT trackmute;

public:
	bool Draw(IGraphics* pGraphics) override;
	void OnMouseDown(int x, int y, IMouseMod* pMod) override;
	void OnMouseUp(int x, int y, IMouseMod* pMod) override;
	void OnMouseOver(int x, int y, IMouseMod *pMod) override;
	void OnMouseOut() override;

public:
	ButtonControl(IPlugBase* pPlug, IGraphics* gui, IRECT rect, std::weak_ptr<mpc::hardware::Button> button);
	~ButtonControl();

};
