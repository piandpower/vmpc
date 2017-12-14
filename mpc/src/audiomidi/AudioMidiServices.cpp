#include <audiomidi/AudioMidiServices.hpp>
//#include <StartUp.hpp>
#include <Mpc.hpp>
//#include <audiomidi/SetupServer.hpp>
//#include <audiomidi/DirectToDiskSettings.hpp>
#include <audiomidi/ExportAudioProcessAdapter.hpp>
#include <audiomidi/MpcMidiPorts.hpp>
#include <ui/sampler/SamplerGui.hpp>
//#include <ui/vmpc/AudioObserver.hpp>
#include <nvram/NvRam.hpp>
#include <nvram/AudioMidiConfig.hpp>
#include <sampler/Sampler.hpp>
#include <sequencer/Sequencer.hpp>
#include <ctootextensions/Voice.hpp>
#include <ctootextensions/DummyAudioProcess.hpp>
#include <ctootextensions/MpcBasicSoundPlayerChannel.hpp>
#include <ctootextensions/MpcBasicSoundPlayerControls.hpp>
#include <ctootextensions/MpcFaderControl.hpp>
#include <ctootextensions/MpcMixerControls.hpp>
#include <ctootextensions/MpcMultiMidiSynth.hpp>
#include <ctootextensions/MpcMultiSynthControls.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>
#include <ctootextensions/MpcSoundPlayerControls.hpp>
#include <synth/MidiChannel.hpp>
#include <audio/core/AudioFormat.hpp>
#include <audio/core/AudioControlsChain.hpp>
#include <audio/core/AudioProcess.hpp>
#include <audio/core/ChannelFormat.hpp>
#include <audio/mixer/AudioMixer.hpp>
#include <audio/mixer/AudioMixerBus.hpp>
#include <audio/mixer/AudioMixerStrip.hpp>
#include <audio/mixer/MixerControlsFactory.hpp>
#include <audio/server/CompoundAudioClient.hpp>
#include <audio/server/IOAudioProcess.hpp>
#include <audio/server/NonRealTimeAudioServer.hpp>
#include <audio/system/AudioDevice.hpp>
#include <audio/system/AudioOutput.hpp>
#include <audio/system/AudioSystem.hpp>
#include <audio/system/DefaultAudioSystem.hpp>
#include <audio/system/MixerConnectedAudioSystem.hpp>
#include <control/CompoundControl.hpp>
#include <control/Control.hpp>
#include <midi/core/MidiSystem.hpp>
#include <midi/core/ConnectedMidiSystem.hpp>
#include <midi/core/DefaultConnectedMidiSystem.hpp>
#include <synth/MidiSynth.hpp>
#include <synth/SynthChannel.hpp>
#include <synth/SynthChannelControls.hpp>
#include <synth/SynthRack.hpp>
#include <synth/SynthRackControls.hpp>

#include <audio/mixer/MixerControls.hpp>
#include <control/CompoundControl.hpp>
#include <audio/core/AudioControlsChain.hpp>

#include <audio/server/PluginAudioServer.hpp>
#include <audio/server/DirectSoundServer.hpp>
#include <audio/server/AsioServer.hpp>
#include <audio/server/CoreAudioServer.hpp>
#include <audio/server/Scanner.hpp>

//#include <audio/server/SilentAudioServer.hpp>
#include <audio/server/UnrealAudioServer.hpp>
#include <audio/server/RtAudioServer.hpp>

#include <midi/core/LegacyDevices.hpp>

#include <file/File.hpp>
#include <file/FileUtil.hpp>
#include <lang/StrUtil.hpp>

#include <Logger.hpp>

#include <cmath>
#include <string>

using namespace mpc::audiomidi;
using namespace std;

AudioMidiServices::AudioMidiServices(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	frameSeq = make_shared<mpc::sequencer::FrameSeq>(mpc);
	disabled = true;
	selectedInputs = vector<int>{ 0, -1 };
	selectedOutputs = vector<int>{ 0, -1, -1, -1, -1 };
	format = make_shared<ctoot::audio::core::AudioFormat>(44100, 16, 2, true, false);
	dummyProcess = make_shared<ctootextensions::DummyAudioProcess>();
	auto scanner = ctoot::audio::server::Scanner();
	scanner.scan();
	directSoundInDevNames = scanner.getDirectSoundInputDeviceNames();
	directSoundOutDevNames = scanner.getDirectSoundOutputDeviceNames();
	asioDeviceNames = scanner.getAsioDeviceNames();
	coreAudioOutDeviceNames = scanner.getCoreAudioOutDeviceNames();
	coreAudioInDeviceNames = scanner.getCoreAudioInDeviceNames();
	initServerNames();
}

void AudioMidiServices::start(std::string mode) {
	//requestedBufferSize = 4096;
	setupMidi();

	if (mode.compare("rtaudio") == 0) {
		server = make_shared<ctoot::audio::server::RtAudioServer>();
	}
	else if (mode.compare("unreal") == 0) {
		server = make_shared<ctoot::audio::server::UnrealAudioServer>();
	}
	offlineServer = make_shared<ctoot::audio::server::NonRealTimeAudioServer>(server);
	setupMixer();
	inputProcesses = vector<ctoot::audio::server::IOAudioProcess*>(2);
	outputProcesses = vector<ctoot::audio::server::IOAudioProcess*>(5);

	selectedInputs = { 0, -1 };
	selectedOutputs = { 0, -1, -1, -1, -1 };

	for (auto& p : inputProcesses)
		p = nullptr;

	for (auto& p : outputProcesses)
		p = nullptr;

	for (int i = 0; i < inputProcesses.size(); i++) {
		if (selectedInputs[i] == -1) continue;
		inputProcesses[i] = server->openAudioInput(getInputNames()[selectedInputs[i]], "mpc_in" + to_string(i));
	}

	for (int i = 0; i < outputProcesses.size(); i++) {
		if (selectedOutputs[i] == -1) continue;
		outputProcesses[i] = server->openAudioOutput(getOutputNames()[selectedOutputs[i]], "mpc_out" + to_string(i));
	}

	createSynth();
	if (oldPrograms.size() != 0) {
		for (int i = 0; i < 4; i++) {
			mpc->getDrum(i)->setProgram(oldPrograms[i]);
		}
	}
	connectVoices();
	mpcMidiPorts = make_shared<MpcMidiPorts>(midiSystem, mpc);
	mpcMidiPorts->setMidiIn1(-1);
	mpcMidiPorts->setMidiIn2(-1);
	mpcMidiPorts->setMidiOutA(-1);
	mpcMidiPorts->setMidiOutB(-1);
	auto sampler = mpc->getSampler().lock();
	sampler->setActiveInput(inputProcesses[mpc->getUis().lock()->getSamplerGui()->getInput()]);
	mixer->getStrip("66").lock()->setInputProcess(sampler->getAudioOutputs()[0]);
	initializeDiskWriter();
	cac = make_shared<ctoot::audio::server::CompoundAudioClient>();
	cac->add(frameSeq.get());
	cac->add(mixer.get());
	cac->add(midiSystem.get());
	cac->add(sampler.get());
	offlineServer->setWeakPtr(offlineServer);
	offlineServer->setClient(cac);
	offlineServer->resizeBuffers(4096);
	offlineServer->start(); // <---- niet cool
	disabled = false;
	//this_thread::sleep_for(chrono::milliseconds(6000));
	//offlineServer->stop();
}

void AudioMidiServices::initServerNames()
{
	serverNames.clear();
	if (isStandalone()) {
#ifdef OS_WIN
		serverNames.push_back("DirectSound");
		for (int i = 0; i < asioDeviceNames.size(); i++)
			serverNames.push_back(asioDeviceNames[i]);
#else
		for (int i = 0; i<coreAudioOutDeviceNames.size(); i++) {
			serverNames.push_back(coreAudioOutDeviceNames[i]);
		}
#endif
	}
	else {
		serverNames.push_back("host -- not configurable");
	}
}

void AudioMidiServices::init()
{
	string serverName("");
	int bufferSize = INT_MAX;
	//if (isStandalone()) mpc::nvram::AudioMidiConfig::load(serverName, bufferSize);

	int index = -1;
	for (int i = 0; i < serverNames.size(); i++) {
		if (moduru::lang::StrUtil::trim(serverNames[i]).compare(moduru::lang::StrUtil::trim(serverName)) == 0) {
			index = i;
			break;
		}
	}

	if (bufferSize > 16384) bufferSize = -1;
	if (index == -1) index = 0;
	requestedBufferSize = bufferSize;
	MLOG("bufferSize from config.vmp " + to_string(bufferSize));
	setDriver(index);
	if (isStandalone()) {
		loadIOConfig();
		requestedBufferSize = bufferSize;
		setDriver(index);
	}
}

void AudioMidiServices::create(int newServerIndex, int bufferSize)
{
	requestedBufferSize = bufferSize;
	setupMidi();
	setupServer(newServerIndex);
	serverIndex = newServerIndex;
	setupMixer();
	openIO(newServerIndex);
	createSynth();
	if (oldPrograms.size() != 0) {
		for (int i = 0; i < 4; i++) {
			mpc->getDrum(i)->setProgram(oldPrograms[i]);
		}
	}
	connectVoices();
	/*
	mpcMidiPorts = make_shared<MpcMidiPorts>(midiSystem, mpc);
	mpcMidiPorts->setMidiIn1(-1);
	mpcMidiPorts->setMidiIn2(-1);
	mpcMidiPorts->setMidiOutA(-1);
	mpcMidiPorts->setMidiOutB(-1);
	*/
	auto sampler = mpc->getSampler().lock();
	//sampler->setActiveInput(inputProcesses[mpc->getGui().lock()->getSamplerGui()->getInput()]);
	mixer->getStrip("66").lock()->setInputProcess(sampler->getAudioOutputs()[0]);
	initializeDiskWriter();
	cac = make_shared<ctoot::audio::server::CompoundAudioClient>();
	cac->add(frameSeq.get());
	cac->add(mixer.get());
	cac->add(midiSystem.get());
	cac->add(sampler.get());
	offlineServer->setWeakPtr(offlineServer);
	offlineServer->setClient(cac);
	offlineServer->start();
}

void AudioMidiServices::setupServer(int index)
{
	bool standalone = isStandalone();
#ifdef __APPLE__
	if (standalone) {
		server = make_shared<ctoot::audio::server::CoreAudioServer>(index, index);
		//server->setRequestedBufferSize(requestedBufferSize == -1 ? 128 : requestedBufferSize);
		//server->resizeBuffers(server->getRequestedBufferSize());
	}
#else
	if (index == 0 && standalone) {
		const int devIn = selectedInputs[0];
		const int devOut = selectedOutputs[0];
		const string inputName = devIn == -1 ? "none" : directSoundInDevNames[devIn];
		const string outputName = devOut == -1 ? "none" : directSoundOutDevNames[devOut];
		server = make_shared<ctoot::audio::server::DirectSoundServer>(inputName, outputName);
		//server->setRequestedBufferSize(requestedBufferSize == -1 ? 4096 : requestedBufferSize);
		//		server->resizeBuffers(server->getRequestedBufferSize());
	}
	else if (index >= 1 && standalone) {
		server = make_shared<ctoot::audio::server::AsioServer>(index - 1);
		//server->setRequestedBufferSize(requestedBufferSize == -1 ? 4096 : requestedBufferSize);
		//server->resizeBuffers(server->getRequestedBufferSize());
	}
#endif

	if (serverNames[index].find("host") != string::npos) {
		server = make_shared<ctoot::audio::server::PluginAudioServer>(4, 10); // 4 mono in, 10 mono out, like real mpc2000xl
																			  //server->setRequestedBufferSize(requestedBufferSize == -1 ? 4096 : requestedBufferSize);
																			  //MLOG("NInChannels() " + to_string(gui.lock()->getIPlugBase()->NInChannels()) + "\n");
	}
	requestedBufferSize = -1;
	offlineServer = make_shared<ctoot::audio::server::NonRealTimeAudioServer>(server);
}

void AudioMidiServices::setupMidi()
{
	midiSystem = make_shared<ctoot::midi::core::DefaultConnectedMidiSystem>();
	if (isStandalone()) {
		ctoot::midi::core::LegacyDevices::installPlatformPorts(midiSystem.get());
	}
}

bool AudioMidiServices::isStandalone() {
	//string apiString = gui.lock()->getIPlugBase()->GetAPIString();
	//return apiString.find("Standalone") != string::npos;
	return true;
}

ctoot::audio::server::NonRealTimeAudioServer* AudioMidiServices::getOfflineServer() {
	return offlineServer.get();
}

weak_ptr<ctoot::audio::server::AudioServer> AudioMidiServices::getAudioServer() {
	return offlineServer;
}

weak_ptr<ctoot::audio::server::PluginAudioServer> AudioMidiServices::getPluginAudioServer() {
	return dynamic_pointer_cast<ctoot::audio::server::PluginAudioServer>(server);
}

void AudioMidiServices::setupMixer()
{
	mixerControls = make_shared<ctootextensions::MpcMixerControls>("MpcMixerControls", 1.f);
	int nMixerChans = 66;
	mixerControls->createAuxBusControls("AUX#1", ctoot::audio::core::ChannelFormat::STEREO());
	mixerControls->createAuxBusControls("AUX#2", ctoot::audio::core::ChannelFormat::STEREO());
	mixerControls->createAuxBusControls("AUX#3", ctoot::audio::core::ChannelFormat::STEREO());
	mixerControls->createAuxBusControls("AUX#4", ctoot::audio::core::ChannelFormat::STEREO());
	ctoot::audio::mixer::MixerControlsFactory::createBusStrips(mixerControls, "L-R", ctoot::audio::core::ChannelFormat::STEREO(), 2);
	ctoot::audio::mixer::MixerControlsFactory::createChannelStrips(mixerControls, nMixerChans);
	mixer = make_shared<ctoot::audio::mixer::AudioMixer>(mixerControls, offlineServer);
	audioSystem = make_shared<ctoot::audio::system::MixerConnectedAudioSystem>(mixer);
	audioSystem->setAutoConnect(false);
	setMasterLevel(nvram::NvRam::getMasterLevel());
	setRecordLevel(nvram::NvRam::getRecordLevel());
	setGroupLevel(100);
}

void AudioMidiServices::setMasterLevel(int i)
{
	auto sc = mixer->getMixerControls()->getStripControls("L-R");
	auto cc = dynamic_pointer_cast<ctoot::control::CompoundControl>(sc->getControls()[0]);
	dynamic_pointer_cast<ctootextensions::MpcFaderControl>(cc->getControls()[2])->setValue(i);
}

int AudioMidiServices::getMasterLevel() {
	auto sc = mixer->getMixerControls()->getStripControls("L-R");
	auto cc = dynamic_pointer_cast<ctoot::control::CompoundControl>(sc->getControls()[0]);
	auto val = dynamic_pointer_cast<ctootextensions::MpcFaderControl>(cc->getControls()[2])->getValue();
	return (int) dynamic_pointer_cast<ctootextensions::MpcFaderControl>(cc->getControls()[2])->getValue();
}

void AudioMidiServices::setRecordLevel(int i) {
	mpc->getSampler().lock()->setInputLevel(i);
}

int AudioMidiServices::getRecordLevel() {
	return mpc->getSampler().lock()->getInputLevel();
}

void AudioMidiServices::setGroupLevel(int i)
{
	for (auto j = 1; j <= 4; j++) {
		auto sc1 = mixer->getMixerControls()->getStripControls("AUX#" + to_string(j));
		auto cc1 = dynamic_pointer_cast<ctoot::control::CompoundControl>(sc1->getControls()[0]);
		dynamic_pointer_cast<ctootextensions::MpcFaderControl>(cc1->getControls()[2])->setValue(i);
	}
}

void AudioMidiServices::openIO(int index)
{
	inputProcesses = vector<ctoot::audio::server::IOAudioProcess*>(2);
	outputProcesses = vector<ctoot::audio::server::IOAudioProcess*>(5);

	bool plug = !isStandalone();

	if (plug) {
		selectedInputs = { 0, 1 };
		selectedOutputs = { 0, 1, 2, 3, 4 };
	}

	for (auto& p : inputProcesses)
		p = nullptr;

	for (auto& p : outputProcesses)
		p = nullptr;
	for (int i = 0; i < inputProcesses.size(); i++) {
		if (selectedInputs[i] == -1) continue;
#ifdef __APPLE__
		inputProcesses[i] = server->openAudioInput(getInputNames()[selectedInputs[i]], "mpc_in" + to_string(i));
#else
		if (i > 0 && index == 0 && !plug) break;
		if (index == 0 && !plug) {
			inputProcesses[i] = server->openAudioInput(directSoundInDevNames[selectedInputs[i]], "mpc_in" + to_string(i));
		}
		else if (index >= 1 && !plug) {
			inputProcesses[i] = server->openAudioInput(getInputNames()[selectedInputs[i]], "mpc_in" + to_string(i));
		}
		else if (plug) {
			inputProcesses[i] = server->openAudioInput(getInputNames()[selectedInputs[i]], "mpc_in" + to_string(i));
		}
#endif
		if (inputProcesses[i] == nullptr) {
			selectedInputs[i] = -1;
		}
	}

	for (int i = 0; i < outputProcesses.size(); i++) {
		if (i > 0 && index == 0 && !plug) break;
		if (selectedOutputs[i] == -1) continue;
#ifdef __APPLE__
		outputProcesses[i] = server->openAudioOutput(getOutputNames()[selectedOutputs[i]], "mpc_out" + to_string(i));
#else
		if (index == 0 && !plug) {
			outputProcesses[i] = server->openAudioOutput(directSoundOutDevNames[selectedOutputs[i]], "mpc_out" + to_string(i));
		}
		else if (index >= 1 && !plug) {
			outputProcesses[i] = server->openAudioOutput(getOutputNames()[selectedOutputs[i]], "mpc_out" + to_string(i));
		}
		else if (plug) {
			outputProcesses[i] = server->openAudioOutput(getOutputNames()[selectedOutputs[i]], "mpc_out" + to_string(i));
		}
#endif
		if (outputProcesses[i] == nullptr) {
			selectedOutputs[i] = -1;
		}
	}
}

vector<string> AudioMidiServices::getInputNames()
{
	if (!server) return vector<string>{"<disabled>"};
#ifndef __APPLE__
	if (serverIndex == 0 && isStandalone()) return directSoundInDevNames;
#endif
	return server->getAvailableInputNames();
}

vector<string> AudioMidiServices::getOutputNames()
{
	if (!server) return vector<string>{"<disabled>"};
#ifndef __APPLE__
	if (serverIndex == 0 && isStandalone()) return directSoundOutDevNames;
#endif
	return server->getAvailableOutputNames();
}

weak_ptr<mpc::ctootextensions::MpcMultiMidiSynth> AudioMidiServices::getMms()
{
	return mms;
}

void AudioMidiServices::createSynth()
{
	synthRackControls = make_shared<ctoot::synth::SynthRackControls>(1);
	synthRack = make_shared<ctoot::synth::SynthRack>(synthRackControls, midiSystem, audioSystem);
	msc = make_shared<ctootextensions::MpcMultiSynthControls>();
	synthRackControls->setSynthControls(0, msc);
	mms = dynamic_pointer_cast<mpc::ctootextensions::MpcMultiMidiSynth>(synthRack->getMidiSynth(0).lock());
	vector<weak_ptr<mpc::ctootextensions::Voice>> weakVoices;
	for (int i = 1; i <= 32; i++) {
		voices.push_back(std::move(make_shared<mpc::ctootextensions::Voice>(i, false)));
		weakVoices.push_back(voices.back());
		voices.back()->setWeakThis(voices.back());
	}
	for (int i = 0; i < 4; i++) {
		auto m = make_shared<ctootextensions::MpcSoundPlayerControls>(mpc->getSampler(), i, mixer, weakVoices, server);
		msc->setChannelControls(i, m);
		synthChannelControls.push_back(m);
	}
	basicVoice = make_shared<mpc::ctootextensions::Voice>(65, true);
	auto m = make_shared<mpc::ctootextensions::MpcBasicSoundPlayerControls>(mpc->getSampler(), mixer, basicVoice);
	msc->setChannelControls(4, m);
	synthChannelControls.push_back(std::move(m));
}

void AudioMidiServices::connectVoices()
{
	mpc->getDrums()[0]->connectVoices();
	mpc->getBasicPlayer()->connectVoice();
}

weak_ptr<MpcMidiPorts> AudioMidiServices::getMidiPorts()
{
return mpcMidiPorts;
}

int AudioMidiServices::getServerIndex()
{
	return serverIndex;
}

int AudioMidiServices::getServerCount() {
	return (int)serverNames.size();
}

string AudioMidiServices::getServerName(int i) {
	if (i == -1) return "<disabled>";
	return serverNames[i];
}

string AudioMidiServices::getCurrentServerName() {
	return serverNames[serverIndex];
}

bool AudioMidiServices::isDirectSound()
{
#ifdef OS_WIN
	if (isStandalone() && serverIndex == 0) return true;
#endif
	return false;
}

bool AudioMidiServices::isAsio()
{
#ifdef OS_WIN
	if (isStandalone() && serverIndex >= 1) return true;
#endif
	return false;
}

void AudioMidiServices::openAsioControlPanel() {

}

bool AudioMidiServices::isCoreAudio()
{
#ifdef OS_WIN
	return false;
#endif
	if (isStandalone()) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

void AudioMidiServices::initializeDiskWriter()
{
	exportProcesses.clear();
	std::shared_ptr<ExportAudioProcessAdapter> diskWriter;
	if (outputProcesses[0] != nullptr) {
		diskWriter = make_shared<ExportAudioProcessAdapter>(outputProcesses[0], format, "diskwriter");
	}
	else {
		diskWriter = make_shared<ExportAudioProcessAdapter>(dummyProcess.get(), format, "diskwriter");
	}
	exportProcesses.push_back(diskWriter);
	mixer->getMainBus()->setOutputProcess(diskWriter.get());
	for (int i = 1; i <= 4; i++) {
		if (outputProcesses[i] != nullptr) {
			diskWriter = make_shared<ExportAudioProcessAdapter>(outputProcesses[i], format, "diskwriter");
		}
		else {
			diskWriter = make_shared<ExportAudioProcessAdapter>(dummyProcess.get(), format, "diskwriter");
		}
		mixer->getStrip(string("AUX#" + to_string(i))).lock()->setDirectOutputProcess(diskWriter.get());
		exportProcesses.push_back(diskWriter);
	}
}

void AudioMidiServices::destroyServices()
{
	MLOG("Trying to destroy services...");
	if (disabled) {
		return;
	}
	offlineServer->stop();
	cac.reset();
	destroyDiskWriter();
	mpc->getSampler().lock()->setActiveInput(nullptr);
	mixer->getStrip("66").lock()->setInputProcess(nullptr);
	mpcMidiPorts->close();
	mpcMidiPorts.reset();
	destroySynth();
	closeIO();
	inputProcesses.clear();
	outputProcesses.clear();

	if (audioSystem) {
		audioSystem->close();
		audioSystem.reset();
	}

	mixer->close();
	mixer.reset();
	mixerControls.reset();
	offlineServer->close();
	offlineServer.reset();
	server.reset();
	midiSystem->close();
	midiSystem.reset();
}

void AudioMidiServices::destroySynth() {
	oldPrograms = vector<int>(4);
	for (int i = 0; i < 4; i++) {
		oldPrograms[i] = mpc->getDrum(i)->getProgram();
	}
	for (auto& s : synthChannelControls) {
		s.reset();
	}
	synthChannelControls.clear();
	voices.clear();
	basicVoice.reset();
	msc.reset();

	synthRack->close();
	synthRack.reset();

	synthRackControls.reset();
}

void AudioMidiServices::destroyDiskWriter() {
	for (auto& eapa : exportProcesses) {
		eapa.reset();
	}
}

void AudioMidiServices::setDriver(int i)
{
	if (i == -1) {
		serverIndex = i;
		destroyServices();
		disable();
		return;
	}
	auto prevDriverIndex = serverIndex;
	if (server) {
		destroyServices();
	}

	auto firstAttemptSuccess = false;
	try {
		create(i, requestedBufferSize);
		firstAttemptSuccess = true;
	}
	catch (exception e) {
		firstAttemptSuccess = false;
		string logmsg = e.what();
	}

	if (!firstAttemptSuccess) {
		if (prevDriverIndex >= 0) {
			try {
				create(prevDriverIndex, -1);
				serverIndex = prevDriverIndex;
				return;
			}
			catch (exception* e1) {
				e1->what();
				disable();
				return;
			}
		}
		else {
			serverIndex = -1;
			disable();
			return;
		}
	}
	disabled = false;
	serverIndex = i;
}

void AudioMidiServices::closeIO()
{
	for (auto j = 0; j < 2; j++) {
		if (inputProcesses[j] == nullptr) {
			continue;
		}
		server->closeAudioInput(inputProcesses[j]);
	}
	for (auto j = 0; j < 5; j++) {
		if (outputProcesses[j] == nullptr) {
			continue;
		}
		server->closeAudioOutput(outputProcesses[j]);
	}
}

void AudioMidiServices::prepareBouncing(DirectToDiskSettings* settings)
{
	auto indivFileNames = std::vector<string>{ "L-R", "1-2", "3-4", "5-6", "7-8" };
	string sep = moduru::file::FileUtil::getSeparator();
	for (int i = 0; i < 5; i++) {
		auto eapa = exportProcesses[i];
		//auto file = new moduru::file::File(mpc::StartUp::home + sep + "vMPC" + sep + "recordings" + sep + indivFileNames[i], nullptr);
		//eapa->prepare(file, settings->lengthInFrames);
	}
	bouncePrepared = true;
}

void AudioMidiServices::startBouncing()
{
	if (!bouncePrepared)
		return;

	for (auto& eapa : exportProcesses) {
		//	eapa->start();
	}
	bouncePrepared = false;
	bouncing = true;
}

void AudioMidiServices::stopBouncing()
{
	if (!bouncing) return;

	for (auto& eapa : exportProcesses) {
		//eapa->stop();
	}
	for (auto& eapa : exportProcesses) {
		//eapa->writeWav();
	}
	//exportProcesses.clear();
	mpc->getSequencer().lock()->stop();
	//gui.lock()->getMainFrame().lock()->openScreen("recordingfinished");
	bouncing = false;
}

weak_ptr<mpc::sequencer::FrameSeq> AudioMidiServices::getFrameSequencer()
{
	return frameSeq;
}

bool AudioMidiServices::isBouncePrepared()
{
	return bouncePrepared;
}

bool AudioMidiServices::isBouncing()
{
	return bouncing;
}

void AudioMidiServices::disable()
{
	disabled = true;
	//inputNames.clear();
	//outputNames.clear();
	//selectedInputs = vector<int>{ 0, -1 };
	//selectedOutputs = vector<int>{ 0, 1, -1, -1, -1 };
}

vector<int> AudioMidiServices::getSelectedInputs() {
	return selectedInputs;
}

vector<int> AudioMidiServices::getSelectedOutputs() {
	return selectedOutputs;
}

void AudioMidiServices::setSelectedInput(int virtualInput, int hardwareInput) {
	selectedInputs[virtualInput] = hardwareInput;
}

void AudioMidiServices::setSelectedOutput(int virtualOutput, int hardwareOutput) {
	selectedOutputs[virtualOutput] = hardwareOutput;
}

bool AudioMidiServices::isDisabled()
{
	return disabled;
}

ctoot::audio::server::IOAudioProcess* AudioMidiServices::getAudioInput(int input)
{
	return inputProcesses[input];
}

void AudioMidiServices::setBufferSize(int size)
{
	if (size < 32) return;
	if (size > 16384) return;
	//server->setRequestedBufferSize(size);
	//if (server->getRequestedBufferSize() != server->getBufferSize()) {
	//if (isDirectSound()) {
	//	dynamic_pointer_cast<ctoot::audio::server::DirectSoundServer>(server)->getRtAudio()->stopStream();
	//	server->resizeBuffers(server->getRequestedBufferSize());
	//	dynamic_pointer_cast<ctoot::audio::server::DirectSoundServer>(server)->startStream();
	//}
	//else if (isAsio()) {
	//	dynamic_pointer_cast<ctoot::audio::server::AsioServer>(server)->getRtAudio()->stopStream();
	//	server->resizeBuffers(server->getRequestedBufferSize());
	//	dynamic_pointer_cast<ctoot::audio::server::AsioServer>(server)->startStream();
	//}
	//else if (isCoreAudio()) {
	//	dynamic_pointer_cast<ctoot::audio::server::CoreAudioServer>(server)->getRtAudio()->stopStream();
	//	server->resizeBuffers(server->getRequestedBufferSize());
	//	dynamic_pointer_cast<ctoot::audio::server::CoreAudioServer>(server)->startStream();
	//}
	//else { // assume plugin and do nothing
	//}
	//}

	setChanged();
	notifyObservers(string("buffersize"));
}

int AudioMidiServices::getBufferSize()
{
	return server->getOutputLatencyFrames();
}

int AudioMidiServices::getDirectSoundInputDevCount() {
	return directSoundInDevNames.size();
}

int AudioMidiServices::getDirectSoundOutputDevCount() {
	return directSoundOutDevNames.size();
}

void AudioMidiServices::saveConfig() {
	if (!isStandalone()) return;
	vector<string> names;
	names.push_back(getCurrentServerName());
	auto inputNames = getInputNames();
	auto outputNames = getOutputNames();
	for (auto& i : selectedInputs) {
		if (i == -1) {
			names.push_back("<disabled>");
			continue;
		}
		names.push_back(inputNames[i]);
	}
	for (auto& i : selectedOutputs) {
		if (i == -1) {
			names.push_back("<disabled>");
			continue;
		}
		names.push_back(outputNames[i]);
	}
	//mpc::nvram::AudioMidiConfig::save(names, getBufferSize());
}

void AudioMidiServices::loadIOConfig() {
	vector<string> devNames;
	//mpc::nvram::AudioMidiConfig::loadIO(&devNames);
	if (devNames.size() == 0) return;
	auto inputNames = getInputNames();
	auto outputNames = getOutputNames();
	for (int i = 0; i < 2; i++) {
		selectedInputs[i] = -1;
		if (devNames[i].compare("<disabled>") == 0) {
			continue;
		}
		for (int j = 0; j < inputNames.size(); j++) {
			if (inputNames[j].compare(devNames[i]) == 0) {
				selectedInputs[i] = j;
				break;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		selectedOutputs[i] = -1;
		if (devNames[i + 2].compare("<disabled>") == 0) {
			continue;
		}
		for (int j = 0; j < outputNames.size(); j++) {
			if (outputNames[j].compare(devNames[i + 2]) == 0) {
				selectedOutputs[i] = j;
				break;
			}
		}
	}
}

ctoot::audio::server::UnrealAudioServer* AudioMidiServices::getUnrealAudioServer() {
	return dynamic_pointer_cast<ctoot::audio::server::UnrealAudioServer>(server).get();
}

ctoot::audio::server::RtAudioServer* AudioMidiServices::getRtAudioServer() {
	return dynamic_pointer_cast<ctoot::audio::server::RtAudioServer>(server).get();
}

AudioMidiServices::~AudioMidiServices() {
}
