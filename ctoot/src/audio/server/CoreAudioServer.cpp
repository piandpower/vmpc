//
//  CoreAudioServer.cpp
//  vMPC
//
//  Created by Izmael on 9/24/16.
//
//

#include "CoreAudioServer.hpp"

using namespace ctoot::audio::server;
using namespace std;

CoreAudioServer::CoreAudioServer(int outDeviceID, int inDeviceID) {
    this->outDeviceID = outDeviceID;
    this->inDeviceID = inDeviceID;
  //  rtAudio = new RtAudio(RtAudio::MACOSX_CORE);
   // if (rtAudio->getDeviceInfo(outDeviceID).outputChannels == 0) this->outDeviceID = rtAudio->getDefaultOutputDevice();
   // if (rtAudio->getDeviceInfo(inDeviceID).inputChannels == 0) this->inDeviceID = rtAudio->getDefaultInputDevice();
    
    detectInputs();
    detectOutputs();
}

//RtAudio* CoreAudioServer::getRtAudio() {
//	return rtAudio;
//}

void CoreAudioServer::start() {
	if (running) return;
	startStream();
	client->setEnabled(true);
	running = true;
}

void CoreAudioServer::stop() {
	running = false;
//	rtAudio->stopStream();
//	rtAudio->closeStream();
}

bool CoreAudioServer::isRunning() {
	return running;
}

void CoreAudioServer::close() {
	destroyRtAudio();
	activeInputs.clear();
	activeOutputs.clear();
}

void CoreAudioServer::work() {
	client->work(bufferSize);
}

void CoreAudioServer::setClient(weak_ptr<AudioClient> client) {
	auto lClient = client.lock();
	this->client = lClient.get();
}

vector<string> CoreAudioServer::getAvailableOutputNames() {
	vector<string> res;
	for (auto& out : availableOutputs)
		res.push_back(out.name);
	return res;
}

vector<string> CoreAudioServer::getAvailableInputNames() {
	vector<string> res;
	for (auto& in : availableInputs)
		res.push_back(in.name);
	return res;
}

void CoreAudioServer::detectOutputs() {
	availableOutputs.clear();
	/*int channels = rtAudio->getDeviceInfo(outDeviceID).outputChannels;
	for (int i = 0; i < channels; i += 2) {
		string name = "Out " + to_string(i + 1) + "/" + to_string(i + 2);
		NamedIO io(name, i, 2);
		availableOutputs.push_back(io);
	}*/
	activeOutputs = vector<ctoot::audio::server::IOAudioProcess*>(availableOutputs.size());
}

void CoreAudioServer::detectInputs() {
	availableInputs.clear();
	//int channels = rtAudio->getDeviceInfo(inDeviceID).inputChannels;
	//for (int i = 0; i < channels; i += 2) {
	//	string name = "In " + to_string(i + 1) + "/" + to_string(i + 2);
	//	NamedIO io(name, i, 2);
	//	availableInputs.push_back(io);
	//}
	activeInputs = vector<ctoot::audio::server::IOAudioProcess*>(availableInputs.size());
}


IOAudioProcess* CoreAudioServer::openAudioOutput(string name, string label) {
	int startChannel = -1;
	for (auto& out : availableOutputs) {
		if (out.name.compare(name) == 0) {
			startChannel = out.first;
		}
	}
	if (startChannel == -1) return nullptr;
	auto process = new StereoOutputProcess(name, false, "CoreAudioServer");
	activeOutputs[startChannel / 2] = process;
	return process;

}

IOAudioProcess* CoreAudioServer::openAudioInput(string name, string label) {
	int startChannel = -1;
	for (auto& in: availableInputs) {
		if (in.name.compare(name) == 0) {
			startChannel = in.first;
		}
	}
	if (startChannel == -1) return nullptr;
	auto process = new StereoInputProcess(name, false, "CoreAudioServer");
	activeInputs[startChannel / 2] = process;
	return process;
}

void CoreAudioServer::closeAudioOutput(ctoot::audio::server::IOAudioProcess* output) {
	output->close();
	for (int i = 0; i < activeOutputs.size(); i++) {
		if (activeOutputs[i] == output) {
			activeOutputs[i] = nullptr;
			break;
		}
	}
}

void CoreAudioServer::closeAudioInput(ctoot::audio::server::IOAudioProcess* input) {
	input->close();
	for (int i = 0; i < activeInputs.size(); i++) {
		if (activeInputs[i] == input) {
			activeInputs[i] = nullptr;
			break;
		}
	}
}

float CoreAudioServer::getSampleRate() {
	return 44100.0f;
}

float CoreAudioServer::getLoad() {
	return 0;
}

int CoreAudioServer::getInputLatencyFrames() {
	return bufferSize;
}

int CoreAudioServer::getOutputLatencyFrames() {
	return bufferSize;
}

int CoreAudioServer::getTotalLatencyFrames() {
	return getInputLatencyFrames() + getOutputLatencyFrames();
}

bool CoreAudioServer::isDeviceProbed(int devIndex) {
	//return rtAudio->getDeviceInfo(devIndex).probed;
	return false;
}

bool CoreAudioServer::startStream()
{
	//if (rtAudio->isStreamRunning()) return false;
	//if (rtAudio->isStreamOpen()) rtAudio->closeStream();

	//int numChannelsOut = 0;
	//int numChannelsIn = 0;
	//if (outDeviceID != -1 && isDeviceProbed(outDeviceID)) {
	//	auto info = rtAudio->getDeviceInfo(outDeviceID);
	//	numChannelsOut = info.outputChannels;
	//}
 //   if (inDeviceID != -1 && isDeviceProbed(inDeviceID)) {
 //       auto info = rtAudio->getDeviceInfo(inDeviceID);
 //       numChannelsIn = info.inputChannels;
 //   }
 //   
	//RtAudio::StreamParameters iParams, oParams;
	//iParams.deviceId = inDeviceID;
	//iParams.nChannels = numChannelsIn;
	//iParams.firstChannel = 0;
 //   
	//oParams.deviceId = outDeviceID;
	//oParams.nChannels = numChannelsOut;
	//oParams.firstChannel = 0;
 //   
	//RtAudio::StreamOptions options;
	//options.flags = RTAUDIO_NONINTERLEAVED;
 //   
	//try
	//{
	//	rtAudio->openStream(&oParams, &iParams, RTAUDIO_FLOAT64, getSampleRate(), &bufferSize, &AudioCallback, this, &options);
	//	rtAudio->startStream();
	//}
	//catch (RtAudioError& e)
	//{
	////	moduru::Logger::l.log(e.getMessage());
	//	return false;
	//}
	return true;
}

void CoreAudioServer::destroyRtAudio() {
	//if (rtAudio) {
	//	if (rtAudio->isStreamOpen()) {
	//		//rtAudio->closeStream();
 //           rtAudio->abortStream();
	//	}
	//	delete rtAudio;
	//	rtAudio = 0;
	//}
}

//int CoreAudioServer::AudioCallback(void *outputBuffer,
//                              void *inputBuffer,
//                              unsigned int nFrames,
//                              double streamTime,
//                              RtAudioStreamStatus status,
//                              void *userData)
//{
//	auto server = (CoreAudioServer*)userData;
//	if (!server->isRunning()) return 0;
//    
//	auto activeInputs = server->getActiveInputs();
//	int numChannelsIn = (int) activeInputs.size() * 2;
//    
//	for (int h = 0; h < numChannelsIn; h += 2) {
//		IOAudioProcess* proc = activeInputs[h / 2];
//		if (proc == nullptr) continue;
//		double* inputBufferD = (double*)inputBuffer;
//        activeInputs[h / 2]->localBuffer.clear();
//		for (int i = 0; i < nFrames; i++) {
//			activeInputs[h / 2]->localBuffer.push_back(inputBufferD[i]);
//			activeInputs[h / 2]->localBuffer.push_back(inputBufferD[i + nFrames]);
//		}
//	}
//
//	server->work();
//    
//	if (status)
//		std::cout << "Stream underflow detected!" << std::endl;
//    
//    double* outputBufferD = (double*)outputBuffer;
//	auto activeOutputs = server->getActiveOutputs();
//	int numChannelsOut = (int) activeOutputs.size() * 2;
//    for (int h = 0; h < numChannelsOut; h += 2) {
//        IOAudioProcess* proc = activeOutputs[h / 2];
//        if (proc == nullptr) continue;
//        int counter = 0;
//        for (int i = 0; i < nFrames; i++) {
//            double* outputs[2] = { outputBufferD + (nFrames * h) + i, outputBufferD + (nFrames * h) + nFrames + i };
//            
//            double* out1 = outputs[0];
//            double* out2 = outputs[1];
//            
//            *out1++ = proc->localBuffer[counter++];
//            *out2++ = proc->localBuffer[counter++];
//        }
//    }
//	return 0;
//}
