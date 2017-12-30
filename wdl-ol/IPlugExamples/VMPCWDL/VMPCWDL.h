#ifndef __VMPCWDL__
#define __VMPCWDL__

#include "IPlug_include_in_plug_hdr.h"
#include "VMPCWDLDSP.h"

#include <Mpc.hpp>

#define MAX_VOICES 16
#define ATTACK_DEFAULT 5.
#define DECAY_DEFAULT 20.
#define RELEASE_DEFAULT 500.
#define TIME_MIN 2.
#define TIME_MAX 5000.

class LedControl;
class KnobControl;
class DataWheelControl;
class LCDControl;
class InputCatcherControl;

class VMPCWDL : public IPlug
{
public:

	VMPCWDL(IPlugInstanceInfo instanceInfo);
	~VMPCWDL();

	void Reset();
	void OnParamChange(int paramIdx);

	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames, int outputChannels);
	bool HostRequestingAboutBox();

	void ProcessMidiMsg(IMidiMsg* pMsg);

private:
	mpc::Mpc* mpc = nullptr;

private:
	DataWheelControl* mDataWheel;
	KnobControl* mRecKnob;
	KnobControl* mVolKnob;
	LedControl* mLedPanel;
	LCDControl* mLCDControl;
	InputCatcherControl* mInputCatcher;

	bool m_WasPlaying = false;
	double mSampleRate;
	double m_Tempo = 120.0;
};

#endif //__VMPCWDL__
