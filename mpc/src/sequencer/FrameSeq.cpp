#include "FrameSeq.hpp"

#include <Mpc.hpp>
#include <controls/Controls.hpp>
#include <controls/AbstractControls.hpp>

#include <sequencer/Sequencer.hpp>
#include <sequencer/Clock.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Song.hpp>
#include <sequencer/Step.hpp>
#include <sequencer/Track.hpp>

#include <sequencer/SeqUtil.hpp>

//#include <ui/sequencer/SongGui.hpp>
//#include <ui/sequencer/window/SequencerWindowGui.hpp>
//#include <controls/AbstractControls.hpp>

using namespace mpc::sequencer;
using namespace std;

FrameSeq::FrameSeq(mpc::Mpc* mpc) {
	this->mpc = mpc;
	sequencer = mpc->getSequencer();
}

//void FrameSeq::setGui(mpc::Mpc* mpc) {
//}

void FrameSeq::start() {
	if (running) {
		return;
	}
	clock.init(44100.0);
	clock.setTick(sequencer.lock()->getPlayStartTick());
	running = true;
}

void FrameSeq::startMetronome() {
	if (running) {
		return;
	}
	metronome = true;
	start();
}

void FrameSeq::work(int nFrames) {
	if (!running) {
		return;
	}
	auto lSequencer = sequencer.lock();
	if (!lSequencer->isCountingIn()) lSequencer->notifyTimeDisplay();

	auto seq = lSequencer->getCurrentlyPlayingSequence().lock();
	double tempo = lSequencer->getTempo().toDouble();
	if (tempo != clock.getBpm()) {
		clock.set_bpm(tempo);
		lSequencer->notify("tempo");
	}

	for (int i = 0; i < nFrames; i++) {

		if (clock.proc()) {
			tickFrameOffset = i;

			/*
			if (kbmc->tapIsPressed()) {
			int tcValue = lSequencer->getTickValues().at(lSequencer->getTcIndex());
			int swingPercentage = lGui->getSequencerWindowGui()->getSwing();
			if (tcValue == 24 || tcValue == 48) {
			int swingOffset = (int)((swingPercentage - 50) * (4.0 / 100.0) * (tcValue / 2.0));
			if (getTickPosition() % (tcValue * 2) == swingOffset + tcValue) {
			repeatPad(getTickPosition());
			}
			else if (getTickPosition() % (tcValue * 2) == 0) {
			repeatPad(getTickPosition());
			}
			}
			else {
			if (getTickPosition() % tcValue == 0) {
			repeatPad(getTickPosition());
			}
			}
			}
			*/

			if (!metronome) {
				if (lSequencer->isCountingIn()) {
					int start = seq->getBarLengths()->at(seq->getFirstLoopBar());

					if (getTickPosition() >= start - 1) {
						lSequencer->playToTick(start - 1);
						move(seq->isLoopEnabled() ? seq->getLoopStart() : 0);
						lSequencer->setCountingIn(false);
					}
					else {
						lSequencer->playToTick(getTickPosition());
						continue;
					}
				}

				if (getTickPosition() >= seq->getLastTick() - 1 && !lSequencer->isSongModeEnabled() && lSequencer->getNextSq() != -1) {
					//lSequencer->playToTick(getTickPosition());
					checkNextSq();
					//move(0);
					seq = lSequencer->getCurrentlyPlayingSequence().lock();
					seq->initLoop();
					move(0);
					//continue;
				}
				/*
				else if (lSequencer->isSongModeEnabled()) {
					auto songGui = lGui->getSongGui();
					if (getTickPosition() >= seq->getLastTick() - 1) {
						Sequencer::repeats++;
						auto song = lSequencer->getSong(songGui->getSelectedSongIndex()).lock();
						auto step = lGui->getSongGui()->getOffset() + 1;
						if (step == song->getStepAmount() - 1 && Sequencer::repeats == song->getStep(step)->getRepeats()) {
							if (!songGui->isLoopEnabled()) {
								lSequencer->playToTick(seq->getLastTick() - 1);
								Sequencer::endOfSong = true;
								lSequencer->stop();
								lSequencer->move(seq->getLastTick());
								continue;
							}
							else {
								lSequencer->playToTick(seq->getLastTick() - 1);
								songGui->setOffset(-1);
								move(0);
							}
						}
						else {
							lSequencer->playToTick(seq->getLastTick() - 1);
							if (Sequencer::repeats == song->getStep(step)->getRepeats()) {
								Sequencer::repeats = 0;
								songGui->setOffset(songGui->getOffset() + 1);
							}
							move(0);
						}
					}
				}
				*/
				else if (seq->isLoopEnabled()) {
					if (getTickPosition() >= seq->getLoopEnd() - 1) {
						move(seq->getLoopStart());
						if (lSequencer->isRecordingOrOverdubbing()) {
							if (lSequencer->isRecording()) {
								lSequencer->switchRecordToOverDub();
							}
							if (lSequencer->isRecordingModeMulti()) {
								for (auto& t : seq->getTracks()) {
									t.lock()->removeDoubles();
								}
							}
							else {
								seq->getTrack(lSequencer->getActiveTrackIndex()).lock()->removeDoubles();
							}
						}
					}
				}
				else {
					if (getTickPosition() >= seq->getLastTick()) {
						lSequencer->stop(seq->getLastTick());
						lSequencer->move(seq->getLastTick());
					}
				}
			}
			lSequencer->playToTick(getTickPosition());
		}
	}
}

int FrameSeq::getEventFrameOffset(int tick) {
	return tickFrameOffset;
}

void FrameSeq::stop() {
	if (!running) {
		return;
	}
	running = false;
	metronome = false;
	tickFrameOffset = 0;
}

bool FrameSeq::isRunning() {
	return running;
}

int FrameSeq::getTickPosition() {
	return clock.getTickPosition();
}

void FrameSeq::move(int newTickPos) {
	clock.setTick(newTickPos);
	sequencer.lock()->move(newTickPos);
}

void FrameSeq::repeatPad(int tick) {
	auto controls = mpc->getControls().lock();
	if (controls == nullptr) return;
	auto pp = controls->getPressedPads();
	for (auto& i : *pp)
		mpc->getActiveControls()->pad(i, (*controls->getPressedPadVelos())[i], true, tick);
}

void FrameSeq::checkNextSq() {
	auto lSeq = sequencer.lock();
	lSeq->setCurrentlyPlayingSequenceIndex(lSeq->getNextSq());
	lSeq->resetNextSq();
	lSeq->notify("nextsqoff");
	lSeq->notify("seqnumbername");
}

FrameSeq::~FrameSeq() {
}
