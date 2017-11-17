#include <audio/core/AudioProcessChain.hpp>

#include <audio/core/AudioControls.hpp>
#include <audio/core/AudioControlsChain.hpp>
#include <audio/core/AudioProcess.hpp>
#include <audio/core/ControlChainObserver.hpp>
//#include <audio/core/AudioServices.hpp>
#include <control/ChainMutation.hpp>
#include <control/Control.hpp>

#include <chrono>

using namespace std;
using namespace ctoot::control;
using namespace ctoot::audio::core;

AudioProcessChain::AudioProcessChain(shared_ptr<AudioControlsChain> controlChain)
{
	this->controlChain = controlChain;
	debugTimes = false;
	debugIndex = 0;
	prevNanos = 0;
	controlChainObserver = new ControlChainObserver(this);
}

void AudioProcessChain::open()
{
	for (auto& control : controlChain->getControls()) {
		try {
			if (dynamic_pointer_cast<AudioControls>(control) != nullptr) {
//				auto p = createProcess(dynamic_cast<AudioControls*>(control)); afaik vMPC Java doesn't make use of this. requires services.
//				processes->push_back(p);
//				if (p != nullptr) {
//					p->open();
//				}
			}
		}
		catch (bad_cast e) {
			continue;
		}

	}
	controlChain->addObserver(controlChainObserver);
	if (debugTimes) {
		t.clear();
	}
}

int AudioProcessChain::processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames)
{
	processMutations();
	if (debugTimes) {
		tstart = chrono::high_resolution_clock::now().time_since_epoch().count();
		elapsed = tstart - prevNanos;
		prevNanos = tstart;
		debugIndex += 1;
		debugIndex %= 100;
		if (debugIndex == 0) {
			return debugProcessAudio(buffer, nFrames);
		}
	}
	for (auto& p : processes) {
		try {
			if (p != nullptr) {
				p->processAudio(buffer, nFrames);
			}
		}
		catch (exception* e) {
			try {
				p->close();
			}
			catch (exception* e2) {
				e2->what();
			}
			p = nullptr;
			string error = "DISABLED AudioProcess in " + getName() + " due to: " + e->what();
		}
	}
	return AUDIO_OK;
}

int AudioProcessChain::debugProcessAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames)
{
	auto len = processes.size();
	for (auto i = 0; i < len; i++) {
		processes[i]->processAudio(buffer, nFrames);
		t[i] = chrono::high_resolution_clock::now().time_since_epoch().count();
	}
	auto load = static_cast<int>(100 * (t[len - 1] - tstart) / elapsed);
	//    npc(::java::lang::System::out())->print(stringBuilder().append(load)->append(u"%: "_j)->toString());
	auto prevt = tstart;
	for (auto i = 0; i < len; i++) {
		///        npc(::java::lang::System::out())->print(stringBuilder().append(((*t)[i] - prevt))->append(u", "_j)->toString());
		prevt = t[i];
	}
	return AUDIO_OK;
}

void AudioProcessChain::close()
{
	controlChain->deleteObserver(controlChainObserver);
	for (auto& p : processes) {
		if (p == nullptr) continue;
		p->close();
		delete p;
	}
	processes.clear();
	t.clear();
}

string AudioProcessChain::getName()
{
    return controlChain->getName();
}

AudioProcess* AudioProcessChain::createProcess(AudioControls* controls)
{
    //return AudioServices::createProcess(controls);
	return nullptr;
}

void AudioProcessChain::processMutations()
{
	ChainMutation* m = nullptr;
	mutationQueue.try_dequeue(m);
	if (m == nullptr) return;
	AudioProcess* p;
	try {
		shared_ptr<Control> controls;
		AudioProcess* process;
		switch (m->getType()) {
		case ChainMutation::DELETE:
			p = dynamic_cast<AudioProcess*>(processes[m->getIndex0()]);
			p->close();
			for (int i = 0; i < processes.size(); i++) {
				if (processes[i] == p) {
					processes.erase(processes.begin() + i);
					delete p;
					break;
				}
			}
			break;
		case ChainMutation::INSERT:
			controls = dynamic_pointer_cast<Control>(controlChain->getControls()[m->getIndex0()]);
			if (dynamic_pointer_cast<AudioControls>(controls) != nullptr) {
				p = createProcess(dynamic_pointer_cast<AudioControls>(controls).get());
				processes.insert(processes.begin() + m->getIndex0(), p);
				if (p != nullptr) {
					p->open();
				}
			}
			break;
		case ChainMutation::MOVE:
			process = dynamic_cast<AudioProcess*>(processes[m->getIndex0()]);
			processes.erase(processes.begin() + m->getIndex0());
			processes.insert(processes.begin() + m->getIndex1(), process);
			break;
		}
	}
	catch (exception* e) {
		//        string error = "Exception for " + controlChain->getName() + ", Mutation: " + m->toString();
	}
	delete m;
}

AudioProcessChain::~AudioProcessChain() {
	if (controlChainObserver != nullptr) {
		delete controlChainObserver;
		controlChainObserver = nullptr;
	}
	//mutationQueue.t
}
