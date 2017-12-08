#include <midi/event/meta/SequencerSpecificEvent.hpp>

#include <midi/event/MidiEvent.hpp>
#include <midi/event/meta/MetaEvent.hpp>
#include <midi/util/MidiUtil.hpp>
#include <midi/util/VariableLengthInt.hpp>

using namespace mpc::midi::event::meta;

SequencerSpecificEvent::SequencerSpecificEvent(int tick, int delta, std::vector<char> data) : MetaEvent(tick, delta, MetaEvent::SEQUENCER_SPECIFIC, new mpc::midi::util::VariableLengthInt(data.size()))
{
	mData = data;
}

void SequencerSpecificEvent::setData(std::vector<char> data)
{
	mData = data;
	mLength->setValue(mData.size());
}

std::vector<char> SequencerSpecificEvent::getData()
{
    return mData;
}

int SequencerSpecificEvent::getEventSize()
{
	return 1 + 1 + mLength->getByteCount() + mData.size();
}

void SequencerSpecificEvent::writeToFile(moduru::io::OutputStream* out)
{
	MetaEvent::writeToFile(out);
	out->write(mLength->getBytes());
	out->write(mData);
}

int SequencerSpecificEvent::compareTo(mpc::midi::event::MidiEvent* other)
{
	if (mTick != other->getTick()) {
		return mTick < other->getTick() ? -1 : 1;
	}
	if (mDelta->getValue() != other->getDelta()) {
		return mDelta->getValue() < other->getDelta() ? 1 : -1;
	}
	if (!(dynamic_cast<SequencerSpecificEvent*>(other) != nullptr)) {
		return 1;
	}
	auto o = dynamic_cast<SequencerSpecificEvent*>(other);
	if (mpc::midi::util::MidiUtil::bytesEqual(mData, o->mData, 0, mData.size())) {
		return 0;
	}
	return 1;
}

void SequencerSpecificEvent::writeToFile(moduru::io::OutputStream* out, bool writeType)
{
    MetaEvent::writeToFile(out, writeType);
}
