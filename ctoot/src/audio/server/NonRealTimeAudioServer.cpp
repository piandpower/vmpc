#include <audio/server/NonRealTimeAudioServer.hpp>

#include <audio/core/AudioBuffer.hpp>
#include <audio/server/AudioClient.hpp>
#include <audio/server/AudioServer.hpp>
#include <audio/server/IOAudioProcess.hpp>

using namespace ctoot::audio::server;
using namespace std;

NonRealTimeAudioServer::NonRealTimeAudioServer(weak_ptr<AudioServer> server) 
{
	this->server = server;
	realTime = true;
	isRunning_ = false;
	startASAP = false;
}

void NonRealTimeAudioServer::setWeakPtr(shared_ptr<NonRealTimeAudioServer> sharedPtr) {
	me = sharedPtr;
}

void NonRealTimeAudioServer::setRealTime(bool rt)
{
	if (!isRunning()) {
		realTime = rt;
		return;
	}
	if (realTime != rt) {
		try {
			stop();
		}
		catch (const exception& e) {
		}
		realTime = rt;
		for (auto& buffer : server.lock()->getBuffers()) {
			buffer->setRealTime(realTime);
		}
		try {
			start();
		}
		catch (exception* e) {
		}
	}
}

bool NonRealTimeAudioServer::isRealTime()
{
    return realTime;
}

void NonRealTimeAudioServer::start()
{
	if (isRunning())
		return;

	if (realTime) {
		auto lServer = server.lock();
		lServer->start();
	}
	else {
		startNRT();
	}
}

void NonRealTimeAudioServer::startNRT()
{
	if (!client.lock()) {
		startASAP = true;
		return;
	}
	if (nrtThread.joinable()) nrtThread.join();
	nrtThread = std::thread(&NonRealTimeAudioServer::static_nrts, this);
}

void NonRealTimeAudioServer::static_nrts(void * args)
{
	static_cast<NonRealTimeAudioServer*>(args)->run();
}

void NonRealTimeAudioServer::stop()
{
    if(!isRunning())
        return;

    if(realTime) {
		auto lServer = server.lock();
        lServer->stop();
    } else if(isRunning_) {
        stopNRT();
    }
}

void NonRealTimeAudioServer::stopNRT()
{
    isRunning_ = false;
}

void NonRealTimeAudioServer::close()
{
	server.lock()->close();
}

void NonRealTimeAudioServer::setClient(weak_ptr<AudioClient> client)
{
	auto lServer = server.lock();
	lServer->setClient(me);
    this->client = client;
    if(startASAP) {
        startASAP = false;
        start();
    }
}

bool NonRealTimeAudioServer::isRunning()
{
	auto lServer = server.lock();
    return realTime ? lServer->isRunning() : isRunning_;
}

float NonRealTimeAudioServer::getLoad()
{
	auto lServer = server.lock();
    return realTime ? lServer->getLoad() : isRunning_ ? 1.0f : 0.0f;
}

void NonRealTimeAudioServer::setEnabled(bool enable)
{
}

void NonRealTimeAudioServer::work(int nFrames)
{
	auto lClient = client.lock();
    if (lClient) lClient->work(nFrames);
}

void NonRealTimeAudioServer::run()
{
    isRunning_ = true;
    while (isRunning_) {
        //work(server.lock()->getBuffers().at(0)->getSampleCount());
		work(512);
		//std::this_thread::yield();
    }
}
void NonRealTimeAudioServer::removeAudioBuffer(ctoot::audio::core::AudioBuffer* buffer) {
	auto lServer = server.lock();
	lServer->removeAudioBuffer(buffer);
}

ctoot::audio::core::AudioBuffer* NonRealTimeAudioServer::createAudioBuffer(string name)
{
	auto lServer = server.lock();
	auto buffer = lServer->createAudioBuffer(name);
	return buffer;
}

vector<string> NonRealTimeAudioServer::getAvailableOutputNames()
{
	auto lServer = server.lock();
    return lServer->getAvailableOutputNames();
}

vector<string> NonRealTimeAudioServer::getAvailableInputNames()
{
	auto lServer = server.lock();
    return lServer->getAvailableInputNames();
}

IOAudioProcess* NonRealTimeAudioServer::openAudioOutput(string name, string label)
{
	auto lServer = server.lock();
    return lServer->openAudioOutput(name, label);
}

IOAudioProcess* NonRealTimeAudioServer::openAudioInput(string name, string label)
{
    return server.lock()->openAudioInput(name, label);
}

void NonRealTimeAudioServer::closeAudioOutput(IOAudioProcess* output)
{
    server.lock()->closeAudioOutput(output);
}

void NonRealTimeAudioServer::closeAudioInput(IOAudioProcess* input)
{
    server.lock()->closeAudioInput(input);
}

float NonRealTimeAudioServer::getSampleRate()
{
    return server.lock()->getSampleRate();
}

int NonRealTimeAudioServer::getInputLatencyFrames()
{
    return server.lock()->getInputLatencyFrames();
}

int NonRealTimeAudioServer::getOutputLatencyFrames()
{
    return server.lock()->getOutputLatencyFrames();
}

int NonRealTimeAudioServer::getTotalLatencyFrames()
{
    return server.lock()->getTotalLatencyFrames();
}

void NonRealTimeAudioServer::resizeBuffers(int newSize) {
	server.lock()->resizeBuffers(newSize);
}

NonRealTimeAudioServer::~NonRealTimeAudioServer() {
	if (nrtThread.joinable()) nrtThread.join();
}
