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

void UnrealAudioServer::work(float* OutAudio, int NumSamples) {
	client->work(NumSamples / 2);
	if (activeOutputs.size() != 0) {
		int counter = 0;
		for (int i = 0; i < NumSamples; i+=2) {
			auto sampleL = activeOutputs.at(0)->localBuffer[counter++];
			auto sampleR = activeOutputs.at(0)->localBuffer[counter++];
			OutAudio[i] = sampleL;
			OutAudio[i + 1] = sampleR;
		}
	}
}

void UnrealAudioServer::work(double** OutAudio, int nFrames) {
	client->work(nFrames);
	if (activeOutputs.size() != 0) {
		int counter = 0;
		for (int i = 0; i < nFrames; i++) {
			auto sampleL = activeOutputs.at(0)->localBuffer[counter++];
			auto sampleR = activeOutputs.at(0)->localBuffer[counter++];
			OutAudio[0][i] = sampleL;
			OutAudio[1][i] = sampleR;
		}
	}
}

void UnrealAudioServer::setClient(weak_ptr<AudioClient> client) {
	auto lClient = client.lock();
	this->client = lClient.get();
}

vector<string> UnrealAudioServer::getAvailableOutputNames() {
	vector<string> res{ "SilentOut1" };
	return res;
}

vector<string> UnrealAudioServer::getAvailableInputNames() {
	vector<string> res;
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

float UnrealAudioServer::getSampleRate() {
	return 44100.0f;
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
