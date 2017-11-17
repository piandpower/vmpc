#include <audio/mixer/LCRControl.hpp>
#include <audio/mixer/MixControlIds.hpp>

using namespace ctoot::audio::mixer;
using namespace std;

LCRControl::LCRControl(string name, ctoot::control::ControlLaw law, float precision, float initialValue)
	: FloatControl(MixControlIds::LCR, name, law, precision, initialValue)
{
}

vector<string>*& LCRControl::presetNames()
{
    return presetNames_;
}
vector<string>* ctoot::audio::mixer::LCRControl::presetNames_ = new vector<string>{"Center","Left", "Right"};


vector<string>* LCRControl::getPresetNames()
{
    return presetNames_;
}

void LCRControl::applyPreset(string presetName)
{
//    if(npc(presetName)->equals(static_cast< ::java::lang::Object* >(::uk::org::toot::misc::Localisation::getString(u"Center"_j)))) {
//        setValue(0.5f);
//    } else if(npc(presetName)->equals(static_cast< ::java::lang::Object* >(::uk::org::toot::misc::Localisation::getString(u"Left"_j)))) {
//        setValue(0.0f);
//    } else if(npc(presetName)->equals(static_cast< ::java::lang::Object* >(::uk::org::toot::misc::Localisation::getString(u"Right"_j)))) {
//        setValue(1.0f);
//    }
}

LCRControl::~LCRControl() {
}
