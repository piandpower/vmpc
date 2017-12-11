#include <controls/mixer/MixerControls.hpp>

#include <ui/sampler/MixerGui.hpp>
#include <ui/sampler/MixerSetupGui.hpp>
#include <sampler/MixerChannel.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>
#include <sequencer/MixerEvent.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/Sequencer.hpp>
#include <sampler/MixerChannel.hpp>

using namespace mpc::controls::mixer;
using namespace std;

MixerControls::MixerControls(mpc::Mpc* mpc) 
	: AbstractMixerControls(mpc)
{
}

void MixerControls::up()
{
    init();
	mixerGui->setYPos(mixerGui->getYPos() - 1);
}

void MixerControls::down()
{
    init();
	mixerGui->setYPos(mixerGui->getYPos() + 1);
}

void MixerControls::left()
{
    init();
	mixerGui->setXPos(mixerGui->getXPos() - 1);
}

void MixerControls::right()
{
    init();
	mixerGui->setXPos(mixerGui->getXPos() + 1);
}

void MixerControls::openWindow()
{
	init();
	mixerGui->setChannelSettingsNote(program.lock()->getPad(mixerGui->getXPos() + (bank_ * 16))->getNote());
	ls.lock()->openScreen("channelsettings");
}

void MixerControls::function(int f)
{
	init();
	switch (f) {
	case 0:
	case 1:
	case 2:
		mixerGui->setTab(f);
		ls.lock()->getCurrentBackground()->SetDirty();
		break;
	case 3:
		ls.lock()->openScreen("mixersetup");
		break;
	case 5:
		mixerGui->setLink(!mixerGui->getLink());
		break;
	}
}

void MixerControls::turnWheel(int i)
{
	init();
	int pad = mixerGui->getXPos() + (bank_ * 16);
	auto lProgram = program.lock();
	auto m = lProgram->getPad(pad)->getMixerChannel().lock();
	auto ma = vector<weak_ptr<mpc::sampler::MixerChannel>>(16);
	for (int i = 0; i < 16; i++) {
		ma[i] = lProgram->getPad(i + (bank_ * 16))->getMixerChannel();
	}

	if (mixerGui->getTab() == 0) {
		bool record = sequencer.lock()->isRecordingOrOverdubbing() && mixerSetupGui->isRecordMixChangesEnabled();
		if (mixerGui->getYPos() == 0) {
			if (!mixerGui->getLink()) {
				m->setPanning(m->getPanning() + i);
				if (record) {
					recordMixerEvent(pad, 1, m->getPanning());
				}
			}
			else {
				int padCounter = 0;
				for (auto mcTemp : ma) {
					auto lMcTemp = mcTemp.lock();
					lMcTemp->setPanning(lMcTemp->getPanning() + i);
					if (record) {
						recordMixerEvent(padCounter++, 1, lMcTemp->getPanning());
					}
				}
			}
		}
		else if (mixerGui->getYPos() == 1) {

			if (!mixerGui->getLink()) {
				m->setLevel(m->getLevel() + i);
				if (record) {
					recordMixerEvent(pad, 0, m->getLevel());
				}
			}
			else {
				auto padCounter = 0;
				for (auto mcTemp : ma) {
					auto lMcTemp = mcTemp.lock();
					lMcTemp->setLevel(lMcTemp->getLevel() + i);
					if (record) {
						recordMixerEvent(padCounter++, 0, lMcTemp->getLevel());
					}
				}
			}
		}
	}
	else if (mixerGui->getTab() == 1) {
		if (mixerGui->getYPos() == 0) {
			if (!mixerGui->getLink()) {
				m->setOutput(m->getOutput() + i);
				return;
			}
			else {
				for (auto mcTemp : ma) {
					auto lMcTemp = mcTemp.lock();
					lMcTemp->setOutput(lMcTemp->getOutput() + i);
				}
			}
		}
		else if (mixerGui->getYPos() == 1) {
			if (!mixerGui->getLink()) {
				m->setVolumeIndividualOut(m->getVolumeIndividualOut() + i);
			}
			else {
				for (auto mcTemp : ma) {
					auto lMcTemp = mcTemp.lock();
					lMcTemp->setVolumeIndividualOut(lMcTemp->getVolumeIndividualOut() + i);
				}
			}
		}
	}
	else if (mixerGui->getTab() == 2) {
		if (mixerGui->getYPos() == 0) {
			if (!mixerGui->getLink()) {
				m->setFxPath(m->getFxPath() + i);
			}
			else {
				for (auto mcTemp : ma) {
					auto lMcTemp = mcTemp.lock();
					lMcTemp->setFxPath(lMcTemp->getFxPath() + i);
				}
			}
		}
		else if (mixerGui->getYPos() == 1) {
			if (!mixerGui->getLink()) {
				m->setFxSendLevel(m->getFxSendLevel() + i);
			}
			else {
				for (auto mcTemp : ma) {
					auto lMcTemp = mcTemp.lock();
					lMcTemp->setFxSendLevel(lMcTemp->getFxSendLevel() + i);
				}
			}
		}
	}
}

void MixerControls::recordMixerEvent(int pad, int param, int value)
{
	auto lSequencer = sequencer.lock();
	auto track = lSequencer->getActiveSequence().lock()->getTrack(lSequencer->getActiveTrackIndex()).lock();
	auto e = dynamic_pointer_cast<mpc::sequencer::MixerEvent>(track->addEvent(lSequencer->getTickPosition(), "mixer").lock());
	e->setPadNumber(pad);
	e->setParameter(param);
	e->setValue(value);
}

MixerControls::~MixerControls() {
}
