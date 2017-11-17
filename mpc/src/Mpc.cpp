#include <Mpc.hpp>

#include <audiomidi/AudioMidiServices.hpp>
#include <audiomidi/EventHandler.hpp>

#include <sampler/Sampler.hpp>
#include <sequencer/Sequence.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/Sequencer.hpp>
#include <ctootextensions/MpcBasicSoundPlayerChannel.hpp>
#include <ctootextensions/MpcMultiMidiSynth.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>

#include <midi/core/MidiInput.hpp>
#include <midi/core/MidiPort.hpp>
#include <midi/core/MidiTransport.hpp>
#include <synth/SynthChannel.hpp>

#include <Logger.hpp>

using namespace mpc;
using namespace std;

Mpc::Mpc()
{
	moduru::Logger::l.log("Mpc ctor\n");
}

void Mpc::init()
{

	sequencer = make_shared<mpc::sequencer::Sequencer>(this);
	moduru::Logger::l.log("sequencer created.\n");

	sampler = make_shared<mpc::sampler::Sampler>();
	moduru::Logger::l.log("sampler created.\n");
	
	/*
	* AudioMidiServices requires sampler to exist.
	*/
	audioMidiServices = make_shared<mpc::audiomidi::AudioMidiServices>(this);
	moduru::Logger::l.log("audio-midi-services created.\n");

	sequencer->init();
	moduru::Logger::l.log("sequencer initialized.\n");

	sampler->init(this);
	moduru::Logger::l.log("sampler initialized.\n");

	eventHandler = make_shared<mpc::audiomidi::EventHandler>(this);
	moduru::Logger::l.log("eventhandler created.\n");

	audioMidiServices->startTestMode();
	moduru::Logger::l.log("audioMidiServices test mode started.\n");
}

weak_ptr<mpc::sequencer::Sequencer> Mpc::getSequencer()
{
    return sequencer;
}

weak_ptr<sampler::Sampler> Mpc::getSampler()
{
    return sampler;
}

mpc::ctootextensions::MpcSoundPlayerChannel* Mpc::getDrum(int i)
{
	auto mms = audioMidiServices->getMms().lock();
	auto channel = mms->getChannel(i).lock().get();
	return dynamic_cast< mpc::ctootextensions::MpcSoundPlayerChannel*>(channel);
}

vector<mpc::ctootextensions::MpcSoundPlayerChannel*> Mpc::getDrums()
{
	auto drums = vector<mpc::ctootextensions::MpcSoundPlayerChannel*>(4);
	for (int i = 0; i < 4; i++) {
		drums[i] = getDrum(i);
	}
	return drums;
}

mpc::ctootextensions::MpcBasicSoundPlayerChannel* Mpc::getBasicPlayer()
{
	auto mms = audioMidiServices->getMms().lock();
	auto channel = mms->getChannel(4).lock().get();
	return dynamic_cast< mpc::ctootextensions::MpcBasicSoundPlayerChannel*>(channel);
}

weak_ptr<audiomidi::AudioMidiServices> Mpc::getAudioMidiServices()
{
    return audioMidiServices;
}

weak_ptr<audiomidi::EventHandler> Mpc::getEventHandler() {
	return eventHandler;
}

vector<char> Mpc::akaiAsciiChar = vector<char>({ ' ', '!', '#', '$', '%', '&', '\'', '(', ')', '-', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '}' });
vector<string> Mpc::akaiAscii = vector<string>({ " ", "!", "#", "$", "%", "&", "'", "(", ")", "-", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "_", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "}" });

Mpc::~Mpc() {
	moduru::Logger::l.log("Mpc dtor\n");
	audioMidiServices->destroyServices();
	moduru::Logger::l.log("audio midi services destroyed.\n");
}
