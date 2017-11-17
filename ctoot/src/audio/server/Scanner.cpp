#include "Scanner.hpp"

#include <Logger.hpp>

using namespace std;
using namespace ctoot::audio::server;

void Scanner::scan() {
#ifdef __APPLE__
    scanCoreAudio();
#else
	scanDirectSound();
	scanAsio();
#endif
}

void Scanner::scanCoreAudio() {
 /*   coreAudioInDeviceNames.clear();
    coreAudioOutDeviceNames.clear();
    auto rtAudio = new RtAudio(RtAudio::Api::MACOSX_CORE);
	int devCount = rtAudio->getDeviceCount();
	for (int i = 0; i < devCount; i++) {
		auto info = rtAudio->getDeviceInfo(i);
        if (!info.probed) continue;
		if (info.inputChannels > 0) coreAudioInDeviceNames.push_back(info.name);
		if (info.outputChannels > 0) coreAudioOutDeviceNames.push_back(info.name);
    }
    delete rtAudio;*/
}

vector<string> Scanner::getCoreAudioInDeviceNames() {
    return coreAudioInDeviceNames;
}

vector<string> Scanner::getCoreAudioOutDeviceNames() {
    return coreAudioOutDeviceNames;
}

void Scanner::scanDirectSound() {
	directSoundDeviceNames.clear();
	directSoundInputDevices.clear();
	directSoundOutputDevices.clear();

	//auto rtAudio = new RtAudio(RtAudio::Api::WINDOWS_DS);
	//int devCount = rtAudio->getDeviceCount();
	//for (int i = 0; i < devCount; i++) {
	//	auto info = rtAudio->getDeviceInfo(i);
	//	directSoundDeviceNames.push_back(info.name);
	//	if (info.outputChannels > 0) {
	//		directSoundOutputDevices.push_back(i);
	//	}
	//	if (info.inputChannels > 0) {
	//		directSoundInputDevices.push_back(i);
	//	}
	//}
	//delete rtAudio;
}

void Scanner::scanAsio() {
#ifndef __APPLE__
	/*asioDeviceNames.clear();
	auto ad = AsioDrivers();
	int driver_count = ad.asioGetNumDev();
	for (int i = 0; i < driver_count; i++) {
		char driverName[32];
		ad.asioGetDriverName(i, driverName, 32);
		string tmp(driverName);
		asioDeviceNames.push_back(tmp);
	}*/
#endif
}

vector<string> Scanner::getDirectSoundInputDeviceNames() {
	vector<string> res;
	for (auto& i : directSoundInputDevices) {
		res.push_back(directSoundDeviceNames[i]);
	}
	return res;
}

vector<string> Scanner::getDirectSoundOutputDeviceNames() {
	vector<string> res;
	for (auto& i : directSoundOutputDevices) {
		res.push_back(directSoundDeviceNames[i]);
	}
	return res;
}

int Scanner::getDirectSoundDeviceID(string name) {
	int res = -1;
	for (int i = 0; i < directSoundDeviceNames.size(); i++) {
		if (directSoundDeviceNames[i].compare(name) == 0) {
			return i;
		}
	}
    return res;
}

vector<string> Scanner::getAsioDeviceNames() {
	return asioDeviceNames;
}
