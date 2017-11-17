#include "ASIOProcess.hpp"

using namespace ctoot::audio::server;
using namespace std;

/*
ASIOProcess::ASIOProcess(string name, AsioChannel info0, AsioChannel info1) {
	this.name = name;
	this.info0 = info0;
	this.info1 = info1; // may be null
	format = info1 == null ? ChannelFormat.MONO : ChannelFormat.STEREO;
}
*/

ChannelFormat* ASIOProcess::getChannelFormat() {
	return format;
}

string ASIOProcess::getName() {
	return name;
}

void ASIOProcess::open() {
	//activeChannels.add(info0);
	if (info1 != nullptr) {
		//activeChannels.add(info1);
	}
}

void ASIOProcess::close() {
	// this won't have any effect until next server stop/start
	// System.out.println("closing " + name);

	//activeChannels.remove(info0);
	if (info1 != nullptr) {
		//activeChannels.remove(info1);
	}
}
