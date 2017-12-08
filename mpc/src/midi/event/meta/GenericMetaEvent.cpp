#include <midi/event/meta/GenericMetaEvent.hpp>

#include <midi/event/MidiEvent.hpp>
#include <midi/event/meta/MetaEventData.hpp>
#include <midi/util/VariableLengthInt.hpp>

using namespace mpc::midi::event::meta;
using namespace std;

GenericMetaEvent::GenericMetaEvent(int tick, int delta, MetaEventData* info) 
	: MetaEvent(tick, delta, info->type, info->length)
{
	mData = info->data;
	/*
	npc(::java::lang::System::out())->println(::java::lang::StringBuilder().append(u"Warning: GenericMetaEvent used because type ("_j)->append(npc(info)->type)
		->append(u") wasn't recognized or unexpected data length ("_j)
		->append(npc(npc(info)->length)->getValue())
		->append(u") for type."_j)->toString());
		*/
}

int GenericMetaEvent::getEventSize()
{
	return 1 + 1 + mLength->getByteCount() + mLength->getValue();
}

void GenericMetaEvent::writeToFile(moduru::io::OutputStream* out)
{
	MetaEvent::writeToFile(out);
	out->write(mLength->getBytes());
	out->write(mData);
}

int GenericMetaEvent::compareTo(mpc::midi::event::MidiEvent* other)
{
	if (mTick != other->getTick()) {
		return mTick < other->getTick() ? -1 : 1;
	}
	if (mDelta->getValue() != other->getDelta()) {
		return mDelta->getValue() < other->getDelta() ? 1 : -1;
	}
	return 1;
}

void GenericMetaEvent::writeToFile(moduru::io::OutputStream* out, bool writeType)
{
    MetaEvent::writeToFile(out, writeType);
}
