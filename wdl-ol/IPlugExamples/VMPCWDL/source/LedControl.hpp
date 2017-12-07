#pragma once

#include <IControl.h>

#include <observer/Observer.hpp>

class LedControl
	: public IPanelControl
	, public moduru::observer::Observer
{

private:
	IBitmap ledGreen;
	IBitmap ledRed;

	IRECT fullLevel{};
	IRECT sixteenLevels{};
	IRECT nextSeq{};
	IRECT trackMute{};
	IRECT padBankA{};
	IRECT padBankB{};
	IRECT padBankC{};
	IRECT padBankD{};
	IRECT after{};
	IRECT undoSeq{};
	IRECT rec{};
	IRECT overDub{};
	IRECT play{};

	bool fullLevelOn{};
	bool sixteenLevelsOn{};
	bool nextSeqOn{};
	bool trackMuteOn{};
	bool padBankAOn{};
	bool padBankBOn{};
	bool padBankCOn{};
	bool padBankDOn{};
	bool afterOn{};
	bool undoSeqOn{};
	bool recOn{};
	bool overDubOn{};
	bool playOn{};

public:
	void setPadBankA(bool b);
	void setPadBankB(bool b);
	void setPadBankC(bool b);
	void setPadBankD(bool b);
	void setFullLevel(bool b);
	void setSixteenLevels(bool b);
	void setNextSeq(bool b);
	void setTrackMute(bool b);
	void setAfter(bool b);
	void setRec(bool b);
	void setOverDub(bool b);
	void setPlay(bool b);
	void setUndoSeq(bool b);

	/*
	* Implement IPanelControl
	*/
	bool Draw(IGraphics* pGraphics) override;

	/*
	* Implement Observer
	*/
public:
	void update(moduru::observer::Observable* o, boost::any arg) override;

public:
	LedControl(IPlugBase* pPlug, IGraphics* pGraphics);
	~LedControl();

};
