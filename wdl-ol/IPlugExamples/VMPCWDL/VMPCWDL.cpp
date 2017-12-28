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

VMPCWDL::VMPCWDL(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo),
	mSampleRate(44100.)
{   
	mpc = new mpc::Mpc();
	mpc->init("rtaudio");
	MLOG("mpc initialized");
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH * gui_scale, GUI_HEIGHT * gui_scale);
	pGraphics->AttachBackground(BG_ID, BG_FN);
	mLedPanel = new LedControl(this, pGraphics);
	for (auto& l : mpc->getHardware().lock()->getLeds()) {
		l->addObserver(mLedPanel);
	}
	pGraphics->AttachControl(mLedPanel);

	mInputCatcher = new InputCatcherControl(this, mpc);
	pGraphics->AttachControl(mInputCatcher);

	auto dataWheels = pGraphics->LoadIBitmap(DATAWHEEL_ID, DATAWHEEL_FN);
	mDataWheel = new DataWheelControl(this, dataWheels, mpc->getHardware().lock()->getDataWheel());

	mpc->getHardware().lock()->getDataWheel().lock()->addObserver(mDataWheel);
	pGraphics->AttachControl(mDataWheel);

	auto knobs = pGraphics->LoadIBitmap(RECKNOB_ID, RECKNOB_FN);
	mRecKnob = new KnobControl(this, 0, knobs, mpc->getHardware().lock()->getRecPot(), mpc->getAudioMidiServices().lock()->getRecordLevel());
	knobs = pGraphics->LoadIBitmap(VOLKNOB_ID, VOLKNOB_FN);
	mVolKnob = new KnobControl(this, 1, knobs, mpc->getHardware().lock()->getVolPot(), mpc->getAudioMidiServices().lock()->getMasterLevel());
	pGraphics->AttachControl(mRecKnob);
	pGraphics->AttachControl(mVolKnob);

	mLCDControl = new LCDControl(this, mpc->getLayeredScreen());
	pGraphics->AttachControl(mLCDControl);

	const int padWidth = 96;
	const int padSpacing = 25;
	const int padOffsetX = 777;
	const int padOffsetY = 398;
	int padCounter = 0;
	for (int j = 3; j >= 0; j--) {
		for (int i = 0; i < 4; i++) {
			int x1 = (padWidth + padSpacing) * i + padOffsetX;
			int x2 = x1 + padWidth;
			int y1 = (padWidth + padSpacing) * j + padOffsetY;
			int y2 = y1 + padWidth;
			IRECT rect(x1, y1, x2, y2);
			auto pc = new PadControl(this, rect, mpc->getHardware().lock()->getPad(padCounter++));
			pGraphics->AttachControl(pc);
		}
	}

	auto sliders = pGraphics->LoadIBitmap(SLIDER_ID, SLIDER_FN);
	auto sc = new SliderControl(this, sliders, mpc->getHardware().lock()->getSlider(), 0, mInputCatcher);
	pGraphics->AttachControl(sc);

	ButtonControl::initRects();
	std::vector<std::string> buttonLabels{ "rec", "overdub", "stop", "play", "playstart", "mainscreen", "prevstepevent", "nextstepevent",	"goto",	"prevbarstart",	"nextbarend", "tap", "nextseq",	"trackmute", "openwindow", "fulllevel", "sixteenlevels", "f1", "f2", "f3", "f4", "f5", "f6", "shift", "enter", "undoseq", "erase", "after", "banka", "bankb", "bankc", "bankd" };
	for (auto& l : buttonLabels) {
		auto bc = new ButtonControl(this, *ButtonControl::rects[l], mpc->getHardware().lock()->getButton(l));
		pGraphics->AttachControl(bc);
	}
	AttachGraphics(pGraphics);

	//MakePreset("preset 1", ... );
	MakeDefaultPreset((char *) "-", kNumPrograms);

	mpc->powerOn();
}

VMPCWDL::~VMPCWDL()
{
	delete mpc;
}

void VMPCWDL::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	auto offlineServer = mpc->getAudioMidiServices().lock()->getOfflineServer();
	if (!offlineServer->isRealTime()) return;
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
	auto server = mpc->getAudioMidiServices().lock()->getRtAudioServer();
	server->work(inputs, outputs, nFrames);
}

void VMPCWDL::Reset()
{
  TRACE;
  IMutexLock lock(this);

  mSampleRate = GetSampleRate();
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
