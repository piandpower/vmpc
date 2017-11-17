#include "SilentAudioServer.hpp"
#include "StereoInputProcess.hpp"
#include "StereoOutputProcess.hpp"
#include "../core/ChannelFormat.hpp"

#include <iostream>
#include <Logger.hpp>


using namespace ctoot::audio::server;
using namespace std;

SilentAudioServer::SilentAudioServer() {
}

void SilentAudioServer::static_audioThread(void * args)
{
	static_cast<SilentAudioServer*>(args)->runAudioThread();
}

void SilentAudioServer::runAudioThread() {
	
	float bufferSize = 512.0;
	float bufferDurationSeconds = bufferSize / 44100.0;
	float bufferDurationMilliseconds = bufferDurationSeconds * 1000.0;
	int sleepMicro = (int) (bufferDurationMilliseconds * 1000.0);

	while (running) {
		work();
		this_thread::sleep_for(chrono::microseconds(sleepMicro));
	}
}

void SilentAudioServer::start() {
	if (running) return;
	client->setEnabled(true);
	running = true;
	audioThread = thread(&SilentAudioServer::static_audioThread, this);
}

void SilentAudioServer::stop() {
	running = false;
	if (audioThread.joinable()) audioThread.join();
}

bool SilentAudioServer::isRunning() {
	return running;
}

void SilentAudioServer::close() {
	activeInputs.clear();
	activeOutputs.clear();
}

void SilentAudioServer::work() {
	std::cout << "working..." << endl;
	moduru::Logger::l.log("working...\n");
	client->work(bufferSize);
	if (activeOutputs.size() != 0) {
		int counter = 0;
		for (int i = 0; i < bufferSize; i++) {
			auto sampleL = activeOutputs.at(0)->localBuffer.at(counter++);
			auto sampleR = activeOutputs.at(0)->localBuffer[counter++];
			if (sampleL != 0)
				moduru::Logger::l.log(to_string(sampleL) + "\n");
		}
	}
}

void SilentAudioServer::setClient(weak_ptr<AudioClient> client) {
	auto lClient = client.lock();
	this->client = lClient.get();
}

vector<string> SilentAudioServer::getAvailableOutputNames() {
	vector<string> res{ "SilentOut1" };
	return res;
}

vector<string> SilentAudioServer::getAvailableInputNames() {
	vector<string> res;
	return res;
}

IOAudioProcess* SilentAudioServer::openAudioOutput(string name, string label) {
	activeOutputs.push_back(new StereoOutputProcess(name, false, "SilentAudioServer"));
	return activeOutputs.back();
}

IOAudioProcess* SilentAudioServer::openAudioInput(string name, string label) {
	activeInputs.push_back(new StereoInputProcess(name, false, "SilentAudioServer"));
	return activeInputs.back();
}

void SilentAudioServer::closeAudioOutput(ctoot::audio::server::IOAudioProcess* output) {
	output->close();
	for (int i = 0; i < activeOutputs.size(); i++) {
		if (activeOutputs[i] == output) {
			activeOutputs.erase(activeOutputs.begin() + i);
			break;
		}
	}
}

void SilentAudioServer::closeAudioInput(ctoot::audio::server::IOAudioProcess* input) {
	input->close();
	for (int i = 0; i < activeInputs.size(); i++) {
		if (activeInputs[i] == input) {
			activeInputs.erase(activeInputs.begin() + i);
			break;
		}
	}
}

float SilentAudioServer::getSampleRate() {
	return 44100.0f;
}

float SilentAudioServer::getLoad() {
	return 0;
}

int SilentAudioServer::getInputLatencyFrames() {
	return bufferSize;
}

int SilentAudioServer::getOutputLatencyFrames() {
	return bufferSize;
}

int SilentAudioServer::getTotalLatencyFrames() {
	return getInputLatencyFrames() + getOutputLatencyFrames();
}
