#include <midi/message/ShortMsg.hpp>

//#include <midi/MidiEvent.hpp>
#include <midi/core/MidiMessage.hpp>
//#include <midi/MidiSystem.hpp>
//#include <midi/Sequence.hpp>
#include <midi/core/ShortMessage.hpp>
//#include <midi/Track.hpp>

using namespace ctoot::midi::message;

ShortMsg::ShortMsg()
{
	createFastPrototypes();
}

ctoot::midi::core::MidiMessage*& ShortMsg::fastShortPrototype()
{
    return fastShortPrototype_;
}
ctoot::midi::core::MidiMessage* ShortMsg::fastShortPrototype_;

bool ShortMsg::isShort(ctoot::midi::core::MidiMessage* msg)
{
    return dynamic_cast< ctoot::midi::core::ShortMessage* >(msg) != nullptr;
}

bool ShortMsg::isShort(int status)
{
    return false;
}

ctoot::midi::core::MidiMessage* ShortMsg::createShort(int status)
{
    auto msg = dynamic_cast< ctoot::midi::core::ShortMessage* >(fastShortPrototype_->clone());
    msg->setMessage(status);
    return msg;
}

ctoot::midi::core::MidiMessage* ShortMsg::createShort(int status, int data1)
{
    return createShort(status, data1, 0);
}

ctoot::midi::core::MidiMessage* ShortMsg::createShort(int status, int data1, int data2)
{
    
    auto msg = dynamic_cast< ctoot::midi::core::ShortMessage* >(fastShortPrototype_->clone());
    msg->setMessage(status, data1, data2);
    return msg;
}

ctoot::midi::core::MidiMessage* ShortMsg::setMessage(ctoot::midi::core::MidiMessage* msg, int status)
{
    dynamic_cast< ctoot::midi::core::ShortMessage* >(msg)->setMessage(status);
    return msg;
}

ctoot::midi::core::MidiMessage* ShortMsg::setMessage(ctoot::midi::core::MidiMessage* msg, int status, int data1, int data2)
{
    
    dynamic_cast< ctoot::midi::core::ShortMessage* >(msg)->setMessage(status, data1, data2);
    return msg;
}

void ShortMsg::createFastPrototypes()
{    
	/*
	try {
        auto sequence = new ctoot::midi::core::Sequence(ctoot::midi::core::Sequence::PPQ, 10);
        auto track = sequence->createTrack();
        track.add(new ctoot::midi::core::MidiEvent(new ctoot::midi::core::ShortMessage(), 0));
        try {
//            auto file = ::java::io::File::createTempFile(u"FSMhack"_j, u"mid"_j);
//            ::javax::sound::midi::MidiSystem::write(sequence, 0, file);
//            sequence = ::javax::sound::midi::MidiSystem::getSequence(file);
//            track = (*npc(sequence)->getTracks())[0];
//            ctoot::midi::core::MidiMessage* msg = nullptr;
//            for (auto i = 0; i < npc(track)->size(); i++) {
//                msg = npc(npc(track)->get(i))->getMessage();
//                auto clazz = msg->getClass();
//                if(fastShortPrototype_ == nullptr && npc(npc(clazz)->getCanonicalName())->equals(static_cast< ::java::lang::Object* >(u"com.sun.media.sound.FastShortMessage"_j))) {
//                    fastShortPrototype_ = msg;
//                }
//            }
//            npc(file)->delete_();
        } catch (exception* ioe) {
			//ioexception
        }
    } catch (exception* imde) {
		//invalidmididataexception
    }
	*/
}

int ShortMsg::getData1(ctoot::midi::core::MidiMessage* msg)
{    
    return dynamic_cast< ctoot::midi::core::ShortMessage* >(msg)->getData1();
}

ctoot::midi::core::MidiMessage* ShortMsg::setData1(ctoot::midi::core::MidiMessage* msg, int data1)
{

	if(msg->getLength() == 1) {
	 //     throw new ::javax::sound::midi::InvalidMidiDataException(u"Can't setData1 on 1 byte message"_j);
		return nullptr;
	}

	dynamic_cast<ctoot::midi::core::ShortMessage*>(msg)->setMessage(getStatus(msg), data1, getData2(msg));
	return msg;
}

int ShortMsg::getData2(ctoot::midi::core::MidiMessage* msg)
{
    
    return dynamic_cast< ctoot::midi::core::ShortMessage* >(msg)->getData2();
}

ctoot::midi::core::MidiMessage* ShortMsg::setData2(ctoot::midi::core::MidiMessage* msg, int data2)
{
    
//    if(msg->getLength() != 3)
//        throw new ::javax::sound::midi::InvalidMidiDataException(stringBuilder().append(u"Can't setData2 on "_j)->append(msg->getLength())
//            ->append(u" byte message"_j)->toString());

    dynamic_cast< ctoot::midi::core::ShortMessage* >(msg)->setMessage(getStatus(msg), getData1(msg), data2);
    return msg;
}

int ShortMsg::getData1and2(ctoot::midi::core::MidiMessage* msg)
{
    return getData1and2(getData1(msg), getData2(msg));
}

int ShortMsg::getData1and2(int data1, int data2)
{
    return (data1 & 63) | (data2 << 7);
}