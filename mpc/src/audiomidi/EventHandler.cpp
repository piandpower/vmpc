#include <audiomidi/EventHandler.hpp>

#include <Mpc.hpp>
#include <audiomidi/AudioMidiServices.hpp>
//#include <audiomidi/MpcMidiPorts.hpp>

#include <sequencer/Event.hpp>
#include <sequencer/FrameSeq.hpp>
#include <sequencer/MidiClockEvent.hpp>
#include <sequencer/MixerEvent.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/NoteEvent.hpp>
#include <sequencer/Sequencer.hpp>
#include <sequencer/TempoChangeEvent.hpp>

#include <ui/Uis.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <ui/midisync/MidiSyncGui.hpp>
//#include <ui/misc/TransGui.hpp>
//#include <ui/vmpc/DirectToDiskRecorderGui.hpp>
//#include <ui/vmpc/MidiGui.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/MixerChannel.hpp>
#include <ctootextensions/MpcMultiMidiSynth.hpp>
#include <thirdp/bcmath/bcmath_stl.h>
//#include <midi/InvalidMidiDataException.hpp>
#include <midi/core/MidiMessage.hpp>
#include <midi/core/ShortMessage.hpp>
#include <midi/core/MidiInput.hpp>

#include <file/File.hpp>

using namespace mpc::audiomidi;
using namespace std;

EventHandler::EventHandler(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	sequencer = mpc->getSequencer();
	sampler = mpc->getSampler();
	msGui = mpc->getUis().lock()->getMidiSyncGui();
	midiGui = mpc->getUis().lock()->getMidiGui();
	swGui = mpc->getUis().lock()->getSequencerWindowGui();
}

void EventHandler::handle(weak_ptr<mpc::sequencer::Event> event, mpc::sequencer::Track* track)
{
	auto ne = dynamic_pointer_cast<mpc::sequencer::NoteEvent>(event.lock());
	if (!track->isOn() && event.lock()->getTick() != -1) return;
	handleNoThru(event, track);
	midiOut(event, track);
}

void EventHandler::handleNoThru(weak_ptr<mpc::sequencer::Event> e, mpc::sequencer::Track* track)
{
	auto event = e.lock();

	auto lSequencer = sequencer.lock();

	if (track->getName().compare("click") == 0) {
		auto lSequencer = sequencer.lock();
		if (!lSequencer->isCountEnabled()) return;
		if (lSequencer->isRecordingOrOverdubbing() && !swGui->getInRec() && !lSequencer->isCountingIn()) return;
		if (lSequencer->isPlaying() && !lSequencer->isRecordingOrOverdubbing() && !swGui->getInPlay() && !lSequencer->isCountingIn()) return;

		auto ne = dynamic_pointer_cast<mpc::sequencer::NoteEvent>(event);

		if (ne->getVelocity() == 0) {
			return;
		}

		auto fs = mpc->getAudioMidiServices().lock()->getFrameSequencer().lock();
		auto eventFrame = fs->getEventFrameOffset(event->getTick());
		sampler.lock()->playMetronome(ne.get(), eventFrame);
		return;
	}
	else {
		if (lSequencer->isCountingIn() && event->getTick() != -1) {
			return;
		}
	}
	auto tce = dynamic_pointer_cast<mpc::sequencer::TempoChangeEvent>(event);
	auto mce = dynamic_pointer_cast<mpc::sequencer::MidiClockEvent>(event);
	auto ne = dynamic_pointer_cast<mpc::sequencer::NoteEvent>(event);
	auto me = dynamic_pointer_cast<mpc::sequencer::MixerEvent>(event);

	if (tce && tce->getTempo().toDouble() != lSequencer->getTempo().toDouble()) {
		lSequencer->setTempo(tce->getTempo());
		return;
	}
	else if (mce) {
		//auto mpcMidiPorts = mpc->getMidiPorts().lock();
		auto clockMsg = dynamic_cast<ctoot::midi::core::ShortMessage*>(mce->getShortMessage());
		clockMsg->setMessage(mce->getStatus());
		switch (msGui->getOut()) {
		case 0:
			//mpcMidiPorts->transmitA(clockMsg);
			break;
		case 1:
			//mpcMidiPorts->transmitB(clockMsg);
			break;
		case 2:
			//mpcMidiPorts->transmitA(clockMsg);
			//mpcMidiPorts->transmitB(clockMsg);
			break;
		}
	}
	else if (ne) {
		auto busNumber = track->getBusNumber();
		if (busNumber != 0) {
			auto drum = busNumber - 1;
			if (ne->getDuration() != -1) {
				if (!(lSequencer->isSoloEnabled() && track->getTrackIndex() != lSequencer->getActiveTrackIndex())) {
					auto newVelo = static_cast<int>(ne->getVelocity() * (track->getVelocityRatio() / 100.0));
					midiAdapter.process(ne, drum, newVelo);
					auto eventFrame = mpc->getAudioMidiServices().lock()->getFrameSequencer().lock()->getEventFrameOffset(event->getTick());
					mpc->getMms()->mpcTransport(track->getTrackIndex(), midiAdapter.get().lock().get(), 0, ne->getVariationTypeNumber(), ne->getVariationValue(), eventFrame);
				}
			}
		}
	}
	else if (me) {
		auto pad = me->getPad();
		auto lSampler = sampler.lock();
		auto p = lSampler->getProgram(lSampler->getDrumBusProgramNumber(track->getBusNumber())).lock();
		auto mixer = p->getMixerChannel(pad).lock();
		if (me->getParameter() == 0) {
			mixer->setLevel(me->getValue());
		}
		else if (me->getParameter() == 1) {
			mixer->setPanning(me->getValue());
		}
	}
}

void EventHandler::midiOut(weak_ptr<mpc::sequencer::Event> e, mpc::sequencer::Track* track)
{
	/*
		auto event = e.lock();
	auto ne = dynamic_pointer_cast<mpc::sequencer::NoteEvent>(event);
	if (ne) {
		if (lGui->getTransGui()->getTr() == -1 || lGui->getTransGui()->getTr() == ne->getTrack()) {
			ne->setNote(ne->getNote() + lGui->getTransGui()->getAmount());
		}
		auto msg = event->getShortMessage();
		auto deviceNumber = track->getDevice() - 1;
		if (deviceNumber != -1 && deviceNumber < 32) {
			auto channel = deviceNumber;
			if (channel > 15) channel -= 16;

			midiAdapter.process(event, channel, -1);
			msg = midiAdapter.get().lock().get();
		}
		auto mpcMidiPorts = mpc->getMidiPorts().lock();
		ctoot::midi::core::MidiInput* r = nullptr;
		r = (midiGui->getOutAReceiverIndex() == -1 || mpcMidiPorts->getReceivers().size() == 0) ? nullptr : mpcMidiPorts->getReceivers()[midiGui->getOutAReceiverIndex()];
		auto notifyLetter = "a";
		if (deviceNumber > 15) {
			deviceNumber -= 16;
			r = midiGui->getOutBReceiverIndex() == -1 ? nullptr : mpcMidiPorts->getReceivers()[midiGui->getOutBReceiverIndex()];
			notifyLetter = "b";
		}
		if (!(mpc->getAudioMidiServices().lock()->isBouncing() && lGui->getD2DRecorderGui()->isOffline()) && r != nullptr && track->getDevice() != 0 && msg != nullptr) {
			r->transport(msg, -1);
		}

		if (lGui->getMainFrame().lock()->getLayeredScreen().lock()->getCurrentScreenName().compare("midioutputmonitor") == 0) {
		setChanged();
		notifyObservers(string(notifyLetter + to_string(deviceNumber)));
		}
	}
	*/
}
