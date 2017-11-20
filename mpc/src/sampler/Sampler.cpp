#include <sampler/Sampler.hpp>
#include <lang/StrUtil.hpp>
#include <Mpc.hpp>
#include <audiomidi/AudioMidiServices.hpp>
#include <disk/SoundLoader.hpp>
#include <maingui/StartUp.hpp>
//#include <hardware/ControlPanel.hpp>
////#include <maingui/Gui.hpp>
//#include <lcdgui/LayeredScreen.hpp>
//#include <lcdgui/LayeredScreen.hpp>
#include <ui/UserDefaults.hpp>
//#include <lcdgui/Background.hpp>
//#include <ui/sampler/SamplerGui.hpp>
//#include <ui/sampler/SoundGui.hpp>
//#include <ui/sequencer/window/SequencerWindowGui.hpp>
#include <sampler/NoteParameters.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>
#include <sampler/MixerChannel.hpp>
#include <sampler/MonitorOutput.hpp>
#include <sampler/Sound.hpp>
#include <sequencer/NoteEvent.hpp>
#include <sequencer/Sequencer.hpp>
#include <sequencer/Track.hpp>
#include <sequencer/Sequence.hpp>
#include <ctootextensions/MpcBasicSoundPlayerChannel.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>
#include <audio/core/ChannelFormat.hpp>
#include <audio/server/IOAudioProcess.hpp>
#include <synth/SynthChannel.hpp>

#include <file/File.hpp>

using namespace mpc::sampler;
using namespace std;

Sampler::Sampler()
{
	this->gui = gui;
	vuCounter = 0;
	vuBufferL = vector<float>(VU_BUFFER_SIZE);
	vuBufferR = vector<float>(VU_BUFFER_SIZE);
	preRecBufferL = boost::circular_buffer<float>(4410);
	preRecBufferR = boost::circular_buffer<float>(4410);
	soundSortingType = 0;
	abcd = vector<string>{ "A", "B", "C", "D" };
	sortNames = vector<string>{ "MEMORY", "NAME", "SIZE" };
	recordBuffer = make_unique<ctoot::audio::core::AudioBuffer>("record", 2, 4096, 44100);
	monitorOutput = make_shared<MonitorOutput>(this, gui);
	auto ud = mpc::maingui::StartUp::getUserDefaults().lock();
	initMasterPadAssign = ud->getPadNotes();

	programs = vector<shared_ptr<Program>>(24);
}

vector<weak_ptr<mpc::sampler::MixerChannel>> Sampler::getDrumMixer(int i)
{
	return mpc->getDrums()[i]->getMixerChannels();
}

const int Sampler::VU_BUFFER_SIZE;

vector<int>* Sampler::getInitMasterPadAssign()
{
	return &initMasterPadAssign;
}

vector<int>* Sampler::getMasterPadAssign()
{
	return &masterPadAssign;
}

void Sampler::setMasterPadAssign(vector<int> v) {
	masterPadAssign = v;
}

vector<int>* Sampler::getAutoChromaticAssign()
{
	return &autoChromaticAssign;
}

void Sampler::work(int nFrames)
{
	/*
	if (input == nullptr && inputSwap == nullptr) return;
	auto lGui = gui.lock();
	if (!lGui) return;
	if (!lGui->getMainFrame().lock()) return;
	auto lMainFrame = lGui->getMainFrame().lock();
	auto ls = lMainFrame->getLayeredScreen().lock();
	if (!ls) return;
	if (ls->getCurrentScreenName().compare("sample") != 0) return;

	auto arm = false;
	if (inputSwap != nullptr) {
		input = inputSwap;
		inputSwap = nullptr;
	}
	if (recordBuffer->getSampleCount() != nFrames) recordBuffer->changeSampleCount(nFrames, false);
	input->processAudio(recordBuffer.get());
	auto leftPairs = recordBuffer->getChannelFormat()->getLeft();
	auto rightPairs = recordBuffer->getChannelFormat()->getRight();
	monitorBufferL = recordBuffer->getChannel(leftPairs[0]);
	monitorBufferR = recordBuffer->getChannel(rightPairs[0]);
	auto controlPanel = lMainFrame->getControlPanel().lock();
	for (int i = 0; i < nFrames; i++) {
		monitorBufferL->at(i) *= controlPanel->getRecord() / 100.0;
		monitorBufferR->at(i) *= controlPanel->getRecord() / 100.0;
		if (armed && abs(monitorBufferL->at(i)) >(lGui->getSamplerGui()->getThreshold() + 64) / 64.0) arm = true;

		if (recording) {
			recordBufferL[recordFrame] = monitorBufferL->at(i);
			recordBufferR[recordFrame++] = monitorBufferR->at(i);
			if (recordFrame == recordBufferL.size()) stopRecording();
		}
		else {
			preRecBufferL.push_back(monitorBufferL->at(i));
			preRecBufferR.push_back(monitorBufferR->at(i));
		}

		if (monitorBufferL->at(i) > 0 && vuCounter++ == 5) {
			vuCounter = 0;
			if (monitorBufferL->at(i) > peakL)
				peakL = monitorBufferL->at(i);

			if (monitorBufferR->at(i) > peakR)
				peakR = monitorBufferR->at(i);

			vuBufferL[vuSampleCounter] = monitorBufferL->at(i) < 0.01 ? 0 : monitorBufferL->at(i);
			vuBufferR[vuSampleCounter++] = monitorBufferR->at(i) < 0.01 ? 0 : monitorBufferR->at(i);
			if (vuSampleCounter == VU_BUFFER_SIZE) {
				vuSampleCounter = 0;
				float highestl = 0;

				for (auto fl : vuBufferL) if (fl > highestl) highestl = fl;

				float highestr = 0;
				for (auto fl : vuBufferR) if (fl > highestr) highestr = fl;

				levelL = highestl;
				levelR = highestr;
				lGui->getSamplerGui()->notify("vumeter");
				vuBufferL = vector<float>(VU_BUFFER_SIZE);
				vuBufferR = vector<float>(VU_BUFFER_SIZE);
			}
		}
	}

	if (arm) {
		arm = false;
		armed = false;
		record();
	}
	*/
}

void Sampler::init(mpc::Mpc* mpc)
{
	this->mpc = mpc;
	auto program = addProgram().lock();;
	program->setName("NewPgm-A");
	for (int i = 0; i < 4; i++) {
		for (auto j = 0; j < 16; j++) {
			string result = "";
			result.append(abcd[i]);
			result.append(moduru::lang::StrUtil::padLeft(to_string(j + 1), "0", 2));
			padNames.push_back(result);
		}
	}

	auto f = make_shared<moduru::file::File>(mpc::maingui::StartUp::resPath + "click.wav", nullptr);
	clickSound = make_shared<Sound>();
	mpc::disk::SoundLoader::getSampleDataFromWav(f, clickSound->getSampleData());
	clickSound->setMono(true);
	clickSound->setLevel(100);
	masterPadAssign = initMasterPadAssign;
	autoChromaticAssign = vector<int>(64);
	for (int i = 0; i < 64; i++)
		autoChromaticAssign[i] = i;
}

void Sampler::playMetronome(mpc::sequencer::NoteEvent* event, int framePos)
{
	auto lGui = gui.lock();
	//auto swGui = lGui->getSequencerWindowGui();
	auto soundNumber = -2;
	/*
	if (swGui->getMetronomeSound() != 0) {
		auto program = mpc->getDrum(swGui->getMetronomeSound() - 1)->getProgram();
		auto accent = event->getVelocity() == swGui->getAccentVelo();
		soundNumber = programs[program]->getNoteParameters(accent ? swGui->getAccentNote() : swGui->getNormalNote())->getSndNumber();
	}
	*/
	mpc->getBasicPlayer()->mpcNoteOn(soundNumber, event->getVelocity(), framePos);
}

void Sampler::playPreviewSample(int start, int end, int loopTo, int overlapMode)
{
	if (sounds.size() == 0) return;
	auto previewSound = sounds[sounds.size() - 1];
	auto oldStart = previewSound->getStart();
	auto oldEnd = previewSound->getEnd();
	auto oldLoopTo = previewSound->getLoopTo();
	previewSound->setStart(start);
	previewSound->setEnd(end);
	previewSound->setLoopTo(loopTo);
	mpc->getBasicPlayer()->noteOn(-3, 127);
	previewSound->setStart(oldStart);
	previewSound->setEnd(oldEnd);
	previewSound->setLoopTo(oldLoopTo);
}

weak_ptr<Program> Sampler::getProgram(int programNumber)
{
	return programs[programNumber];
}

int Sampler::getProgramCount()
{
	int res = 0;
	for (auto& p : programs)
		if (p) res++;
	return res;
}

weak_ptr<Program> Sampler::addProgram(int i) {
	if (programs[i]) return weak_ptr<Program>();
	programs[i] = make_shared<Program>(this);
	return programs[i];
}

weak_ptr<Program> Sampler::addProgram()
{
	for (auto& p : programs) {
		if (!p) {
			p = make_shared<Program>(this);
			return p;
		}
	}
	return weak_ptr<Program>();
}

void Sampler::deleteProgram(weak_ptr<Program> program) {
	auto lProgram = program.lock();
	for (auto&& p : programs) {
		if (p == lProgram) {
			p.reset();
			break;
		}
	}
}

vector<weak_ptr<Sound>> Sampler::getSounds()
{
	auto res = vector<weak_ptr<Sound>>();
	for (auto& s : sounds)
		res.push_back(s);
	return res;
}

weak_ptr<Sound> Sampler::addSound() {
	auto res = make_shared<Sound>();
	sounds.push_back(res);
	return res;
}

weak_ptr<Sound> Sampler::addSound(int sampleRate) {
	auto res = make_shared<Sound>(sampleRate, sounds.size());
	sounds.push_back(res);
	return res;
}

int Sampler::getSoundCount()
{
	return sounds.size();
}

string Sampler::getSoundName(int i)
{
	return sounds[i]->getName();
}

string Sampler::getPadName(int i)
{
	return padNames[i];
}

vector<weak_ptr<Program>> Sampler::getPrograms()
{
	auto res = vector<weak_ptr<Program>>();
	for (auto& p : programs)
		res.push_back(p);
	return res;
}

void Sampler::replaceProgram(weak_ptr<Program> p, int index) {
	auto sourceProgram = p.lock();
	auto destProgram = programs[index];
	destProgram.swap(sourceProgram);
	deleteProgram(sourceProgram);
}

void Sampler::deleteAllPrograms(bool init) {
	for (auto& p : programs)
		p.reset();
	if (!init) return;
	addProgram().lock()->setName("NewPgm-A");
	checkProgramReferences();
}

void Sampler::checkProgramReferences() {
	auto lSequencer = mpc->getSequencer().lock();
	auto t = lSequencer->getActiveSequence().lock()->getTrack(lSequencer->getActiveTrackIndex()).lock();
	auto bus = t->getBusNumber();
	for (int i = 0; i < 4; i++) {
		auto pgm = getDrumBusProgramNumber(bus);
		if (!programs[pgm]) {
			// check what real MPC does in this condition
			for (int i = 0; i < 24; i++) {
				if (programs[i]) {
					setDrumBusProgramNumber(bus, i);
				}
			}
		}
	}
}

vector<float>* Sampler::getClickSample()
{
	return &clickSample;
}

weak_ptr<Sound> Sampler::getSound(int sampleNumber)
{
	if (sampleNumber == -1)
		return weak_ptr<Sound>();

	if (sampleNumber >= sounds.size())
		return weak_ptr<Sound>();

	return sounds[sampleNumber];
}

weak_ptr<Sound> Sampler::getPreviewSound()
{
	if (sounds.size() == 0) return weak_ptr<Sound>();
	return sounds[sounds.size() - 1];
}

void Sampler::setLoopEnabled(int sampleIndex, bool enabled)
{
	sounds[sampleIndex]->setLoopEnabled(enabled);

	if (!enabled) return;

	for (auto& p : programs) {
		if (!p) continue;
		for (int i = 0; i < 64; i++) {
			if (p->getNoteParameters(i + 35)->getSndNumber() == sampleIndex) {
				p->getNoteParameters(i + 35)->setVoiceOverlap(2);
			}
		}
	}
}

void Sampler::trimSample(int sampleNumber, int start, int end)
{
	auto s = sounds[sampleNumber];
	auto data = s->getSampleData();
	auto lf = s->getLastFrameIndex();

	if (!s->isMono()) {
		int startRight = start + lf;
		int endRight = end + lf;
		data->erase(data->begin() + endRight, data->end());
		data->erase(data->begin() + lf, data->begin() + startRight);
	}
	data->erase(data->begin() + end, data->end());
	data->erase(data->begin(), data->begin() + start);
}

void Sampler::deleteSection(const unsigned int sampleNumber, const unsigned int start, const unsigned int end) {
	auto s = sounds[sampleNumber];
	auto data = s->getSampleData();
	auto lf = s->getLastFrameIndex();

	if (!s->isMono()) {
		const unsigned int startRight = start + lf;
		const unsigned int endRight = end + lf;
		data->erase(data->begin() + startRight, data->begin() + endRight);
	}
	data->erase(data->begin() + start, data->begin() + end);
}

void Sampler::sort()
{
	soundSortingType++;
	if (soundSortingType > 2) soundSortingType = 0;
	switch (soundSortingType) {
	case 0:
		stable_sort(sounds.begin(), sounds.end(), memIndexCmp);
		break;
	case 1:
		stable_sort(sounds.begin(), sounds.end(), nameCmp);
		break;
	case 2:
		stable_sort(sounds.begin(), sounds.end(), sizeCmp);
		break;
	}
}

int Sampler::getSampleIndexName(int index)
{
	auto newIndex = 0;
	for (int i = 0; i < sounds.size(); i++) {
		if (sortSamplesByName()[i].lock()->getMemoryIndex() == index) {
			newIndex = i;
			break;
		}
	}
	return newIndex;
}

int Sampler::getSampleIndexSize(int index)
{
	auto newIndex = -1;
	for (int i = 0; i < sounds.size(); i++) {
		if (sortSamplesBySize()[i].lock()->getMemoryIndex() == index) {
			newIndex = i;
			break;
		}
	}
	return newIndex;
}

vector<weak_ptr<Sound>> Sampler::sortSamplesByMemoryIndex(vector<weak_ptr<Sound>> list)
{
	//Collections::sort(list, new Sampler_sortSamplesByMemoryIndex_2(this));
	return list;
}

vector<weak_ptr<Sound>> Sampler::sortSamplesByName()
{
	vector<weak_ptr<Sound>> res;
	//::java::util::List* tempSamples = new ::java::util::ArrayList();
	//npc(tempSamples)->addAll(static_cast< ::java::util::Collection* >(sounds));
	//::java::util::Collections::sort(tempSamples, new Sampler_sortSamplesByName_3(this));
	return res;
}

vector<weak_ptr<Sound>> Sampler::sortSamplesBySize()
{
	vector<weak_ptr<Sound>> res;
	//	::java::util::List* tempSamples = new ::java::util::ArrayList();
	//	npc(tempSamples)->addAll(static_cast< ::java::util::Collection* >(sounds));
	//	::java::util::Collections::sort(tempSamples, new Sampler_sortSamplesBySize_4(this));
	return res;
}

void Sampler::deleteSample(int sampleIndex)
{
	sounds.erase(sounds.begin() + sampleIndex);
	for (int i = sampleIndex; i < sounds.size(); i++) {
		sounds[i]->setMemoryIndex(sounds[i]->getMemoryIndex() - 1);
	}

	for (auto& p : programs) {
		if (!p) continue;
		for (auto& n : p->getNotesParameters()) {
			if (n->getSndNumber() == sampleIndex) {
				n->setSoundNumber(-1);
			}
			else if (n->getSndNumber() > sampleIndex) {
				n->setSoundNumber(n->getSndNumber() - 1);
			}
		}
	}
}

int Sampler::getSoundSortingType()
{
	return soundSortingType;
}

void Sampler::deleteAllSamples()
{
	sounds.clear();
	for (auto& p : programs) {
		if (!p) continue;
		for (auto& n : p->getNotesParameters()) {
			n->setSoundNumber(-1);
		}
	}
}

void Sampler::process12Bit(vector<float>* fa)
{
	for (auto j = 0; j < fa->size(); j++) {

		if (fa->at(j) != 0.0f) {
			auto fShort = static_cast< int16_t >(fa->at(j) * 32768.0);

			if (fa->at(j) > 0.9999999f)
				fShort = 32767;
			int16_t newShort = fShort &= ~(1 << 12);
			newShort &= ~(1 << 13);
			newShort &= ~(1 << 14);
			newShort &= ~(1 << 15);
			fa->at(j) = static_cast< float >(newShort / 32768.0);
		}
		else {
			fa->at(j) = 0;
		}
	}
}

vector<float>* Sampler::process8Bit(vector<float>* fa)
{
	auto newSampleData = vector<float>(fa->size());
	for (auto j = 0; j < newSampleData.size(); j++) {
		if ((*fa)[j] != 0.0f) {
			auto fByte = static_cast< int8_t >(((*fa)[j] * 128.0));
			newSampleData[j] = static_cast< float >((fByte / 128.0));
		}
		else {
			newSampleData[j] = 0;
		}
	}
	return &newSampleData;
}

Sound* Sampler::createZone(Sound* source, int start, int end, int endMargin)
{
	auto overlap = (int)(endMargin * (source->getSampleRate() / 1000.0));
	if (!source->isMono()) {
		start *= 2;
		end *= 2;
		overlap *= 2;
	}
	if (overlap > end - start)
		overlap = end - start;

	auto zone = new Sound(source->getSampleRate(), sounds.size());
	auto zoneLength = end - start + overlap;
	auto zoneData = new vector<float>(zoneLength);
	for (int i = 0; i < zoneLength; i++)
		(*zoneData)[i] = (*source->getSampleData())[i + start];

	//zone->setSampleData(*zoneData);
	zone->setMono(source->isMono());
	return zone;
}

void Sampler::stopAllVoices()
{
	if (mpc->getAudioMidiServices().lock()->isDisabled()) return;
	mpc->getBasicPlayer()->allSoundOff();
	for (auto m : mpc->getDrums())
		m->allSoundOff();
}

void Sampler::stopAllVoices(int frameOffset) {
	dynamic_cast<ctootextensions::MpcSoundPlayerChannel*>(mpc->getDrums()[0])->allSoundOff(frameOffset);
}

// Like stopAllVoices, but without short fade-out/decay, so associated sample data can be removed from memory directly.
void Sampler::finishVoices() {
	mpc->getBasicPlayer()->finishVoice();
}

void Sampler::playX(int playXMode, vector<int>* zone)
{
	/*
	int index = gui.lock()->getSoundGui()->getSoundIndex();
	auto sound = sounds[index];
	auto start = 0;
	auto end = sound->getSampleData()->size() - 1;

	if (!sound->isMono()) end /= 2;

	auto fullEnd = end;
	if (playXMode == 1) {
		start = (*zone)[0];
		end = (*zone)[1];
	}
	if (playXMode == 2)
		end = sound->getStart();

	if (playXMode == 3)
		end = sound->getLoopTo();

	if (playXMode == 4) {
		start = sound->getEnd();
		end = fullEnd;
	}
	int oldStart = sound->getStart();
	int oldEnd = sound->getEnd();
	sound->setStart(start);
	sound->setEnd(end);
	mpc->getBasicPlayer()->noteOn(-4, 127);
	sound->setStart(oldStart);
	sound->setEnd(oldEnd);
	*/
}

weak_ptr<Sound> Sampler::getPlayXSound() {
	//return sounds[gui.lock()->getSoundGui()->getSoundIndex()];
	return weak_ptr<Sound>();
}

int Sampler::getFreeSampleSpace()
{
	int freeSpace = 32 * 1024 * 1000;
	for (auto& s : sounds) {
		freeSpace -= s->getSampleData()->size() * 2;
	}
	return freeSpace;
}

int Sampler::getLastInt(string s)
{
	auto offset = s.length();
	for (int i = s.length() - 1; i >= 0; i--) {
		auto c = s.at(i);
		if (isdigit(c)) {
			offset--;
		}
		else {

			if (offset == s.length())
				return INT_MIN;
			break;
		}
	}
	return stoi(s.substr(offset));
}

string Sampler::addOrIncreaseNumber(string s)
{
	auto res = addOrIncreaseNumber2(s);
	bool exists = true;
	while (exists) {
		exists = false;
		for (int i = 0; i < getSoundCount(); i++) {
			if (getSoundName(i).compare(res) == 0) {
				exists = true;
				res = addOrIncreaseNumber2(res);
				break;
			}
		}
	}
	return res;
}

string Sampler::addOrIncreaseNumber2(string s) {
	int candidate = getLastInt(s);
	string res = s;
	if (candidate != INT_MIN) {
		auto candidateStr = to_string(candidate);
		int candidateLength = candidateStr.length();

		res = res.substr(0, res.length() - candidateLength);

		candidate++;

		candidateStr = to_string(candidate);
		candidateLength = candidateStr.length();

		if (res.length() + candidateLength > 16)
			res = res.substr(0, 16 - candidateLength);

		res += candidateStr;
	}
	else {
		res = s + to_string(1);
	}
	return res;
}

Pad* Sampler::getLastPad(Program* program)
{
	/*
	auto lGui = gui.lock();
	auto lastValidPad = lGui->getSamplerGui()->getPad();
	if (lastValidPad == -1) lastValidPad = lGui->getSamplerGui()->getPrevPad();
	return program->getPad(lastValidPad);
	*/
	return program->getPad(0);
}

NoteParameters* Sampler::getLastNp(Program* program)
{
	/*
	auto lGui = gui.lock();
	auto lastValidNote = lGui->getSamplerGui()->getNote();
	if (lastValidNote == 34) lastValidNote = lGui->getSamplerGui()->getPrevNote();
	return program->getNoteParameters(lastValidNote);
	*/
	return program->getNoteParameters(0);
}

int Sampler::getUnusedSampleAmount()
{
	vector<weak_ptr<Sound>> soundSet;
	for (auto& p : programs) {
		for (auto& nn : p->getNotesParameters()) {
			if (nn->getSndNumber() != -1) {
				auto candidate = sounds[nn->getSndNumber()];
				bool contains = false;
				for (auto& s : soundSet) {
					if (s.lock() == candidate) {
						contains = true;
						break;
					}
				}
				if (!contains) soundSet.push_back(sounds[nn->getSndNumber()]);
			}
		}
	}
	return (int)(sounds.size() - soundSet.size());
}

void Sampler::purge()
{
	vector<weak_ptr<Sound>> soundSet;
	for (auto& p : programs) {
		for (auto& nn : p->getNotesParameters()) {
			if (nn->getSndNumber() != -1) {
				auto candidate = sounds[nn->getSndNumber()];
				bool contains = false;
				for (auto& s : soundSet) {
					if (s.lock() == candidate) {
						contains = true;
						break;
					}
				}
				if (!contains) soundSet.push_back(sounds[nn->getSndNumber()]);
			}
		}
	}

	vector<weak_ptr<Sound>> toRemove;
	for (auto& s : sounds) {
		bool contains = false;
		for (auto& s2 : soundSet) {
			if (s2.lock() == s) {
				contains = true;
				break;
			}
		}
		if (!contains) toRemove.push_back(s);
	}
	for (auto& s : toRemove)
		deleteSound(s);
}

void Sampler::deleteSound(weak_ptr<Sound> sound) {
	auto lSound = sound.lock();
	for (int i = 0; i < sounds.size(); i++) {
		if (sounds[i] == lSound) {
			sounds.erase(sounds.begin() + i);
			break;
		}
	}
}

vector<float> Sampler::mergeToStereo(vector<float> fa0, vector<float> fa1)
{
	int newSampleLength = fa0.size() * 2;
	if (fa1.size() > fa0.size()) newSampleLength = fa1.size() * 2;

	auto newSampleData = vector<float>(newSampleLength);
	int k = 0;
	for (int i = 0; i < newSampleLength; i = i + 2) {
		if (fa0.size() > k) {
			newSampleData[i] = fa0[k];
		}
		else {
			newSampleData[i] = 0.0f;
		}
		if (fa1.size() > k) {
			newSampleData[i + 1] = fa1[k++];
		}
		else {
			newSampleData[i + 1] = 0.0f;
		}
	}
	return newSampleData;
}

void Sampler::setDrumBusProgramNumber(int busNumber, int programNumber)
{
	mpc->getDrums()[busNumber - 1]->setProgram(programNumber);
}

int Sampler::getDrumBusProgramNumber(int busNumber)
{
	return mpc->getDrums()[busNumber - 1]->getProgram();
}

mpc::ctootextensions::MpcSoundPlayerChannel* Sampler::getDrum(int i)
{
	return mpc->getDrum(i);
}

weak_ptr<Sound> Sampler::getClickSound()
{
	return clickSound;
}

void Sampler::arm()
{
	if (recording) return;

	if (armed) {
		armed = false;
		record();
		return;
	}
	auto lGui = gui.lock();
	//lGui->getMainFrame().lock()->getLayeredScreen().lock()->getCurrentBackground()->setName("waitingforinputsignal");
	armed = true;
}

void Sampler::record()
{
	/*
	auto lGui = gui.lock();
	int recSize = 0;
	recSize = lGui->getSamplerGui()->getTime() * 4410;
	lGui->getMainFrame().lock()->getLayeredScreen().lock()->getCurrentBackground()->setName("recording");
	recordBufferL = vector<float>(recSize);
	recordBufferR = vector<float>(recSize);
	recordFrame = 0;
	recording = true;
	*/
}

void Sampler::stopRecordingEarlier()
{
	auto stopFrameIndex = recordFrame;
	stopRecordingBasic();
	auto s = getPreviewSound().lock();
	int newSize = s->isMono() ? stopFrameIndex : stopFrameIndex * 2;
	auto sampleData = s->getSampleData();
	sampleData->resize(newSize);
	//gui.lock()->getMainFrame().lock()->openScreen("keeporretry");
}

void Sampler::stopRecording()
{
	stopRecordingBasic();
	//gui.lock()->getMainFrame().lock()->openScreen("keeporretry");
}

void Sampler::stopRecordingBasic()
{
	auto lGui = gui.lock();
	auto counter = 0;
	auto s = addSound().lock();
	s->setName(addOrIncreaseNumber("sound"));
	auto sampleDataL = vector<float>(preRecBufferL.size() + recordBufferL.size());
	auto sampleDataR = vector<float>(preRecBufferR.size() + recordBufferR.size());
	auto preRecCounter = 0;
	for (float f : preRecBufferL) {
		//while (preRecCounter != 4410 - (lGui->getSamplerGui()->getPreRec() * 44.1))
		//	preRecCounter++;

		sampleDataL[counter++] = f;
	}
	for (auto f : recordBufferL)
		sampleDataL[counter++] = f;

	counter = 0;
	preRecCounter = 0;
	for (float f : preRecBufferR) {
	//	while (preRecCounter != 4410 - (lGui->getSamplerGui()->getPreRec() * 44.1))
		//	preRecCounter++;

		sampleDataR[counter++] = f;
	}
	for (auto f : recordBufferR)
		sampleDataR[counter++] = f;
	/*
	auto mode = lGui->getSamplerGui()->getMode();
	auto data = s->getSampleData();
	data->clear();
	if (mode == 2) {
		data->resize(sampleDataL.size() * 2);
		for (int i = 0; i < sampleDataL.size(); i++) {
			data->at(i) = sampleDataL[i];
			data->at(i + sampleDataL.size()) = sampleDataR[i];
		}
		s->setMono(false);
	}
	else {
		data->resize(sampleDataL.size());
		auto src = mode == 0 ? &sampleDataL : &sampleDataR;
		for (int i = 0; i < src->size(); i++)
			data->at(i) = src->at(i);
		s->setMono(true);
	}
	*/
	recording = false;
}

void Sampler::setEnabled(bool enabled)
{
}

string Sampler::getName()
{
	return "sampler";
}

vector<ctoot::audio::system::AudioInput*> Sampler::getAudioInputs()
{
	return vector<ctoot::audio::system::AudioInput*>(0);
}

vector<ctoot::audio::system::AudioOutput*> Sampler::getAudioOutputs()
{
	return vector<ctoot::audio::system::AudioOutput*>{ monitorOutput.get() };
}

void Sampler::closeAudio()
{
	monitorOutput->close();
}

int Sampler::getPeakL()
{
	return (int)(peakL * 34.0);
}

int Sampler::getPeakR()
{
	return (int)(peakR * 34.0);
}

int Sampler::getLevelL()
{
	return (int)(levelL * 34.0);
}

int Sampler::getLevelR()
{
	return (int)(levelR * 34.0);
}

void Sampler::resetPeak()
{
	peakL = 0;
	peakR = 0;
	setChanged();
	notifyObservers(string("vumeter"));
}

bool Sampler::isArmed()
{
	return armed;
}

bool Sampler::isRecording()
{
	return recording;
}

void Sampler::unArm()
{
	armed = false;
	setSampleBackground();
}

void Sampler::cancelRecording()
{
	recording = false;
	setSampleBackground();
}

void Sampler::setSampleBackground()
{
	//gui.lock()->getMainFrame().lock()->getLayeredScreen().lock()->getCurrentBackground()->setName("sample");
}

int Sampler::checkExists(string soundName)
{
	for (int i = 0; i < getSoundCount(); i++) {
		if (moduru::lang::StrUtil::eqIgnoreCase(moduru::lang::StrUtil::replaceAll(soundName, ' ', ""), getSoundName(i)))
			return i;
	}
	return -1;
}

int Sampler::getNextSoundIndex(int j, bool up)
{
	auto inc = up ? 1 : -1;
	if (getSoundSortingType() == 0)
		return j + inc;

	if (getSoundSortingType() == 1) {
		auto nextIndex = getSampleIndexName(j) + inc;
		if (nextIndex > getSoundCount() - 1)
			return j;

		for (int i = 0; i < getSoundCount(); i++)
			if (getSampleIndexName(i) == nextIndex)
				return i;
	}
	else if (getSoundSortingType() == 2) {
		auto nextIndex = getSampleIndexSize(j) + inc;
		if (nextIndex > getSoundCount() - 1)
			return j;

		for (int i = 0; i < getSoundCount(); i++)
			if (getSampleIndexSize(i) == nextIndex)
				return i;
	}
	return j;
}

void Sampler::setSoundGuiPrevSound()
{
	//auto soundGui = gui.lock()->getSoundGui();
	//soundGui->setSoundIndex(getNextSoundIndex(soundGui->getSoundIndex(), false), getSoundCount());
}

void Sampler::setSoundGuiNextSound()
{
	//auto soundGui = gui.lock()->getSoundGui();
	//soundGui->setSoundIndex(getNextSoundIndex(soundGui->getSoundIndex(), true), getSoundCount());
}

Sound* Sampler::copySound(Sound* sound)
{
	auto newSound = new Sound(sound->getSampleRate(), sounds.size());
	newSound->setName(sound->getName());
	newSound->setLoopEnabled(sound->isLoopEnabled());
	auto dest = newSound->getSampleData();
	auto src = sound->getSampleData();
	dest->reserve(src->size());
	copy(src->begin(), src->end(), back_inserter(*dest));
	newSound->setMono(sound->isMono());
	newSound->setEnd(sound->getEnd());
	newSound->setStart(sound->getStart());
	newSound->setLoopTo(sound->getLoopTo());
	return newSound;
}

void Sampler::setActiveInput(ctoot::audio::server::IOAudioProcess* input)
{
	this->input = nullptr;
	monitorBufferL = nullptr;
	monitorBufferR = nullptr;
	inputSwap = input;
}

void Sampler::silenceRecordBuffer() {
	recordBuffer->makeSilence();
	levelL = 0;
	levelR = 0;
	resetPeak();
}

bool Sampler::memIndexCmp(weak_ptr<Sound> a, weak_ptr<Sound> b) {
	return a.lock()->getMemoryIndex() < b.lock()->getMemoryIndex();
}

bool Sampler::nameCmp(weak_ptr<Sound> a, weak_ptr<Sound> b) {
	return a.lock()->getName() < b.lock()->getName();
}

bool Sampler::sizeCmp(weak_ptr<Sound> a, weak_ptr<Sound> b) {
	return a.lock()->getLastFrameIndex() < b.lock()->getLastFrameIndex();
}

int Sampler::getUsedProgram(int startIndex, bool up) {
	auto res = startIndex;

	if (up) {
		for (int i = startIndex + 1; i < programs.size(); i++) {
			if (programs[i]) {
				res = i;
				break;
			}
		}
	}
	else {
		for (int i = startIndex - 1; i >= 0; i--) {
			if (programs[i]) {
				res = i;
				break;
			}
		}
	}
	return res;
}

Sampler::~Sampler() {
}
