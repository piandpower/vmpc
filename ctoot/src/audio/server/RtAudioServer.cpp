#include "RtAudioServer.hpp"
#include "StereoInputProcess.hpp"
#include "StereoOutputProcess.hpp"
#include "../core/ChannelFormat.hpp"

#include <iostream>
#include <Logger.hpp>


using namespace ctoot::audio::server;
using namespace std;

RtAudioServer::RtAudioServer() {
}

void RtAudioServer::start() {
	if (running) return;
	client->setEnabled(true);
	running = true;
}

void RtAudioServer::stop() {
	running = false;
}

bool RtAudioServer::isRunning() {
	return running;
}

void RtAudioServer::close() {
	activeInputs.clear();
	activeOutputs.clear();
}

void RtAudioServer::work() {
}

void RtAudioServer::work(double** InAudio, double** OutAudio, int nFrames) {
	auto activeInputs = getActiveInputs();
	if (activeInputs.size() != 0) {
		activeInputs.at(0)->localBuffer.clear();
		double* inputBufferL = (double*)InAudio[0];
		double* inputBufferR = (double*)InAudio[1];
		for (int i = 0; i < nFrames; i++) {
			activeInputs.at(0)->localBuffer.push_back(inputBufferL[i]);
			activeInputs.at(0)->localBuffer.push_back(inputBufferR[i]);
		}
	}

	client->work(nFrames);
	if (activeOutputs.size() != 0) {
		int counter = 0;
		for (int i = 0; i < nFrames; i++) {
			auto sampleL = activeOutputs[0]->localBuffer[counter++];
			auto sampleR = activeOutputs[0]->localBuffer[counter++];
			OutAudio[0][i] = sampleL;
			OutAudio[1][i] = sampleR;
		}
	}
}

void RtAudioServer::setClient(weak_ptr<AudioClient> client) {
	auto lClient = client.lock();
	this->client = lClient.get();
}

vector<string> RtAudioServer::getAvailableOutputNames() {
	vector<string> res{ "Unreal Stereo Out 1" };
	return res;
}

vector<string> RtAudioServer::getAvailableInputNames() {
	vector<string> res{ "Unreal Stereo In 1" };
	return res;
}

IOAudioProcess* RtAudioServer::openAudioOutput(string name, string label) {
	activeOutputs.push_back(new StereoOutputProcess(name, false, "RtAudioServer"));
	return activeOutputs.back();
}

IOAudioProcess* RtAudioServer::openAudioInput(string name, string label) {
	activeInputs.push_back(new StereoInputProcess(name, false, "RtAudioServer"));
	return activeInputs.back();
}

void RtAudioServer::closeAudioOutput(ctoot::audio::server::IOAudioProcess* output) {
	output->close();
	for (int i = 0; i < activeOutputs.size(); i++) {
		if (activeOutputs[i] == output) {
			activeOutputs.erase(activeOutputs.begin() + i);
			break;
		}
	}
}

void RtAudioServer::closeAudioInput(ctoot::audio::server::IOAudioProcess* input) {
	input->close();
	for (int i = 0; i < activeInputs.size(); i++) {
		if (activeInputs[i] == input) {
			activeInputs.erase(activeInputs.begin() + i);
			break;
		}
	}
}

float RtAudioServer::getSampleRate() {
	return 44100.0f;
}

float RtAudioServer::getLoad() {
	return 0;
}

int RtAudioServer::getInputLatencyFrames() {
	return bufferSize;
}

int RtAudioServer::getOutputLatencyFrames() {
	return bufferSize;
}

int RtAudioServer::getTotalLatencyFrames() {
	return getInputLatencyFrames() + getOutputLatencyFrames();
}
