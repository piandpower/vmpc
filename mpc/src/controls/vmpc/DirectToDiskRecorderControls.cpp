#include <controls/vmpc/DirectToDiskRecorderControls.hpp>

#include <Mpc.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <audiomidi/DirectToDiskSettings.hpp>
#include <ui/NameGui.hpp>
#include <ui/sequencer/SongGui.hpp>
#include <ui/vmpc/DirectToDiskRecorderGui.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/SeqUtil.hpp>
#include <sequencer/Sequencer.hpp>
#include <sequencer/Song.hpp>
#include <audio/server/AudioServer.hpp>
#include <audio/server/NonRealTimeAudioServer.hpp>

using namespace mpc::controls::vmpc;
using namespace std;

DirectToDiskRecorderControls::DirectToDiskRecorderControls(mpc::Mpc* mpc)
	: AbstractVmpcControls(mpc)
{
}

void DirectToDiskRecorderControls::turnWheel(int i)
{
    init();
	checkAllTimes(i);
	if (param.compare("record") == 0) {
		d2dRecorderGui->setRecord(d2dRecorderGui->getRecord() + i);
	}
	else if (param.compare("sq") == 0) {
		d2dRecorderGui->setSq(d2dRecorderGui->getSq() + i);
	}
	else if (param.compare("song") == 0) {
		d2dRecorderGui->setSong(d2dRecorderGui->getSong() + i);
	}
	else if (param.compare("splitlr") == 0) {
		d2dRecorderGui->setSplitLR(i > 0);
	}
	else if (param.compare("offline") == 0) {
		d2dRecorderGui->setOffline(i > 0);
	}
    else if(param.compare("outputfolder") == 0) {
        nameGui->setName(d2dRecorderGui->getOutputfolder());
        nameGui->setNameLimit(8);
        nameGui->setParameterName("outputfolder");
        ls.lock()->openScreen("name");
    }
}

void DirectToDiskRecorderControls::function(int i)
{
	init();
	auto lAms = ams.lock();
	auto lSequencer = sequencer.lock();
	int seq;
	int lengthInFrames;
	string outputFolder = "";
	bool split;
	shared_ptr<mpc::sequencer::Sequence> sequence;
    unique_ptr<mpc::audiomidi::DirectToDiskSettings> settings;
	switch (i) {
	case 3:
		ls.lock()->openScreen("sequencer");
		break;
	case 4:
		seq = d2dRecorderGui->getSq();
		outputFolder = d2dRecorderGui->getOutputfolder();
		split = false;
		sequence = lSequencer->getSequence(seq).lock();
		bool offline = d2dRecorderGui->isOffline();
		auto sequenceRaw = sequence.get();
		auto lastTick = sequence->getLastTick();
		shared_ptr<mpc::sequencer::Song> song;
		switch (d2dRecorderGui->getRecord()) {
		case 0:
			ls.lock()->openScreen("sequencer");
			lengthInFrames = mpc::sequencer::SeqUtil::sequenceFrameLength(sequence.get(), 0, sequence->getLastTick());
            settings = make_unique<mpc::audiomidi::DirectToDiskSettings>(lengthInFrames, outputFolder, split);
			lAms->prepareBouncing(settings.get());
			sequence->setLoopEnabled(false);
			lSequencer->playFromStart();
			lAms->getOfflineServer()->setRealTime(!offline);
			break;
		case 1:
			ls.lock()->openScreen("sequencer");
			lengthInFrames = mpc::sequencer::SeqUtil::loopFrameLength(sequence.get());
            settings = make_unique<mpc::audiomidi::DirectToDiskSettings>(lengthInFrames, outputFolder, split);
			lAms->prepareBouncing(settings.get());
			sequence->setLoopEnabled(false);
			lSequencer->move(sequence->getLoopStart());
			lSequencer->play();
			lAms->getOfflineServer()->setRealTime(!offline);
			break;
		case 2:
			ls.lock()->openScreen("sequencer");
			lengthInFrames = mpc::sequencer::SeqUtil::sequenceFrameLength(sequence.get(), d2dRecorderGui->getTime0(), d2dRecorderGui->getTime1());
            settings = make_unique<mpc::audiomidi::DirectToDiskSettings>(lengthInFrames, outputFolder, split);
			lAms->prepareBouncing(settings.get());
			sequence->setLoopEnabled(false);
			lSequencer->move(d2dRecorderGui->getTime0());
			lSequencer->play();
			lAms->getOfflineServer()->setRealTime(!offline);
			break;
		case 3:
			song = lSequencer->getSong(d2dRecorderGui->getSong()).lock();
			if (!song->isUsed()) return;

			lengthInFrames = mpc::sequencer::SeqUtil::songFrameLength(song.get(), lSequencer.get());
            settings = make_unique<mpc::audiomidi::DirectToDiskSettings>(lengthInFrames, outputFolder, split);
			lAms->prepareBouncing(settings.get());
			ls.lock()->openScreen("song");
			lSequencer->setSongModeEnabled(true);
			lSequencer->playFromStart();
			lAms->getOfflineServer()->setRealTime(!offline);
			mpc->getUis().lock()->getSongGui()->setLoop(false);
			break;
		case 4:
			ls.lock()->openScreen("recordjam");
			break;
		}
		break;
	}
}
