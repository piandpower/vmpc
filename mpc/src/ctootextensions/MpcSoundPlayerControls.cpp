#include <ctootextensions/MpcSoundPlayerControls.hpp>

#include <sampler/Sampler.hpp>
#include <audio/mixer/AudioMixer.hpp>
#include <audio/server/AudioServer.hpp>

using namespace mpc::ctootextensions;
using namespace std;

MpcSoundPlayerControls::MpcSoundPlayerControls(
	weak_ptr<mpc::sampler::Sampler> sampler
	, int drumNumber
	, weak_ptr<ctoot::audio::mixer::AudioMixer> mixer
	, vector<weak_ptr<mpc::ctootextensions::Voice>> voices
	, weak_ptr<ctoot::audio::server::AudioServer> server)
	: ctoot::synth::SynthChannelControls(MPC_SOUND_PLAYER_CHANNEL_ID, NAME_)
{
	this->sampler = sampler;
	this->drumNumber = drumNumber;
	this->mixer = mixer;
	this->voices = voices;
	this->server = server;
}

const int MpcSoundPlayerControls::MPC_SOUND_PLAYER_CHANNEL_ID;

string MpcSoundPlayerControls::NAME()
{
    return NAME_;
}
string MpcSoundPlayerControls::NAME_ = "MpcSoundPlayer";

weak_ptr<mpc::sampler::Sampler> MpcSoundPlayerControls::getSampler()
{
    return sampler;
}

int MpcSoundPlayerControls::getDrumNumber()
{
    return drumNumber;
}

weak_ptr<ctoot::audio::mixer::AudioMixer> MpcSoundPlayerControls::getMixer()
{
    return mixer;
}

vector<weak_ptr<mpc::ctootextensions::Voice>> MpcSoundPlayerControls::getVoices()
{
    return voices;
}

ctoot::audio::server::AudioServer* MpcSoundPlayerControls::getServer()
{
    return server.lock().get();
}

MpcSoundPlayerControls::~MpcSoundPlayerControls() {
}
