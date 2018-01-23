#include "UnrealAudioServer.hpp"
#include "StereoInputProcess.hpp"
#include "StereoOutputProcess.hpp"
#include "../core/ChannelFormat.hpp"

#include <iostream>
#include <Logger.hpp>


using namespace ctoot::audio::server;
using namespace std;

UnrealAudioServer::UnrealAudioServer() {
}

void UnrealAudioServer::start() {
	if (running) return;
	client->setEnabled(true);
	running = true;
}

void UnrealAudioServer::stop() {
	running = false;
}

bool UnrealAudioServer::isRunning() {
	return running;
}

void UnrealAudioServer::close() {
	activeInputs.clear();
	activeOutputs.clear();
}

void UnrealAudioServer::work() {
}

void UnrealAudioServer::work(float* OutAudio, int NumSamples) {
	client->work(NumSamples / 2);
	if (activeOutputs.size() != 0) {
		int counter = 0;
		for (int i = 0; i < NumSamples; i+=2) {
			auto sampleL = activeOutputs[0]->localBuffer[counter++];
			auto sampleR = activeOutputs[0]->localBuffer[counter++];
			OutAudio[i] = sampleL;
			OutAudio[i + 1] = sampleR;
		}
	}
}

void UnrealAudioServer::setClient(weak_ptr<AudioClient> client) {
	auto lClient = client.lock();
	this->client = lClient.get();
}

vector<string> UnrealAudioServer::getAvailableOutputNames() {
	vector<string> res{ "STEREO OUT", "ASSIGNABLE MIX OUT 1/2", "ASSIGNABLE MIX OUT 3/4", "ASSIGNABLE MIX OUT 5/6", "ASSIGNABLE MIX OUT 7/8" };
	return res;
}

vector<string> UnrealAudioServer::getAvailableInputNames() {
	vector<string> res{ "RECORD IN" };
	return res;
}

IOAudioProcess* UnrealAudioServer::openAudioOutput(string name, string label) {
	activeOutputs.push_back(new StereoOutputProcess(name, false, "UnrealAudioServer"));
	return activeOutputs.back();
}

IOAudioProcess* UnrealAudioServer::openAudioInput(string name, string label) {
	activeInputs.push_back(new StereoInputProcess(name, false, "UnrealAudioServer"));
	return activeInputs.back();
}

void UnrealAudioServer::closeAudioOutput(ctoot::audio::server::IOAudioProcess* output) {
	output->close();
	for (int i = 0; i < activeOutputs.size(); i++) {
		if (activeOutputs[i] == output) {
			activeOutputs.erase(activeOutputs.begin() + i);
			break;
		}
	}
}

void UnrealAudioServer::closeAudioInput(ctoot::audio::server::IOAudioProcess* input) {
	input->close();
	for (int i = 0; i < activeInputs.size(); i++) {
		if (activeInputs[i] == input) {
			activeInputs.erase(activeInputs.begin() + i);
			break;
		}
	}
}

float UnrealAudioServer::getLoad() {
	return 0;
}

int UnrealAudioServer::getInputLatencyFrames() {
	return bufferSize;
}

int UnrealAudioServer::getOutputLatencyFrames() {
	return bufferSize;
}

int UnrealAudioServer::getTotalLatencyFrames() {
	return getInputLatencyFrames() + getOutputLatencyFrames();
}
