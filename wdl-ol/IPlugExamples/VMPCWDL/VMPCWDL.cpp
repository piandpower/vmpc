#include "VMPCWDL.h"
#include "IPlug_include_in_plug_src.h"
#include "resource.h"

#include "IControl.h"

#include "source/DataWheelControl.hpp"
#include "source/KnobControl.hpp"
#include "source/LedControl.hpp"
#include "source/LCDControl.hpp"
#include "source/InputCatcherControl.hpp"

#include <sequencer/Sequencer.hpp>
#include <sequencer/Sequence.hpp>

#include <audiomidi/AudioMidiServices.hpp>
#include <audio/server/UnrealAudioServer.hpp>

#include <lcdgui/LayeredScreen.hpp>

const int kNumPrograms = 8;
const int kNumParams = 0;

VMPCWDL::VMPCWDL(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo),
	mSampleRate(44100.)
{

	mpc = new mpc::Mpc();
	mpc->init();

	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	pGraphics->AttachBackground(BG_ID, BG_FN);
	mLedPanel = new LedControl(this, pGraphics);
	mLedPanel->setPadBankA(true);
	pGraphics->AttachControl(mLedPanel);

	mInputCatcher = new InputCatcherControl(this, mpc->getKbmc().lock().get());
	pGraphics->AttachControl(mInputCatcher);

	auto dataWheels = pGraphics->LoadIBitmap(DATAWHEEL_ID, DATAWHEEL_FN);
	mDataWheel = new DataWheelControl(this, dataWheels);
	pGraphics->AttachControl(mDataWheel);
	auto knobs = pGraphics->LoadIBitmap(RECKNOB_ID, RECKNOB_FN);
	mRecKnob = new KnobControl(this, 0, knobs);
	knobs = pGraphics->LoadIBitmap(VOLKNOB_ID, VOLKNOB_FN);
	mVolKnob = new KnobControl(this, 1, knobs);
	pGraphics->AttachControl(mRecKnob);
	pGraphics->AttachControl(mVolKnob);

	mLCDControl = new LCDControl(this, mpc->getLayeredScreen());
	pGraphics->AttachControl(mLCDControl);

	AttachGraphics(pGraphics);

	//MakePreset("preset 1", ... );
	MakeDefaultPreset((char *) "-", kNumPrograms);
}

VMPCWDL::~VMPCWDL()
{
	delete mpc;
}

void VMPCWDL::NoteOnOff(IMidiMsg* pMsg)
{
  int v;

  int status = pMsg->StatusMsg();
  int velocity = pMsg->Velocity();
  int note = pMsg->NoteNumber();

  if (status == IMidiMsg::kNoteOn && velocity) // Note on
  {

  }
  else  // Note off
  {
  }
}

void VMPCWDL::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	auto urserver = mpc->getAudioMidiServices().lock()->getUnrealAudioServer();
	urserver->work(outputs, nFrames);
}

void VMPCWDL::Reset()
{
  TRACE;
  IMutexLock lock(this);

  mSampleRate = GetSampleRate();
  mMidiQueue.Resize(GetBlockSize());
}

void VMPCWDL::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    default:
      break;
  }
}

void VMPCWDL::ProcessMidiMsg(IMidiMsg* pMsg)
{
  int status = pMsg->StatusMsg();
  int velocity = pMsg->Velocity();
  
  switch (status)
  {
    case IMidiMsg::kNoteOn:
    case IMidiMsg::kNoteOff:
      // filter only note messages
      if (status == IMidiMsg::kNoteOn && velocity)
      {
        //mKeyStatus[pMsg->NoteNumber()] = true;
        //mNumHeldKeys += 1;
      }
      else
      {
        //mKeyStatus[pMsg->NoteNumber()] = false;
        //mNumHeldKeys -= 1;
      }
      break;
    default:
      return; // if !note message, nothing gets added to the queue
  }
  

  //mKeyboard->SetDirty();
  mMidiQueue.Add(pMsg);
}

//Called by the standalone wrapper if someone clicks about
bool VMPCWDL::HostRequestingAboutBox()
{
  IMutexLock lock(this);
  if(GetGUI())
  {
    // get the IBitmapOverlay to show
    //mAboutBox->SetValueFromPlug(1.);
    //mAboutBox->Hide(false);
  }
  return true;
}
