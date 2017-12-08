#include <midi/MidiTrack.hpp>

#include <midi/event/MidiEvent.hpp>
#include <midi/event/NoteOn.hpp>
#include <midi/event/meta/EndOfTrack.hpp>
#include <midi/event/meta/Tempo.hpp>
#include <midi/event/meta/TimeSignatureEvent.hpp>
#include <midi/util/MidiUtil.hpp>
#include <midi/util/VariableLengthInt.hpp>

#include <io/ByteArrayInputStream.hpp>

using namespace mpc::midi;
using namespace mpc::midi::event;
using namespace std;

MidiTrack::MidiTrack() 
{
	mSize = 0;
	mSizeNeedsRecalculating = false;
	mClosed = false;
	mEndOfTrackDelta = 0;
}

MidiTrack::MidiTrack(moduru::io::InputStream* in)
{
	mSize = 0;
	mSizeNeedsRecalculating = false;
	mClosed = false;
	mEndOfTrackDelta = 0;
	auto buffer = vector<char>(4);
	in->read(&buffer);
	if (!mpc::midi::util::MidiUtil::bytesEqual(buffer, IDENTIFIER, 0, 4)) {
		string error = "Track identifier did not match MTrk!";
		throw std::invalid_argument(error.c_str());
	}
	in->read(&buffer);
	mSize = mpc::midi::util::MidiUtil::bytesToInt(buffer, 0, 4);
	buffer.resize(mSize);
	in->read(&buffer);
	readTrackData(buffer);
}
const bool MidiTrack::VERBOSE;

vector<char> mpc::midi::MidiTrack::IDENTIFIER = vector<char>{ 'M', 'T', 'r', 'k' };

MidiTrack* MidiTrack::createTempoTrack()
{
	auto T = new MidiTrack();
	T->insertEvent(make_shared<meta::TimeSignature>());
	T->insertEvent(make_shared<meta::Tempo>());
	return T;
}

void MidiTrack::readTrackData(vector<char> data)
{
	auto in = moduru::io::ByteArrayInputStream(&data);
	int totalTicks = 0;
	while (in.available() > 0) {
		auto delta = mpc::midi::util::VariableLengthInt(&in);
		int value = delta.getValue();
		totalTicks += value;
		auto E = MidiEvent::parseEvent(totalTicks, value, &in);
		if (E == nullptr) {
			string error = "Event skipped!\n";
			continue;
		}
		if (dynamic_pointer_cast<meta::EndOfTrack>(E)) {
			mEndOfTrackDelta = E->getDelta();
			break;
		}
		mEvents.push_back(E);
	}
}

vector<weak_ptr<MidiEvent>> MidiTrack::getEvents()
{
	auto res = vector<weak_ptr<MidiEvent>>();
	for (auto& e : mEvents)
		res.push_back(e);
	return res;
}

int MidiTrack::getEventCount()
{
    return mEvents.size();
}

int MidiTrack::getSize()
{
    if(mSizeNeedsRecalculating) {
        recalculateSize();
    }
    return mSize;
}

int MidiTrack::getLengthInTicks()
{
	if (mEvents.size() == 0) {
		return 0;
	}
	auto E = *std::next(mEvents.begin(), (int) (mEvents.size()) - 1);
	return E->getTick();
}

int MidiTrack::getEndOfTrackDelta()
{
    return mEndOfTrackDelta;
}

void MidiTrack::setEndOfTrackDelta(int delta)
{
    mEndOfTrackDelta = delta;
}

void MidiTrack::insertNote(int channel, int pitch, int velocity, int tick, int duration)
{
	insertEvent(make_shared<NoteOn>(tick, channel, pitch, velocity));
	insertEvent(make_shared<NoteOn>(tick + duration, channel, pitch, 0));
}

void MidiTrack::insertEvent(std::weak_ptr<event::MidiEvent> newE)
{
	auto newEvent = newE.lock();
	if (!newEvent) {
		return;
	}
	if (mClosed) {
		//npc(::java::lang::System::err())->println(u"Error: Cannot add an event to a closed track."_j);
		return;
	}
	shared_ptr<MidiEvent> prev;
	shared_ptr<MidiEvent> next;
	if (mEvents.size() > 0) prev = mEvents.back();
	try {
		//auto treeSet = ::java::lang::Class::forName(u"java.util.TreeSet"_j);
		//auto floor = treeSet->getMethod(u"floor"_j, new ::java::lang::ClassArray({static_cast< ::java::lang::Class* >(::java::lang::Object::class_())}));
		//auto ceiling = treeSet->getMethod(u"ceiling"_j, new ::java::lang::ClassArray({static_cast< ::java::lang::Class* >(::java::lang::Object::class_())}));
		//prev = dynamic_cast< ::event::MidiEvent* >(npc(floor)->invoke(mEvents, new ::java::lang::ObjectArray({static_cast< ::java::lang::Object* >(newEvent)})));
		//next = dynamic_cast< ::event::MidiEvent* >(npc(ceiling)->invoke(mEvents, new ::java::lang::ObjectArray({static_cast< ::java::lang::Object* >(newEvent)})));

	}
	catch (exception* e) {
		for (auto& n : mEvents) {
			next = n;
			if (next->getTick() > newEvent->getTick()) {
				break;
			}
			prev = next;
		}
	}
	mEvents.push_back(newEvent);
	mSizeNeedsRecalculating = true;
	if (prev) {
		newEvent->setDelta(newEvent->getTick() - prev->getTick());
	}
	else {
		newEvent->setDelta(newEvent->getTick());
	}
	if (next) {
		next->setDelta(next->getTick() - newEvent->getTick());
	}
	mSize += newEvent->getSize();
	if (dynamic_pointer_cast<meta::EndOfTrack>(newEvent)) {
		if (next) {
			throw std::invalid_argument("Attempting to insert EndOfTrack before an existing event. Use closeTrack() when finished with MidiTrack.");
		}
		mClosed = true;
	}
}

bool MidiTrack::removeEvent(::event::MidiEvent* E)
{
	shared_ptr<MidiEvent> prev;
	shared_ptr<MidiEvent> curr;
	shared_ptr<MidiEvent> next;
	for (auto& n : mEvents) {
		next = n;
		if (E == curr.get()) {
			break;
		}
		prev = curr;
		curr = next;
		next = nullptr;
	}
	if (!next) {
		//return mEvents.erase(curr);
		return false;
	}
	//if (!mEvents.erase(curr)) {
//		return false;
	//}
	if (prev != nullptr) {
		next->setDelta(next->getTick() - prev->getTick());
	}
	else {
		next->setDelta(next->getTick());
	}
	return true;
}

void MidiTrack::closeTrack()
{
	int lastTick = 0;
	if (mEvents.size() > 0) {
		auto last = dynamic_pointer_cast<MidiEvent>(*std::next(mEvents.begin(), (int)(mEvents.size()) - 1));
		lastTick = last->getTick();
	}
	insertEvent(make_shared<meta::EndOfTrack>(lastTick + mEndOfTrackDelta, 0));
}

void MidiTrack::dumpEvents()
{
    //auto it = mEvents->iterator();
    //while (npc(it)->hasNext()) {
        //npc(::java::lang::System::out())->println(static_cast< ::java::lang::Object* >(dynamic_cast< ::event::MidiEvent* >(npc(it)->next())));
    //}
}

void MidiTrack::recalculateSize()
{
	mSize = 0;
	shared_ptr<MidiEvent> last;
	for (auto& E : mEvents) {
		string str = typeid(*E.get()).name();
		mSize += E->getSize();
		if (last && !E->requiresStatusByte(last.get())) {
			mSize--;
		}
		last = E;
	}
	mSizeNeedsRecalculating = false;
}

void MidiTrack::writeToFile(moduru::io::OutputStream* out)
{
	if (!mClosed) {
		closeTrack();
	}
	if (mSizeNeedsRecalculating) {
		recalculateSize();
	}
	out->write(IDENTIFIER);
	out->write(::util::MidiUtil::intToBytes(mSize, 4));
	//auto it = mEvents->iterator();
	shared_ptr<MidiEvent> lastEvent;
	for (auto& event : mEvents) {
		if (VERBOSE) {
			//"Writing: "_j)->append(event)->toString());
		}
		event->writeToFile(out, event->requiresStatusByte(lastEvent.get()));
		lastEvent = event;
	}
}
