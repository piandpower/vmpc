#include "VMPCWDL.h"
#include "IPlug_include_in_plug_src.h"
#include "resource.h"

#include "IControl.h"

#include "source/DataWheelControl.hpp"
#include "source/KnobControl.hpp"
#include "source/LedControl.hpp"
#include "source/LCDControl.hpp"
#include "source/InputCatcherControl.hpp"
#include "source/PadControl.hpp"
#include "source/SliderControl.hpp"
#include "source/ButtonControl.hpp"

//#include "source/FTControl.hpp"

// mpc
#include <sequencer/Sequencer.hpp>
#include <sequencer/Sequence.hpp>

#include <ui/midisync/MidiSyncGui.hpp>

#include <audiomidi/MpcMidiPorts.hpp>
#include <audiomidi/MpcMidiInput.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <audio/server/RtAudioServer.hpp>
#include <audio/server/NonRealTimeAudioServer.hpp>
#include <hardware/Hardware.hpp>
#include <hardware/DataWheel.hpp>
#include <hardware/Led.hpp>
#include <hardware/Pot.hpp>
#include <hardware/HwSlider.hpp>

// ctoot
#include <midi/core/ShortMessage.hpp>

const int kNumPrograms = 8;
const int kNumParams = 1;

enum ELayout
{
	kWidth = GUI_WIDTH,
	kHeight = GUI_HEIGHT,
};

// Adding names for views. Default view is needed, place it on top
enum viewSets
{
	defaultView, // Default view will always be at 0

};

void VMPCWDL::SetGUILayout(int viewMode, double windowWidth, double windowHeight)
{
	// Use this function to move, hide, show and resize controls. windowWidth and windowHeight are not affected by GUI scaling

	// Every view will have it's own gui layout, so if you for example hide some control on miniView you don't
	// need to unhide it in defaultView because layout is separate for every view

	if (viewMode == defaultView)
	{
		//GetGUIResize()->MoveControlHorizontally(*knobIndex, windowWidth - 200);
		//GetGUIResize()->MoveControlHorizontally(*mDataWheel_index, windowWidth - 920);
		//GetGUIResize()->MoveControlVertically(*mDataWheel_index, windowHeight - 579);
	}
}

VMPCWDL::VMPCWDL(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumParams, kNumPrograms, instanceInfo),
	mSampleRate(44100.)
{   
	mpc = new mpc::Mpc();
	mpc->init("rtaudio", mSampleRate);
	MLOG("mpc initialized");
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT);
	//auto guiResize = new IPlugGUIResize(this, pGraphics, true, 25, 25);
	//AttachGUIResize(guiResize);
  
	//GetGUIResize()->UsingBitmaps();
	//GetGUIResize()->SelectViewMode(defaultView);
	//GetGUIResize()->SetGUIScaleLimits(50, 100);
	//GetGUIResize()->SetWindowSizeLimits(defaultView, 1298/2, 994/2, 1298, 994);

	pGraphics->AttachBackground(BG_ID, BG_FN);
	mLedPanel = new LedControl(this, pGraphics);
	for (auto& l : mpc->getHardware().lock()->getLeds()) {
		l->addObserver(mLedPanel);
	}
	pGraphics->AttachControl(mLedPanel);

	mInputCatcher = new InputCatcherControl(this, mpc);
	pGraphics->AttachKeyCatcher(mInputCatcher);

	auto dataWheels = pGraphics->LoadPointerToBitmap(DATAWHEEL_ID, DATAWHEEL_FN, 100);
	mDataWheel = new DataWheelControl(this, dataWheels, mpc->getHardware().lock()->getDataWheel(), mInputCatcher);

	mpc->getHardware().lock()->getDataWheel().lock()->addObserver(mDataWheel);
	mDataWheel_index = pGraphics->AttachControl(mDataWheel);

	auto knobs = pGraphics->LoadPointerToBitmap(RECKNOB_ID, RECKNOB_FN, 100);
	mRecKnob = new KnobControl(this, 0, knobs, mpc->getHardware().lock()->getRecPot(), mpc->getAudioMidiServices().lock()->getRecordLevel());
	knobs = pGraphics->LoadPointerToBitmap(VOLKNOB_ID, VOLKNOB_FN, 100);
	mVolKnob = new KnobControl(this, 1, knobs, mpc->getHardware().lock()->getVolPot(), mpc->getAudioMidiServices().lock()->getMasterLevel());
	pGraphics->AttachControl(mRecKnob);
	pGraphics->AttachControl(mVolKnob);

	const int padWidth = 96;
	int padSpacing = 25;
	const int padOffsetX = 778;
	const int padOffsetY = 397;
	int padCounter = 0;
	auto padhit = pGraphics->LoadPointerToBitmap(PADHIT_ID, PADHIT_FN);
	for (int j = 3; j >= 0; j--) {
		for (int i = 0; i < 4; i++) {
			int x1 = (padWidth + padSpacing) * i + padOffsetX + i;
			int x2 = x1 + padWidth + i;
			int y1 = (padWidth + padSpacing) * j + padOffsetY;
			int y2 = y1 + padWidth;
			IRECT rect(x1, y1, x2, y2);
			auto pc = new PadControl(this, rect, mpc->getHardware().lock()->getPad(padCounter++), padhit);
			pGraphics->AttachControl(pc);
		}
	}

	auto sliders = pGraphics->LoadPointerToBitmap(SLIDER_ID, SLIDER_FN);
	sliders->N = 100;
	auto sc = new SliderControl(this, sliders, mpc->getHardware().lock()->getSlider(), 0, mInputCatcher);
	pGraphics->AttachControl(sc);

	ButtonControl::initRects();
	std::vector<std::string> buttonLabels{ "rec", "overdub", "stop", "play", "playstart", "mainscreen", "prevstepevent", "nextstepevent",	"goto",	"prevbarstart",	"nextbarend", "tap", "nextseq",	"trackmute", "openwindow", "fulllevel", "sixteenlevels", "f1", "f2", "f3", "f4", "f5", "f6", "shift", "enter", "undoseq", "erase", "after", "banka", "bankb", "bankc", "bankd" };
	for (auto& l : buttonLabels) {
		auto bc = new ButtonControl(this, pGraphics, *ButtonControl::rects[l], mpc->getHardware().lock()->getButton(l));
		pGraphics->AttachControl(bc);
	}
	//pGraphics->HandleMouseOver(true);
	mLCDControl = new LCDControl(this, mpc->getLayeredScreen());
	pGraphics->AttachControl(mLCDControl);
	//GetGUIResize()->UseHandleForGUIScaling(true);
	AttachGraphics(pGraphics);
	//MakePreset("preset 1", ... );
	MakeDefaultPreset((char *) "-", kNumPrograms);

	mpc->powerOn();
}

VMPCWDL::~VMPCWDL()
{
	//delete mpc;
}

void VMPCWDL::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames, int outputChannels)
{
	if (mpc->getAudioMidiServices().lock()->isDisabled()) return;
	auto offlineServer = mpc->getAudioMidiServices().lock()->getOfflineServer();
	if (!offlineServer->isRealTime()) return;
	auto server = mpc->getAudioMidiServices().lock()->getRtAudioServer();

	auto midiOutMsgQueues = mpc->getMidiPorts().lock()->getReceivers();

	for (auto& queue : *midiOutMsgQueues) {
		for (auto msg : queue) {
			auto velo = msg.getData2();
			if (velo == 0) continue;
			IMidiMsg imsg;
			imsg.MakeNoteOnMsg(msg.getData1(), velo, 0, msg.getChannel());
			imsg.mStatus = msg.getStatus();
			SendMidiMsg(&imsg);
		}
		for (auto msg : queue) {
			auto velo = msg.getData2();
			if (velo != 0) continue;
			IMidiMsg imsg;
			imsg.MakeNoteOffMsg(msg.getData1(), 0, msg.getChannel());
			imsg.mStatus = msg.getStatus();
			SendMidiMsg(&imsg);
		}
		queue.clear();
	}

	auto msGui = mpc->getUis().lock()->getMidiSyncGui();
	bool syncEnabled = msGui->getModeIn() == 1;
	
	if (syncEnabled) {
		const double tempo = GetTempo();
		if (tempo != m_Tempo || mpc->getSequencer().lock()->getTempo().toDouble() != tempo) {
			mpc->getSequencer().lock()->setTempo(BCMath(tempo));
			m_Tempo = tempo;
		}

		ITimeInfo ti;
		GetTime(&ti);

		bool isPlaying = ti.mTransportIsRunning;

		if (!m_WasPlaying && isPlaying)
		{
			mpc->getSequencer().lock()->playFromStart();
		}
		if (m_WasPlaying && !isPlaying) {
			mpc->getSequencer().lock()->stop();
		}
		m_WasPlaying = isPlaying;
	}

	server->work(inputs, outputs, nFrames, outputChannels);
}

void VMPCWDL::Reset()
{
  TRACE;
  IMutexLock lock(this);

  mSampleRate = GetSampleRate();
  auto server = mpc->getAudioMidiServices().lock()->getRtAudioServer();
  if (GetSampleRate() != server->getSampleRate()) {
	  MLOG("Changing sample rate to " + to_string(GetSampleRate()));
	  mpc->getAudioMidiServices().lock()->destroyServices();
	  mpc->getAudioMidiServices().lock()->start("rtaudio", GetSampleRate());
  }
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
      if (status == IMidiMsg::kNoteOn && velocity)
      {
		  auto tootMsg = ctoot::midi::core::ShortMessage();
		  auto data = std::vector<char>{ (char)ctoot::midi::core::ShortMessage::NOTE_ON, (char)(pMsg->mData1), (char)(velocity) };
		  tootMsg.setMessage(data, 3);
		 mpc->getMpcMidiInput(0)->transport(&tootMsg, 0);
      }
      else
      {
		  auto tootMsg = ctoot::midi::core::ShortMessage();
		  auto data = std::vector<char>{ (char)ctoot::midi::core::ShortMessage::NOTE_OFF, (char)(pMsg->mData1), (char)(velocity) };
		  tootMsg.setMessage(data, 3);
		 mpc->getMpcMidiInput(0)->transport(&tootMsg, 0);
      }
      break;
	default:
		return;
  }
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
