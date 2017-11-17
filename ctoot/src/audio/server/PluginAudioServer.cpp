#include <audio/server/PluginAudioServer.hpp>
#include <audio/server/StereoInputProcess.hpp>
#include <audio/server/StereoOutputProcess.hpp>

#include <thread>

using namespace std;
using namespace ctoot::audio::server;

PluginAudioServer::PluginAudioServer(int inCount, int outCount)
{
	this->inCount = inCount;
	this->outCount = outCount;
	detectInputs();
	detectOutputs();
}

void PluginAudioServer::close()
{
	activeInputs.clear();
	activeOutputs.clear();
}

void PluginAudioServer::closeAudioInput(IOAudioProcess* input)
{
	input->close();
	for (int i = 0; i < activeInputs.size(); i++) {
		if (activeInputs[i] == input) {
			activeInputs[i] = nullptr;
			break;
		}
	}
}
void PluginAudioServer::closeAudioOutput(IOAudioProcess* output)
{
	output->close();
	for (int i = 0; i < activeOutputs.size(); i++) {
		if (activeOutputs[i] == output) {
			activeOutputs[i] = nullptr;
			break;
		}
	}
}

void PluginAudioServer::start()
{
	if (running) return;
	resizeBuffers(bufferSize);
	for (auto& o : activeOutputs) {
		if (o != nullptr)
			o->localBuffer.resize(bufferSize * 2);
	}
	auto lClient = client.lock();
	lClient->setEnabled(true);
	running = true;
}

void PluginAudioServer::stop()
{
	running = false;
}

bool PluginAudioServer::isRunning()
{
	return running;
}

void PluginAudioServer::setClient(weak_ptr<AudioClient> client)
{
	this->client = client;
}

void PluginAudioServer::detectInputs() {
	availableInputs.clear();
	for (int i = 0; i < inCount; i += 2) {
		string name = "In " + to_string(i + 1) + "/" + to_string(i + 2);
		NamedIO io(name, i, 2);
		availableInputs.push_back(io);
	}
	activeInputs = vector<ctoot::audio::server::IOAudioProcess*>(availableInputs.size());
}

void PluginAudioServer::detectOutputs() {
	availableOutputs.clear();
	for (int i = 0; i < outCount; i += 2) {
		string name = "Out " + to_string(i + 1) + "/" + to_string(i + 2);
		NamedIO io(name, i, 2);
		availableOutputs.push_back(io);
	}
	activeOutputs = vector<ctoot::audio::server::IOAudioProcess*>(availableOutputs.size());
}

vector<string> PluginAudioServer::getAvailableOutputNames()
{
	vector<string> res;
	for (int i = 0; i < outCount; i += 2)
		res.push_back("Out " + to_string(i + 1) + "/" + to_string(i + 2));
	return res;
}

vector<string> PluginAudioServer::getAvailableInputNames()
{
	vector<string> res;
	for (int i = 0; i < inCount; i += 2)
		res.push_back("In " + to_string(i + 1) + "/" + to_string(i + 2));
	return res;
}

IOAudioProcess* PluginAudioServer::openAudioOutput(string name, string label)
{
	int startChannel = -1;
	for (auto& out : availableOutputs) {
		if (out.name.compare(name) == 0) {
			startChannel = out.first;
		}
	}
	if (startChannel == -1) return nullptr;
	auto process = new StereoOutputProcess(name, false, "PluginAudioServer");
	activeOutputs[startChannel / 2] = process;
	return process;
}

IOAudioProcess* PluginAudioServer::openAudioInput(string name, string label)
{
	int startChannel = -1;
	for (auto& in : availableInputs) {
		if (in.name.compare(name) == 0) {
			startChannel = in.first;
		}
	}
	if (startChannel == -1) return nullptr;
	auto process = new StereoInputProcess(name, false, "PluginAudioServer");
	activeInputs[startChannel / 2] = process;
	return process;
}

float PluginAudioServer::getSampleRate()
{
	return 44100.0f;
}

float PluginAudioServer::getLoad()
{
	return 0;
}

int PluginAudioServer::getInputLatencyFrames()
{
	return bufferSize;
}

int PluginAudioServer::getOutputLatencyFrames()
{
	return PluginAudioServer::bufferSize;
}

int PluginAudioServer::getTotalLatencyFrames()
{
	return getInputLatencyFrames() + getOutputLatencyFrames();
}

void PluginAudioServer::work(double** inputs, double** outputs, int nFrames) {
	if (!running) return;


    if (activeInputs.size() > 0 && activeInputs[0]->localBuffer.size() >= bufferSize) {
		for (auto& i : activeInputs)
			if (i != nullptr)
				i->localBuffer.clear();
	}

    if (bufferPointer == 0) {
		//if (requestedBufferSize != bufferSize) {
		//	resizeBuffers(bufferSize);
		//	for (auto& o : activeOutputs) {
		//		if (o != nullptr)
		//			o->localBuffer.resize(bufferSize * 2);
		//	}
		//}
	}

	for (int j = 0; j < activeInputs.size(); j++) {
		if (activeInputs[j] == nullptr) continue;
		double* in1 = inputs[j*2];
		double* in2 = inputs[(j*2)+1];
		for (int i = 0; i < nFrames; i++)
		{
			activeInputs[j]->localBuffer.push_back(*in1++);
			activeInputs[j]->localBuffer.push_back(*in2++);
		}
	}


	if (bufferPointer == 0) {
		auto lClient = client.lock();
		if (lClient) lClient->work(bufferSize);
	}
	int currentBufferPointer = bufferPointer;

	for (int j = 0; j < activeOutputs.size(); j++) {
		if (activeOutputs[j] == nullptr) continue;
		double* out1 = outputs[j * 2];
		double* out2 = outputs[(j * 2) + 1];
		bufferPointer = currentBufferPointer;
		for (int i = 0; i < nFrames; i++)
		{
			*out1++ = activeOutputs[j]->localBuffer[bufferPointer++];
			*out2++ = activeOutputs[j]->localBuffer[bufferPointer++];
		}
	}
	if (bufferPointer >= activeOutputs[0]->localBuffer.size()) bufferPointer = 0;
}

PluginAudioServer::~PluginAudioServer()
{
}
