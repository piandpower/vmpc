#include <synth/SynthChannel.hpp>

#include <audio/core/ChannelFormat.hpp>
//#include <midi/misc/Controller.hpp>
#include <synth/ControlChange.hpp>
#include <cmath>

using namespace ctoot::synth;
using namespace std;

SynthChannel::SynthChannel() 
{
	
	if (freqTable_.size() == 0) createFreqTable();
	sampleRate = 44100;
	inverseNyquist = 2.0f / sampleRate;
	rawBend = 8192;
	bendRange = 2;
	bendFactor = 1;
	pressure = 0;
	polyPressure = vector<char>(128);
	controller = vector<char>(128);
}

const double ctoot::synth::SynthChannel::ONE_SEMITONE { 1.0594630943592953 };

vector<float>* SynthChannel::freqTable()
{
    return &freqTable_;
}
vector<float> SynthChannel::freqTable_ = vector<float>(0);

void SynthChannel::createFreqTable()
{
	freqTable_ = vector<float>(140);
    for (auto i = 0; i < freqTable_.size(); i++) {
        freqTable_[i] = midiFreqImpl(i);
    }
}

float SynthChannel::midiFreq(float pitch)
{
	if (pitch < 0)
		return freqTable_[0];

	if (pitch >= (int)(freqTable_.size()) - 1)
		return freqTable_[(int)(freqTable_.size()) - 2];

	auto idx = static_cast<int>(pitch);
	auto frac = pitch - idx;
	return freqTable_[idx] * (1 - frac) + freqTable_[idx + 1] * frac;
}

float SynthChannel::midiFreqImpl(int pitch)
{
    
    return static_cast< float >((440.0 * pow(2.0, (static_cast< double >(pitch) - 69.0) / 12.0)));
}

void SynthChannel::setSampleRate(int rate)
{
    sampleRate = rate;
    inverseNyquist = 2.0f / sampleRate;
}

ctoot::audio::core::ChannelFormat* SynthChannel::getChannelFormat()
{
    return ctoot::audio::core::ChannelFormat::MONO();
}

void SynthChannel::noteOff(int pitch, int velocity)
{
    noteOff(pitch);
}

void SynthChannel::controlChange(int arg0, int arg1)
{
	controller[arg0] = static_cast<int8_t>(arg1);
	if (arg0 < 32) controller[arg0 + 32] = 0;

	setChanged();
	notifyObservers(new ctoot::synth::ControlChange(arg0, arg1));
}

int SynthChannel::getController(int arg0)
{
    return controller[arg0];
}

void SynthChannel::resetAllControllers()
{
    for (auto i = 0; i < controller.size() ; i++) {
        controller[i] = 0;
    }
 /*
	controller[ctoot::midi::misc::Controller::EXPRESSION] = 127;
    controller[ctoot::midi::misc::Controller::EXPRESSION + 32] = 127;
    controller[ctoot::midi::misc::Controller::VOLUME] = 100;
    controller[ctoot::midi::misc::Controller::PAN] = 64;
    controller[ctoot::midi::misc::Controller::PAN + 32] = 64;
	*/
    pressure = 0;
    setPitchBend(8192);
}

int SynthChannel::getProgram()
{
    return 0;
}

void SynthChannel::programChange(int arg0)
{
}

void SynthChannel::programChange(int arg0, int arg1)
{
}

int SynthChannel::getChannelPressure()
{
    return pressure;
}

void SynthChannel::setChannelPressure(int arg0)
{
    pressure = arg0;
}

int SynthChannel::getPolyPressure(int arg0)
{
    return polyPressure[arg0];
}

void SynthChannel::setPolyPressure(int arg0, int arg1)
{
    polyPressure[arg0] = static_cast< int8_t >(arg1);
}

bool SynthChannel::getSolo()
{
    return false;
}

bool SynthChannel::getMute()
{
    return false;
}

bool SynthChannel::getMono()
{
    return false;
}

bool SynthChannel::getOmni()
{
    return false;
}

void SynthChannel::setSolo(bool arg0)
{
}

void SynthChannel::setMute(bool arg0)
{
}

void SynthChannel::setMono(bool mono)
{
}

void SynthChannel::setOmni(bool arg0)
{
}

bool SynthChannel::localControl(bool arg0)
{
    return false;
}

void SynthChannel::setPitchBend(int bend)
{
    rawBend = bend;
    bend -= 8192;
    auto b = static_cast< float >(bendRange) * bend / 8192;
    bendFactor = static_cast< float >(pow(ONE_SEMITONE, b));
}

int SynthChannel::getPitchBend()
{
    return rawBend;
}

float SynthChannel::getBendFactor()
{
    return bendFactor;
}
