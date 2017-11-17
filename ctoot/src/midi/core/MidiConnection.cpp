#include <midi/core/MidiConnection.hpp>

#include <midi/core/MidiInput.hpp>
#include <midi/core/MidiOutput.hpp>

using namespace ctoot::midi::core;

MidiConnection::MidiConnection(MidiOutput* from, MidiInput* to, int flags) 
{
	flags = 0;
	if (from == nullptr || to == nullptr) {
		//        throw new ::java::lang::IllegalArgumentException(u"MidiConnection constructor null argument"_j);
	}
	this->from = from;
	this->to = to;
	this->flags = flags;
	from->addConnectionTo(to);
}

const int MidiConnection::PLAYBACK;
const int MidiConnection::SYSTEM;
const int MidiConnection::OUTPUT_LOCKED;
const int MidiConnection::INPUT_LOCKED;

void MidiConnection::connectTo(MidiInput* to)
{
    if((flags & INPUT_LOCKED) != 0) {
//        throw new ::java::lang::IllegalStateException(u"MidiConnection input is locked"_j);
		return;
    }
    if(to == nullptr) {
//        throw new ::java::lang::IllegalArgumentException(u"MidiConnection can't connectTo(null)"_j);
		return;
    }
    from->removeConnectionTo(this->to);
    this->to = to;
    from->addConnectionTo(to);
}

void MidiConnection::connectFrom(MidiOutput* from)
{
    if((flags & OUTPUT_LOCKED) != 0) {
//        throw new ::java::lang::IllegalStateException(u"MidiConnection output is locked"_j);
		return;
    }
    if(from == nullptr) {
//        throw new ::java::lang::IllegalArgumentException(u"MidiConnection can't connectFrom(null)"_j);
		return;
    }
    this->from->removeConnectionTo(to);
    this->from = from;
    this->from->addConnectionTo(to);
}

void MidiConnection::close()
{
    from->removeConnectionTo(to);
    //from = nullptr;
    //to = nullptr;
}

MidiOutput* MidiConnection::getMidiOutput()
{
    return from;
}

MidiInput* MidiConnection::getMidiInput()
{
    return to;
}

bool MidiConnection::isSystem()
{
    return (flags & SYSTEM) != 0;
}

bool MidiConnection::isPlayback()
{
    return (flags & PLAYBACK) != 0;
}

MidiConnection::~MidiConnection() {
	//delete from;
	//from = nullptr;
	//delete to;
	//to = nullptr;
}
