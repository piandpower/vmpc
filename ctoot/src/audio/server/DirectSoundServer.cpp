#include "DirectSoundServer.hpp"
#include "StereoInputProcess.hpp"
#include "StereoOutputProcess.hpp"
#include "../core/ChannelFormat.hpp"

using namespace ctoot::audio::server;
using namespace std;

DirectSoundServer::DirectSoundServer(string inputDevice, string outputDevice) {
	//rtAudio = new RtAudio(RtAudio::WINDOWS_DS);
	inputID = getDeviceIndex(inputDevice);
	outputID = getDeviceIndex(outputDevice);
}

void DirectSoundServer::start() {
	if (running) return;
	client->setEnabled(true);
	running = true;
	startStream();
}

void DirectSoundServer::stop() {
	running = false;
	//rtAudio->stopStream();
	//rtAudio->closeStream();
}

bool DirectSoundServer::isRunning() {
	return running;
}

void DirectSoundServer::close() {
	destroyRtAudio();
	activeInputs.clear();
	activeOutputs.clear();
}

void DirectSoundServer::work() {
	client->work(bufferSize);
}

void DirectSoundServer::setClient(weak_ptr<AudioClient> client) {
	auto lClient = client.lock();
	this->client = lClient.get();
}

vector<string> DirectSoundServer::getAvailableOutputNames() {
	vector<string> res;
	return res;
}

vector<string> DirectSoundServer::getAvailableInputNames() {
	vector<string> res;
	return res;
}

IOAudioProcess* DirectSoundServer::openAudioOutput(string name, string label) {
	activeOutputs.push_back(new StereoOutputProcess(name, false, "DirectSoundServer"));
	return activeOutputs.back();
}

IOAudioProcess* DirectSoundServer::openAudioInput(string name, string label) {
	activeInputs.push_back(new StereoInputProcess(name, false, "DirectSoundServer"));
	return activeInputs.back();
}

void DirectSoundServer::closeAudioOutput(ctoot::audio::server::IOAudioProcess* output) {
	output->close();
	for (int i = 0; i < activeOutputs.size(); i++) {
		if (activeOutputs[i] == output) {
			activeOutputs.erase(activeOutputs.begin() + i);
			break;
		}
	}
}

void DirectSoundServer::closeAudioInput(ctoot::audio::server::IOAudioProcess* input) {
	input->close();
	for (int i = 0; i < activeInputs.size(); i++) {
		if (activeInputs[i] == input) {
			activeInputs.erase(activeInputs.begin() + i);
			break;
		}
	}
}

float DirectSoundServer::getSampleRate() {
	return 44100.0f;
}

float DirectSoundServer::getLoad() {
	return 0;
}

int DirectSoundServer::getInputLatencyFrames() {
	return bufferSize;
}

int DirectSoundServer::getOutputLatencyFrames() {
	return bufferSize;
}

int DirectSoundServer::getTotalLatencyFrames() {
	return getInputLatencyFrames() + getOutputLatencyFrames();
}


/// RtAudio impl.

int DirectSoundServer::getDeviceCount() {
	//return rtAudio->getDeviceCount();
	return 0;
}

string DirectSoundServer::getDeviceName(int devIndex) {
	//return rtAudio->getDeviceInfo(devIndex).name;
	return "foo";
}

int DirectSoundServer::getDeviceIndex(std::string name) {
	//int count = rtAudio->getDeviceCount();
	//for (int i = 0; i < count; i++) {
	//	if (getDeviceName(i).compare(name) == 0)
	//		return i;
	//}
	return -1;
}

bool DirectSoundServer::isDeviceProbed(int devIndex) {
	//return rtAudio->getDeviceInfo(devIndex).probed;
	return false;
}

int DirectSoundServer::getInputChannelCount(int devIndex) {
	//auto info = rtAudio->getDeviceInfo(devIndex);
	//return info.inputChannels;
	return 0;
}

int DirectSoundServer::getOutputChannelCount(int devIndex) {
	//auto info = rtAudio->getDeviceInfo(devIndex);
	//return info.outputChannels;
	return 0;
}

bool DirectSoundServer::startStream()
{
	//if (rtAudio->isStreamRunning()) return false;

	//if (rtAudio->isStreamOpen()) rtAudio->closeStream();

	//int numChannelsOut = 0;
	//if (outputID != -1 && isDeviceProbed(outputID)) {
	//	numChannelsOut = getOutputChannelCount(outputID);
	//}
	//int numChannelsIn = 0;
	//if (inputID != -1 && isDeviceProbed(inputID)) {
	//	numChannelsIn = getInputChannelCount(inputID);
	//}

	//RtAudio::StreamParameters iParams, oParams;
	//iParams.deviceId = inputID;
	//iParams.nChannels = numChannelsIn;
	//iParams.firstChannel = 0;

	//oParams.deviceId = outputID;
	//oParams.nChannels = numChannelsOut;
	//oParams.firstChannel = 0;

	//RtAudio::StreamOptions options;
	//options.flags = RTAUDIO_NONINTERLEAVED;

	//try
	//{
	//	rtAudio->openStream(&oParams, activeInputs.size() == 0 ? NULL : &iParams, RTAUDIO_FLOAT64, getSampleRate(), &bufferSize, &AudioCallback, this, &options);
	//	rtAudio->startStream();
	//}
	//catch (RtAudioError& e)
	//{
	//	e.getMessage();
	//	e.printMessage();
	//	return false;
	//}
	return true;
}

void DirectSoundServer::destroyRtAudio() {
	/*if (rtAudio) {
		if (rtAudio->isStreamOpen()) {
			rtAudio->closeStream();
		}
		delete rtAudio;
		rtAudio = 0;
	}*/
}

//int DirectSoundServer::AudioCallback(void *outputBuffer,
//	void *inputBuffer,
//	unsigned int nFrames,
//	double streamTime,
//	RtAudioStreamStatus status,
//	void *userData)
//{
//	auto server = (ctoot::audio::server::AudioServer*)userData;
//	if (!server->isRunning()) return 0;
//
//	auto activeInputs = server->getActiveInputs();
//	if (activeInputs.size() != 0) {
//		activeInputs.at(0)->localBuffer.clear();
//		double* inputBufferD = (double*)inputBuffer;
//
//		for (int i = 0; i < nFrames; i++) {
//			activeInputs.at(0)->localBuffer.push_back(inputBufferD[i]);
//			activeInputs.at(0)->localBuffer.push_back(inputBufferD[i + nFrames]);
//		}
//	}
//	server->work();
//
//	if (status)
//		std::cout << "Stream underflow detected!" << std::endl;
//
//	auto activeOutputs = server->getActiveOutputs();
//
//	if (activeOutputs.size() != 0) {
//		double* outputBufferD = (double*)outputBuffer;
//		int counter = 0;
//		int numChannelsOut = activeOutputs.at(0)->getChannelFormat()->getCount();
//		for (int h = 0; h < numChannelsOut; h += 2) {
//			for (int i = 0; i < nFrames; i++) {
//				double* outputs[2] = { outputBufferD + (nFrames * h) + i, outputBufferD + (nFrames * h) + nFrames + i };
//
//				double* out1 = outputs[0];
//				double* out2 = outputs[1];
//
//				*out1++ = activeOutputs.at(0)->localBuffer[counter++];
//				*out2++ = activeOutputs.at(0)->localBuffer[counter++];
//			}
//		}
//	}
//	return 0;
//}

//RtAudio* DirectSoundServer::getRtAudio() {
//	return rtAudio;
//}
