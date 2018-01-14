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

void RtAudioServer::work(float** InAudio, float** OutAudio, int nFrames, int inputChannels, int outputChannels) {
	auto activeInputs = getActiveInputs();
	if (activeInputs.size() != 0 && inputChannels >= 2) {
		float* inputBufferL = (float*)InAudio[0];
		float* inputBufferR = (float*)InAudio[1];
		if (activeInputs[0]->localBuffer.size() != nFrames * 2);
		activeInputs[0]->localBuffer.resize(nFrames * 2);
		int frameCounter = 0;
		for (int i = 0; i < nFrames * 2; i += 2) {
			activeInputs[0]->localBuffer[i] = inputBufferL[frameCounter];
			activeInputs[0]->localBuffer[i + 1] = inputBufferR[frameCounter++];
		}
	}
	int channelsToProcess = activeOutputs.size() < (outputChannels / 2) ? activeOutputs.size() : (outputChannels / 2);
	client->work(nFrames);
	for (int output = 0; output < channelsToProcess; output++) {
		int counter = 0;
		for (int i = 0; i < nFrames; i++) {
			auto sampleL = activeOutputs[output]->localBuffer[counter++];
			auto sampleR = activeOutputs[output]->localBuffer[counter++];
			OutAudio[0][i] = (float)(sampleL);
			OutAudio[1][i] = (float)(sampleR);
		}
	}
}

void RtAudioServer::work(double** InAudio, double** OutAudio, int nFrames, int outputChannels) {
	auto activeInputs = getActiveInputs();
	if (activeInputs.size() != 0) {
		double* inputBufferL = (double*)InAudio[0];
		double* inputBufferR = (double*)InAudio[1];
		if (activeInputs[0]->localBuffer.size() != nFrames * 2);
			activeInputs[0]->localBuffer.resize(nFrames * 2);
		int frameCounter = 0;
		for (int i = 0; i < nFrames * 2; i += 2) {
			activeInputs[0]->localBuffer[i] = inputBufferL[frameCounter];
			activeInputs[0]->localBuffer[i + 1] = inputBufferR[frameCounter++];
		}
	}
	int channelsToProcess = activeOutputs.size() < (outputChannels / 2) ? activeOutputs.size() : (outputChannels / 2);
	client->work(nFrames);
	for (int output = 0; output < channelsToProcess; output++) {
		int counter = 0;
		for (int i = 0; i < nFrames; i++) {
			auto sampleL = activeOutputs[output]->localBuffer[counter++];
			auto sampleR = activeOutputs[output]->localBuffer[counter++];
			OutAudio[ (output * 2) ][i] = sampleL;
			OutAudio[ (output * 2) + 1][i] = sampleR;
		}
	}
}

void RtAudioServer::setClient(weak_ptr<AudioClient> client) {
	auto lClient = client.lock();
	this->client = lClient.get();
}

vector<string> RtAudioServer::getAvailableOutputNames() {
	vector<string> res{ "STEREO OUT", "ASSIGNABLE MIX OUT 1/2", "ASSIGNABLE MIX OUT 3/4", "ASSIGNABLE MIX OUT 5/6", "ASSIGNABLE MIX OUT 7/8" };
	return res;
}

vector<string> RtAudioServer::getAvailableInputNames() {
	vector<string> res{ "RECORD IN" };
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
