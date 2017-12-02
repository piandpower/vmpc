#include <file/all/AllSequencer.hpp>

#include <Mpc.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <sequencer/Sequencer.hpp>

using namespace mpc::file::all;
using namespace std;

mpc::file::all::Sequencer::Sequencer(vector<char> loadBytes)
{
	sequence = loadBytes[SEQ_OFFSET];
	track = loadBytes[TR_OFFSET];
	tc = loadBytes[TC_OFFSET];
	secondSeqEnabled = loadBytes[SECOND_SEQ_ENABLED_OFFSET] > 0;
	secondSeqIndex = loadBytes[SECOND_SEQ_INDEX_OFFSET];
}

mpc::file::all::Sequencer::Sequencer(mpc::Mpc* mpc)
{
	saveBytes = vector<char>(LENGTH);
	for (int i = 0; i < LENGTH; i++)
		saveBytes[i] = TEMPLATE[i];
	auto seq = mpc->getSequencer().lock();
	saveBytes[SEQ_OFFSET] = seq->getActiveSequenceIndex();
	saveBytes[TR_OFFSET] = seq->getActiveTrackIndex();
	saveBytes[TC_OFFSET] = mpc->getUis().lock()->getSequencerWindowGui()->getNoteValue();
	saveBytes[SECOND_SEQ_ENABLED_OFFSET] = seq->isSecondSequenceEnabled() ? 1 : 0;
	saveBytes[SECOND_SEQ_INDEX_OFFSET] = seq->getSecondSequenceIndex();
}

const int mpc::file::all::Sequencer::LENGTH;
vector<char> mpc::file::all::Sequencer::TEMPLATE = vector<char>{ 0, 0, 0, 0 , (char) 176, 4, 1, 3, 0, 0, 0, 0, 12, 0, 0, 0 };
const int mpc::file::all::Sequencer::SEQ_OFFSET;
const int mpc::file::all::Sequencer::TR_OFFSET;
const int mpc::file::all::Sequencer::TC_OFFSET;
const int mpc::file::all::Sequencer::SECOND_SEQ_ENABLED_OFFSET;
const int mpc::file::all::Sequencer::SECOND_SEQ_INDEX_OFFSET;

vector<char> mpc::file::all::Sequencer::getBytes()
{
    return saveBytes;
}
