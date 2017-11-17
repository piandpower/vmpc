#include <midi/misc/Controller.hpp>

using namespace ctoot::midi::misc;
using namespace std;

Controller::Controller()
{
	PERFORMANCE_CONTROLLERS_ = vector<int>({ PITCH_BEND_PSEUDO, CHANNEL_PRESSURE_PSEUDO, MODULATION, BREATH, FOOT, PORTAMENTO, EXPRESSION, SWITCHES });
	SOUND_CONTROLLERS_ = vector<int>({ PROGRAM_PSEUDO, VARIATION, TIMBRE, BRIGHTNESS, ATTACK, RELEASE });
	MIXER_CONTROLLERS_ = vector<int>({ VOLUME, PAN, BALANCE, FX_1, FX_2 });
	EFFECTS_CONTROLLERS_ = vector<int>({ EXTERNAL_FX, TREMELO, CHORUS, DETUNE, PHASER });
	UNDEFINED_CONTROLLERS_ = vector<int>({ CONTROLLER_3, CONTROLLER_9, 14, 15 });
	NO_CONTROLLERS_ = vector<int>({});
	SWITCH_CONTROLLERS_ = vector<int>({ HOLD_PEDAL, PORTAMENTO_SWITCH, SUSTENUTO_PEDAL, SOFT_PEDAL, LEGATO, HOLD_2_PEDAL });
	CATEGORIES_ = vector<string>({ "Performance", "Sound", "Mixer", "Effects", "Undefined" });
}

const int Controller::BANK_SELECT;
const int Controller::MODULATION;
const int Controller::BREATH;
const int Controller::CONTROLLER_3;
const int Controller::FOOT;
const int Controller::PORTAMENTO;
const int Controller::DATA_ENTRY;
const int Controller::VOLUME;
const int Controller::BALANCE;
const int Controller::CONTROLLER_9;
const int Controller::PAN;
const int Controller::EXPRESSION;
const int Controller::FX_1;
const int Controller::FX_2;
const int Controller::HOLD_PEDAL;
const int Controller::PORTAMENTO_SWITCH;
const int Controller::SUSTENUTO_PEDAL;
const int Controller::SOFT_PEDAL;
const int Controller::LEGATO;
const int Controller::HOLD_2_PEDAL;
const int Controller::VARIATION;
const int Controller::TIMBRE;
const int Controller::RELEASE;
const int Controller::ATTACK;
const int Controller::BRIGHTNESS;
const int Controller::EXTERNAL_FX;
const int Controller::TREMELO;
const int Controller::CHORUS;
const int Controller::DETUNE;
const int Controller::PHASER;
const int Controller::DATA_INCREMENT;
const int Controller::DATA_DECREMENT;
const int Controller::NON_REGISTERED_FINE;
const int Controller::NON_REGISTERED_COARSE;
const int Controller::REGISTERED_FINE;
const int Controller::REGISTERED_COARSE;
const int Controller::ALL_SOUND_OFF;
const int Controller::ALL_CONTROLLERS_OFF;
const int Controller::LOCAL_CONTROL;
const int Controller::ALL_NOTES_OFF;
const int Controller::OMNI_MODE_OFF;
const int Controller::OMNI_MODE_ON;
const int Controller::MONO_MODE;
const int Controller::POLY_MODE;
const int Controller::SWITCHES;
const int Controller::PITCH_BEND_PSEUDO;
const int Controller::POLY_PRESSURE_PSEUDO;
const int Controller::CHANNEL_PRESSURE_PSEUDO;
const int Controller::PROGRAM_PSEUDO;

vector<int>& Controller::PERFORMANCE_CONTROLLERS()
{
    
    return PERFORMANCE_CONTROLLERS_;
}
vector<int> Controller::PERFORMANCE_CONTROLLERS_;

vector<int>& Controller::SOUND_CONTROLLERS()
{
    
    return SOUND_CONTROLLERS_;
}
vector<int> Controller::SOUND_CONTROLLERS_;

vector<int>& Controller::MIXER_CONTROLLERS()
{
    
    return MIXER_CONTROLLERS_;
}
vector<int> Controller::MIXER_CONTROLLERS_;

vector<int>& Controller::EFFECTS_CONTROLLERS()
{
    
    return EFFECTS_CONTROLLERS_;
}
vector<int> Controller::EFFECTS_CONTROLLERS_;

vector<int>& Controller::UNDEFINED_CONTROLLERS()
{
    
    return UNDEFINED_CONTROLLERS_;
}
vector<int> Controller::UNDEFINED_CONTROLLERS_;

vector<int>& Controller::NO_CONTROLLERS()
{
    
    return NO_CONTROLLERS_;
}
vector<int> Controller::NO_CONTROLLERS_;

vector<int>& Controller::SWITCH_CONTROLLERS()
{
    
    return SWITCH_CONTROLLERS_;
}
vector<int> Controller::SWITCH_CONTROLLERS_;

vector<string>& Controller::CATEGORIES()
{
    
    return CATEGORIES_;
}
vector<string> Controller::CATEGORIES_;

vector<string> Controller::getCategories()
{
    
    return CATEGORIES_;
}

vector<int> Controller::getControllers(string category)
{

	if (string("Performance").compare(category) == 0) {
		return PERFORMANCE_CONTROLLERS_;
	}
	else if (string("Sound").compare(category) == 0) {
		return SOUND_CONTROLLERS_;
	}
	else if (string("Mixer").compare(category) == 0) {
		return MIXER_CONTROLLERS_;
	}
	else if (string("Effects").compare(category) == 0) {
		return EFFECTS_CONTROLLERS_;
	}
	else if (string("Undefined").compare(category) == 0) {
		return UNDEFINED_CONTROLLERS_;
	}
	return NO_CONTROLLERS_;
}

string Controller::propertyName(int controller)
{

	switch (controller) {
	case SWITCHES:
		return "Switches";
	case PITCH_BEND_PSEUDO:
		return "Pitch Bend";
	case POLY_PRESSURE_PSEUDO:
		return "Poly Pressure";
	case CHANNEL_PRESSURE_PSEUDO:
		return "Channel Pressure";
	case PROGRAM_PSEUDO:
		return "Program";
	case BANK_SELECT:
		return "Bank Select";
	case MODULATION:
		return "Modulation";
	case BREATH:
		return "Breath";
	case FOOT:
		return "Foot";
	case PORTAMENTO:
		return "Portamento";
	case DATA_ENTRY:
		return "Data Entry";
	case VOLUME:
		return "Volume";
	case BALANCE:
		return "Balance";
	case PAN:
		return "Pan";
	case EXPRESSION:
		return "Expression";
	case FX_1:
		return "Effect 1";
	case FX_2:
		return "Effect 2";
	case HOLD_PEDAL:
		return "Hold Pedal";
	case PORTAMENTO_SWITCH:
		return "Portamento Switch";
	case SUSTENUTO_PEDAL:
		return "Sustenuto Pedal";
	case SOFT_PEDAL:
		return "Soft Pedal";
	case LEGATO:
		return "Legato";
	case HOLD_2_PEDAL:
		return "Hold2 Pedal";
	case VARIATION:
		return "Variation";
	case TIMBRE:
		return "Timbre";
	case RELEASE:
		return "Release";
	case ATTACK:
		return "Attack";
	case BRIGHTNESS:
		return "Brightness";
	case EXTERNAL_FX:
		return "External Effects";
	case TREMELO:
		return "Tremelo";
	case CHORUS:
		return "Chorus";
	case DETUNE:
		return "Detune";
	case PHASER:
		return "Phaser";
	case DATA_INCREMENT:
		return "Data Increment";
	case DATA_DECREMENT:
		return "Data Decrement";
	case NON_REGISTERED_FINE:
		return "NRPN fine";
	case NON_REGISTERED_COARSE:
		return "NRPN coarse";
	case REGISTERED_FINE:
		return "RPN fine";
	case REGISTERED_COARSE:
		return "RPN coarse";
	case ALL_SOUND_OFF:
		return "All Sound Off";
	case ALL_CONTROLLERS_OFF:
		return "All Controllers Off";
	case LOCAL_CONTROL:
		return "Local Control";
	case ALL_NOTES_OFF:
		return "All Notes Off";
	case OMNI_MODE_OFF:
		return "Omni Off";
	case OMNI_MODE_ON:
		return "Omni On";
	case MONO_MODE:
		return "Mono";
	case POLY_MODE:
		return "Poly";
	default:
		return "Controller " + to_string(controller);
	}
}

bool Controller::is7bit(int controller)
{
    
    return controller >= 64 || (controller < 0 && controller != PITCH_BEND_PSEUDO);
}

int Controller::getMinimum(int controller)
{
    
    return 0;
}

int Controller::getMaximum(int controller)
{
    
    return -1 + (is7bit(controller) ? 128 : 128 * 128);
}

int Controller::getDefault(int controller)
{
    
    switch (controller) {
    case VOLUME:
        return 100 * 128;
    case EXPRESSION:
        return getMaximum(controller);
    case PAN:
    case BALANCE:
    case PITCH_BEND_PSEUDO:
        return (getMaximum(controller) + 1) / 2;
    default:
        return 0;
    }

}

int Controller::getOffset(int controller)
{

	return (controller == PAN || controller == BALANCE || controller == PITCH_BEND_PSEUDO) ? (getMaximum(controller) + 1) / 2 : 0;
}

int Controller::getOrientation(int controller)
{
	/*
	   if(controller == PAN || controller == BALANCE)
		   return ::javax::swing::SwingConstants::HORIZONTAL;

	   return ::javax::swing::SwingConstants::VERTICAL;
   */
	return 0;
}