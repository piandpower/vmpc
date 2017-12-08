#include <midi/event/meta/KeySignature.hpp>

#include <midi/event/MidiEvent.hpp>
#include <midi/event/meta/GenericMetaEvent.hpp>
#include <midi/event/meta/MetaEventData.hpp>
#include <midi/event/meta/MetaEvent.hpp>
#include <midi/util/VariableLengthInt.hpp>

using namespace mpc::midi::event::meta;
using namespace std;

KeySignature::KeySignature(int tick, int delta, int key, int scale) : MetaEvent(tick, delta, MetaEvent::KEY_SIGNATURE, new mpc::midi::util::VariableLengthInt(2))
{
	this->setKey(key);
	mScale = scale;
}

const int KeySignature::SCALE_MAJOR;
const int KeySignature::SCALE_MINOR;

void KeySignature::setKey(int key)
{
	mKey = static_cast<unsigned char>(key);
	if (mKey < -7)
		mKey = -7;
	else if (mKey > 7)
		mKey = 7;
}

int KeySignature::getKey()
{
    return mKey;
}

void KeySignature::setScale(int scale)
{
    mScale = scale;
}

int KeySignature::getScale()
{
    return mScale;
}

int KeySignature::getEventSize()
{
    return 5;
}

void KeySignature::writeToFile(moduru::io::OutputStream* out) 
{
    MetaEvent::writeToFile(out);
    out->write(2);
    out->write(mKey);
    out->write(mScale);
}

shared_ptr<MetaEvent> KeySignature::parseKeySignature(int tick, int delta, MetaEventData* info)
{
	if (info->length->getValue() != 2) {
		return make_shared<GenericMetaEvent>(tick, delta, info);
	}
	int key = info->data[0];
	int scale = info->data[1];
	return make_shared<KeySignature>(tick, delta, key, scale);
}

int KeySignature::compareTo(mpc::midi::event::MidiEvent* other)
{
    if(mTick != other->getTick()) {
        return mTick < other->getTick() ? -1 : 1;
    }
    if(mDelta->getValue() != other->getDelta()) {
        return mDelta->getValue() < other->getDelta() ? 1 : -1;
    }
    if(!(dynamic_cast< KeySignature* >(other) != nullptr)) {
        return 1;
    }
    auto o = dynamic_cast< KeySignature* >(other);
    if(mKey != o->mKey) {
        return mKey < o->mKey ? -1 : 1;
    }
    if(mScale != o->mScale) {
        return mKey < o->mScale ? -1 : 1;
    }
    return 0;
}

void KeySignature::writeToFile(moduru::io::OutputStream* out, bool writeType)
{
    MetaEvent::writeToFile(out, writeType);
}
