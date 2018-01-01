#include <file/aps/ApsLoader.hpp>

#include <Mpc.hpp>
#include <disk/AbstractDisk.hpp>
#include <disk/StdDisk.hpp>
#include <disk/MpcFile.hpp>
#include <disk/SoundLoader.hpp>
#include <file/aps/ApsAssignTable.hpp>
#include <file/aps/ApsDrumConfiguration.hpp>
#include <file/aps/ApsGlobalParameters.hpp>
#include <file/aps/ApsMixer.hpp>
#include <file/aps/ApsNoteParameters.hpp>
#include <file/aps/ApsParser.hpp>
#include <file/aps/ApsProgram.hpp>
#include <file/aps/ApsSlider.hpp>

#include <ui/Uis.hpp>
#include <ui/disk/DiskGui.hpp>
#include <ui/sampler/MixerSetupGui.hpp>
#include <ui/sampler/SamplerGui.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <sampler/StereoMixerChannel.hpp>
#include <sampler/IndivFxMixerChannel.hpp>
#include <sampler/NoteParameters.hpp>
#include <sampler/Pad.hpp>
#include <sampler/Program.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/PgmSlider.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>

#include <lang/StrUtil.hpp>

using namespace mpc::file::aps;
using namespace moduru::lang;
using namespace std;

ApsLoader::ApsLoader(mpc::Mpc* mpc, mpc::disk::MpcFile* file) 
{
	this->mpc = mpc;
	this->file = file;
	if (loadThread.joinable()) loadThread.join();
	loadThread = thread(&ApsLoader::static_load, this);
}

void ApsLoader::static_load(void* this_p) {
	static_cast<ApsLoader*>(this_p)->load();
}

void ApsLoader::load()
{
	auto lDisk = mpc->getDisk().lock();
	lDisk->setBusy(true);
	auto apsParser = new ApsParser(file);
	auto sampler = mpc->getSampler().lock();
	sampler->deleteAllSamples();
	const bool initPgms = false;
	for (int i = 0; i < apsParser->getSoundNames().size(); i++) {
		auto ext = "snd";
		mpc::disk::MpcFile* soundFile = nullptr;
		string soundFileName = StrUtil::replaceAll(apsParser->getSoundNames()[i], ' ', "");
		for (auto& f : lDisk->getAllFiles()) {
			if (StrUtil::eqIgnoreCase(StrUtil::replaceAll(f->getName(), ' ', ""), soundFileName + ".SND"))
				soundFile = f;
		}
		if (soundFile == nullptr) {
			for (auto& f : lDisk->getAllFiles()) {
				if (StrUtil::eqIgnoreCase(StrUtil::replaceAll(f->getName(), ' ', ""), soundFileName + ".WAV"))
					soundFile = f;
			}
			ext = "wav";
		}
		if (soundFile != nullptr) {
			loadSound(soundFileName, ext, soundFile, mpc, false, i);
		}
		else {
			if (dynamic_pointer_cast<mpc::disk::StdDisk>(lDisk)) {
				ext = "snd";
				soundFileName = soundFileName.substr(0, 8);
				for (auto& f : lDisk->getAllFiles()) {
					if (StrUtil::eqIgnoreCase(StrUtil::replaceAll(f->getName(), ' ', ""), soundFileName + ".SND"))
						soundFile = f;
				}
				if (soundFile == nullptr) {
					for (auto& f : lDisk->getAllFiles()) {
						if (StrUtil::eqIgnoreCase(StrUtil::replaceAll(f->getName(), ' ', ""), soundFileName + ".WAV"))
							soundFile = f;
					}
					ext = "wav";
				}
			}
			if (soundFile == nullptr) {
				throw std::invalid_argument("error loading soundfile");
			}
		}
	}
	sampler->deleteAllPrograms(initPgms);
	for (auto& p : apsParser->getPrograms()) {
		auto newProgram = sampler->addProgram(p->index).lock();
		auto assignTable = p->getAssignTable()->get();
		newProgram->setName(p->getName());
		for (int i = 0; i < 64; i++) {
			int padnn = assignTable[i];
			newProgram->getPad(i)->setNote(padnn);
			auto apssmc = p->getStereoMixerChannel(i + 35);
			auto apsifmc = p->getIndivFxMixerChannel(i + 35);
			auto smc = newProgram->getPad(i)->getStereoMixerChannel().lock();
			auto ifmc = newProgram->getPad(i)->getIndivFxMixerChannel().lock();
			ifmc->setFxPath(apsifmc->getFxPath());
			smc->setLevel(apssmc->getLevel());
			smc->setPanning(apssmc->getPanning());
			ifmc->setVolumeIndividualOut(apsifmc->getVolumeIndividualOut());
			ifmc->setFxSendLevel(apsifmc->getFxSendLevel());
			ifmc->setOutput(apsifmc->getOutput());
			auto np = newProgram->getNoteParameters(i + 35);
			auto anp = p->getNoteParameters(i + 35);
			np->setSoundNumber(anp->getSoundNumber());
			np->setTune(anp->getTune());
			np->setVoiceOverlap(anp->getVoiceOverlap());
			np->setDecayMode(anp->getDecayMode());
			np->setAttack(anp->getAttack());
			np->setDecay(anp->getDecay());
			np->setFilterAttack(anp->getVelocityToFilterAttack());
			np->setFilterDecay(anp->getVelocityToFilterDecay());
			np->setFilterEnvelopeAmount(anp->getVelocityToFilterAmount());
			np->setFilterFrequency(anp->getCutoffFrequency());
			np->setFilterResonance(anp->getResonance());
			np->setMuteAssignA(anp->getMute1());
			np->setMuteAssignB(anp->getMute2());
			np->setOptNoteA(anp->getAlsoPlay1());
			np->setOptionalNoteB(anp->getAlsoPlay2());
			np->setSliderParameterNumber(anp->getSliderParameter());
			np->setSoundGenMode(anp->getSoundGenerationMode());
			np->setVelocityToStart(anp->getVelocityToStart());
			np->setVelocityToAttack(anp->getVelocityToAttack());
			np->setVelocityToFilterFrequency(anp->getVelocityToFilterFrequency());
			np->setVeloToLevel(anp->getVelocityToLevel());
			np->setVeloRangeLower(anp->getVelocityRangeLower());
			np->setVeloRangeUpper(anp->getVelocityRangeUpper());
			np->setVelocityToPitch(anp->getVelocityToPitch());
		}
		auto slider = dynamic_cast<mpc::sampler::PgmSlider*>(newProgram->getSlider());
		slider->setAttackHighRange(p->getSlider()->getAttackHigh());
		slider->setAttackLowRange(p->getSlider()->getAttackLow());
		slider->setControlChange(p->getSlider()->getProgramChange());
		slider->setDecayHighRange(p->getSlider()->getDecayHigh());
		slider->setDecayLowRange(p->getSlider()->getDecayLow());
		slider->setFilterHighRange(p->getSlider()->getFilterHigh());
		slider->setFilterLowRange(p->getSlider()->getFilterLow());
		slider->setAssignNote(p->getSlider()->getNote());
		slider->setTuneHighRange(p->getSlider()->getTuneHigh());
		slider->setTuneLowRange(p->getSlider()->getTuneLow());
	}

	for (int i = 0; i < 4; i++) {
		auto m = apsParser->getDrumMixers()[i];
		auto drum = mpc->getDrum(i);
		for (int note = 35; note <= 98; note++) {
			auto apssmc = m->getStereoMixerChannel(note);
			auto apsifmc = m->getIndivFxMixerChannel(note);
			auto drumsmc = drum->getStereoMixerChannels()[note - 35].lock();
			auto drumifmc = drum->getIndivFxMixerChannels()[note - 35].lock();
			drumifmc->setFxPath(apsifmc->getFxPath());
			drumsmc->setLevel(apssmc->getLevel());
			drumsmc->setPanning(apssmc->getPanning());
			drumifmc->setVolumeIndividualOut(apsifmc->getVolumeIndividualOut());
			drumifmc->setOutput(apsifmc->getOutput());
			drumifmc->setFxSendLevel(apsifmc->getFxSendLevel());
		}
		auto pgm = apsParser->getDrumConfiguration(i)->getProgram();
		drum->setProgram(pgm);
		drum->setReceivePgmChange(apsParser->getDrumConfiguration(i)->getReceivePgmChange());
		drum->setReceiveMidiVolume(apsParser->getDrumConfiguration(i)->getReceiveMidiVolume());
	}
	auto uis = mpc->getUis().lock();
	uis->getMixerSetupGui()->setRecordMixChangesEnabled(apsParser->getGlobalParameters()->isRecordMixChangesEnabled());
	uis->getMixerSetupGui()->setCopyPgmMixToDrumEnabled(apsParser->getGlobalParameters()->isCopyPgmMixToDrumEnabled());
	uis->getMixerSetupGui()->setFxDrum(apsParser->getGlobalParameters()->getFxDrum());
	uis->getMixerSetupGui()->setIndivFxSourceDrum(apsParser->getGlobalParameters()->isIndivFxSourceDrum());
	uis->getMixerSetupGui()->setStereoMixSourceDrum(apsParser->getGlobalParameters()->isStereoMixSourceDrum());
	uis->getSamplerGui()->setPadToIntSound(apsParser->getGlobalParameters()->isPadToIntSoundEnabled());
	uis->getDiskGui()->removePopup();
	uis->getSoundGui()->setSoundIndex(0, sampler->getSoundCount());
	mpc->getLayeredScreen().lock()->openScreen("load");
	lDisk->setBusy(false);
}

void ApsLoader::loadSound(string soundFileName, string ext, mpc::disk::MpcFile* soundFile, mpc::Mpc* mpc, bool replace, int loadSoundIndex)
{
	auto sl = mpc::disk::SoundLoader(mpc, mpc->getSampler().lock()->getSounds(), replace);
	sl.setPartOfProgram(true);
	sl.loadSound(soundFile);
	showPopup(soundFileName, ext, soundFile->length());
}

void ApsLoader::showPopup(string name, string ext, int sampleSize)
{
	mpc->getUis().lock()->getDiskGui()->openPopup(StrUtil::padRight(name, " ", 16), ext);
	if (dynamic_pointer_cast<mpc::disk::StdDisk>(mpc->getDisk().lock())) {
		auto sleepTime = sampleSize / 400;
		if (sleepTime < 300)
			sleepTime = 300;
		//this_thread::sleep_for(chrono::milliseconds((int)(sleepTime*mpc::maingui::Constants::TFACTOR)));
		this_thread::sleep_for(chrono::milliseconds((int)(sleepTime* 0.1)));
	}
}

ApsLoader::~ApsLoader() {
	if (loadThread.joinable()) loadThread.join();
}
