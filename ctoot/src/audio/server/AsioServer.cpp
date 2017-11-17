#include "AsioServer.hpp"
#include "StereoInputProcess.hpp"
#include "StereoOutputProcess.hpp"
#include "../core/ChannelFormat.hpp"

#include "NamedIO.hpp"

using namespace ctoot::audio::server;
using namespace std;

AsioServer::AsioServer(int deviceID) {
	//rtAudio = new RtAudio(RtAudio::WINDOWS_ASIO);
	this->deviceID = deviceID;
	detectOutputs();
	detectInputs();
}

//RtAudio* AsioServer::getRtAudio() {
//	return rtAudio;
//}

void AsioServer::start() {
	if (running) return;
	startStream();
	client->setEnabled(true);
	running = true;
}

void AsioServer::stop() {
	running = false;
	//rtAudio->stopStream();
	//rtAudio->closeStream();
}

bool AsioServer::isRunning() {
	return running;
}

void AsioServer::close() {
	destroyRtAudio();
	activeInputs.clear();
	activeOutputs.clear();
}

void AsioServer::work() {
	client->work(bufferSize);
}

void AsioServer::setClient(weak_ptr<AudioClient> client) {
	auto lClient = client.lock();
	this->client = lClient.get();
}

vector<string> AsioServer::getAvailableOutputNames() {
	vector<string> res;
	for (auto& out : availableOutputs)
		res.push_back(out.name);
	return res;
}

void AsioServer::detectOutputs() {
	availableOutputs.clear();
	//int channels = rtAudio->getDeviceInfo(deviceID).outputChannels;
	//for (int i = 0; i < channels; i += 2) {
	//	string name = "Out " + to_string(i + 1) + "/" + to_string(i + 2);
	//	NamedIO io(name, i, 2);
	//	availableOutputs.push_back(io);
	//}
	activeOutputs = vector<ctoot::audio::server::IOAudioProcess*>(availableOutputs.size());
}

void AsioServer::detectInputs() {
	availableInputs.clear();
	//int channels = rtAudio->getDeviceInfo(deviceID).inputChannels;
	//for (int i = 0; i < channels; i += 2) {
	//	string name = "In " + to_string(i + 1) + "/" + to_string(i + 2);
	//	NamedIO io(name, i, 2);
	//	availableInputs.push_back(io);
	//}
	activeInputs = vector<ctoot::audio::server::IOAudioProcess*>(availableInputs.size());
}

vector<string> AsioServer::getAvailableInputNames() {
	vector<string> res;
	for (auto& in : availableInputs)
		res.push_back(in.name);
	return res;
}

IOAudioProcess* AsioServer::openAudioOutput(string name, string label) {
	int startChannel = -1;
	for (auto& out : availableOutputs) {
		if (out.name.compare(name) == 0) {
			startChannel = out.first;
		}
	}
	if (startChannel == -1) return nullptr;
	auto process = new StereoOutputProcess(name, false, "AsioServer");
	activeOutputs[startChannel / 2] = process;
	return process;
}

IOAudioProcess* AsioServer::openAudioInput(string name, string label) {
	int startChannel = -1;
	for (auto& in: availableInputs) {
		if (in.name.compare(name) == 0) {
			startChannel = in.first;
		}
	}
	if (startChannel == -1) return nullptr;
	auto process = new StereoInputProcess(name, false, "AsioServer");
	activeInputs[startChannel / 2] = process;
	return process;
}

void AsioServer::closeAudioOutput(ctoot::audio::server::IOAudioProcess* output) {
	output->close();
	for (int i = 0; i < activeOutputs.size(); i++) {
		if (activeOutputs[i] == output) {
			activeOutputs[i] = nullptr;
			break;
		}
	}
}

void AsioServer::closeAudioInput(ctoot::audio::server::IOAudioProcess* input) {
	input->close();
	for (int i = 0; i < activeInputs.size(); i++) {
		if (activeInputs[i] == input) {
			activeInputs[i] = nullptr;
			break;
		}
	}
}

float AsioServer::getSampleRate() {
	return 44100.0f;
}

float AsioServer::getLoad() {
	return 0;
}

int AsioServer::getInputLatencyFrames() {
	return bufferSize;
}

int AsioServer::getOutputLatencyFrames() {
	return bufferSize;
}

int AsioServer::getTotalLatencyFrames() {
	return getInputLatencyFrames() + getOutputLatencyFrames();
}


/// RtAudio impl.

bool AsioServer::isDeviceProbed(int devIndex) {
	//return rtAudio->getDeviceInfo(devIndex).probed;
	return false;
}

bool AsioServer::startStream()
{
	//if (rtAudio->isStreamRunning()) return false;
	//if (rtAudio->isStreamOpen()) rtAudio->closeStream();

	int numChannelsOut = 0;
	int numChannelsIn = 0;
	if (deviceID != -1 && isDeviceProbed(deviceID)) {
		//auto info = rtAudio->getDeviceInfo(deviceID);
		//numChannelsOut = info.outputChannels;
		//numChannelsIn = info.inputChannels;
	}

	//RtAudio::StreamParameters iParams, oParams;
	//iParams.deviceId = deviceID;
	//iParams.nChannels = numChannelsIn;
	//iParams.firstChannel = 0;

	//oParams.deviceId = deviceID;
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
	//	return false;
	//}
	return true;
}

void AsioServer::destroyRtAudio() {
	//if (rtAudio) {
	//	if (rtAudio->isStreamRunning()) {
	//		rtAudio->abortStream();
	//		rtAudio->closeStream();
	//	}
	//	delete rtAudio;
	//	rtAudio = 0;
	//}
}

//int AsioServer::AudioCallback(void *outputBuffer,
//	void *inputBuffer,
//	unsigned int nFrames,
//	double streamTime,
//	RtAudioStreamStatus status,
//	void *userData)
//{
//
//	auto server = (ctoot::audio::server::AudioServer*) userData;
//
//	if (!server->isRunning()) return 0;
//
//	auto activeInputs = server->getActiveInputs();
//
//	int numChannelsIn = activeInputs.size() * 2;
//
//	for (int h = 0; h < numChannelsIn; h += 2) {
//		IOAudioProcess* proc = activeInputs[h / 2];
//		if (proc == nullptr) continue;
//		double* inputBufferD = (double*)inputBuffer;
//		activeInputs[h / 2]->localBuffer.clear();
//		for (int i = 0; i < nFrames; i++) {
//			activeInputs[h / 2]->localBuffer.push_back(inputBufferD[i]);
//			activeInputs[h / 2]->localBuffer.push_back(inputBufferD[i + nFrames]);
//		}
//	}
//	
//	server->work();
//	auto activeOutputs = server->getActiveOutputs();
//	if (status)
//		std::cout << "Stream underflow detected!" << std::endl;
//
//		double* outputBufferD = (double*)outputBuffer;
//		int numChannelsOut = activeOutputs.size() * 2;
//		for (int h = 0; h < numChannelsOut; h += 2) {
//			IOAudioProcess* proc = activeOutputs[h / 2];
//			if (proc == nullptr) continue;
//			int counter = 0;
//			for (int i = 0; i < nFrames; i++) {
//				double* outputs[2] = { outputBufferD + (nFrames * h) + i, outputBufferD + (nFrames * h) + nFrames + i };
//
//				double* out1 = outputs[0];
//				double* out2 = outputs[1];
//
//				*out1++ = proc->localBuffer[counter++];
//				*out2++ = proc->localBuffer[counter++];
//			}
//		}
//	return 0;
//}
