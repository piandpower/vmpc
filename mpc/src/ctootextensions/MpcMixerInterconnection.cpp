#include <ctootextensions/MpcMixerInterconnection.hpp>

#include <ctootextensions/InterconnectionInputProcess.hpp>
#include <ctootextensions/InterconnectionOutputProcess.hpp>
#include <audio/core/AudioBuffer.hpp>
#include <audio/core/AudioProcess.hpp>
#include <audio/server/AudioServer.hpp>

using namespace mpc::ctootextensions;
using namespace std;

MpcMixerInterconnection::MpcMixerInterconnection(string name, ctoot::audio::server::AudioServer* server)
{
	auto const sharedBuffer = server->createAudioBuffer(name);
	inputProcess = new InterconnectionInputProcess(this, sharedBuffer);
	outputProcess = new InterconnectionOutputProcess(this, sharedBuffer);
}

ctoot::audio::core::AudioProcess* MpcMixerInterconnection::getInputProcess()
{
    return inputProcess;
}

ctoot::audio::core::AudioProcess* MpcMixerInterconnection::getOutputProcess()
{
    return outputProcess;
}

void MpcMixerInterconnection::setLeftEnabled(bool b)
{
    leftEnabled = b;
}

void MpcMixerInterconnection::setRightEnabled(bool b)
{
    rightEnabled = b;
}

MpcMixerInterconnection::~MpcMixerInterconnection() {
	if (inputProcess != nullptr) {
		delete inputProcess;
	}
	if (outputProcess != nullptr) {
		delete outputProcess;
	}
}
