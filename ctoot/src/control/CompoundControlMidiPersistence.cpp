#include <control/CompoundControlMidiPersistence.hpp>

#include <control/BypassControl.hpp>
#include <control/CompoundControl.hpp>
#include <control/Control.hpp>
//#include <control/automation/ControlSysexMsg.hpp>
//#include <control/automation/MidiPersistence.hpp>

using namespace ctoot::control;
using namespace std;

CompoundControlMidiPersistence::CompoundControlMidiPersistence(FILE* root) 

{
	this->root = root;
}

/*
vector<string*> CompoundControlMidiPersistence::getPresets(CompoundControl* c)
{
	vector<string*> names;
	auto dir = new ::java::io::File(root, path(c));
    ::java::util::List* names = new ::java::util::ArrayList();
    if(!npc(dir)->exists() || !npc(dir)->isDirectory())
        return names;

    auto files = npc(dir)->listFiles();
    for(auto file : *npc(files)) {
        if(npc(file)->isDirectory())
            continue;

        npc(names)->add(static_cast< ::java::lang::Object* >(npc(file)->getName()));
    }
    return names;
}
*/
/*

void CompoundControlMidiPersistence::loadPreset(CompoundControl* c, string* name)
{
    auto providerId = npc(c)->getProviderId();
    auto moduleId = npc(c)->getId();
    auto path = new ::java::io::File(root, this->path(c));
    auto file = new ::java::io::File(path, name);
    if(!npc(file)->exists())
        return;

    try {
        auto sequence = ::javax::sound::midi::MidiSystem::getSequence(file);
        auto track = (*npc(sequence)->getTracks())[0];
        for (auto i = 0; i < npc(track)->size(); i++) {
            auto msg = npc(npc(track)->get(i))->getMessage();
            if(!::automation::ControlSysexMsg::isControl(msg))
                continue;

            if(::automation::ControlSysexMsg::getProviderId(msg) != providerId)
                continue;

            if(::automation::ControlSysexMsg::getModuleId(msg) != moduleId)
                continue;

            auto control = npc(c)->deepFind(::automation::ControlSysexMsg::getControlId(msg));
            if(control == nullptr)
                continue;

            if(dynamic_cast< BypassControl* >(control) != nullptr)
                continue;

            npc(control)->setIntValue(::automation::ControlSysexMsg::getValue(msg));
        }
    } catch (::java::io::IOException* ioe) {
        npc(ioe)->printStackTrace();
    } catch (::javax::sound::midi::InvalidMidiDataException* imde) {
        npc(imde)->printStackTrace();
    }
}
*/

/*

void CompoundControlMidiPersistence::savePreset(CompoundControl* c, string* name)
{
    auto providerId = npc(c)->getProviderId();
    auto moduleId = npc(c)->getId();
    try {
        auto sequence = new ::javax::sound::midi::Sequence(::javax::sound::midi::Sequence::PPQ, int32_t(1));
        auto track = npc(sequence)->createTrack();
        ::automation::MidiPersistence::store(providerId, moduleId, 0, c, track);
        auto path = new ::java::io::File(root, this->path(c));
        npc(path)->mkdirs();
        ::javax::sound::midi::MidiSystem::write(sequence, 0, new ::java::io::File(path, name));
    } catch (::java::io::IOException* ioe) {
        npc(ioe)->printStackTrace();
    } catch (::javax::sound::midi::InvalidMidiDataException* imde) {
        npc(imde)->printStackTrace();
    }
}
*/

string* CompoundControlMidiPersistence::path(ctoot::control::CompoundControl* c)
{
	/*
	return ::stringBuilder().append(npc(c)->getPersistenceDomain())->append(::java::io::File::separator())
        ->append(npc(c)->getProviderId())
        ->append(::java::io::File::separator())
        ->append(npc(c)->getId())->toString();
		*/
	return nullptr;
}
