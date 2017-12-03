#include <disk/SoundSaver.hpp>

#include <Mpc.hpp>
#include <disk/AbstractDisk.hpp>
#include <disk/MpcFile.hpp>
#include <ui/disk/DiskGui.hpp>
#include <sampler/Sound.hpp>

#include <lang/StrUtil.hpp>

using namespace mpc::disk;
using namespace moduru::lang;
using namespace std;

SoundSaver::SoundSaver(mpc::Mpc* mpc, vector<weak_ptr<mpc::sampler::Sound> > sounds, bool wav) 
{
	this->mpc = mpc;
	disk = mpc->getDisk();
	diskGui = mpc->getUis().lock()->getDiskGui();
	disk.lock()->setBusy(true);
	this->sounds = sounds;
	this->wav = wav;
	//(new ::java::lang::Thread(static_cast< ::java::lang::Runnable* >(this)))->start();
}

void SoundSaver::run()
{
    string const ext = wav ? ".WAV" : ".SND";
	auto lDisk = disk.lock();
	for (auto s : sounds) {
		auto skip = false;
		string fileName = StrUtil::replaceAll(s.lock()->getName(), ' ', "") + ext;
		mpc->getLayeredScreen().lock()->createPopup("Writing " + StrUtil::toUpper(fileName), 85);
		if (lDisk->checkExists(fileName)) {
			if (diskGui->getSaveReplaceSameSounds()) {
				auto success = lDisk->getFile(fileName)->del();
			}
			else {
				skip = true;
			}
		}
		if (skip) continue;

		if (!wav) {
			lDisk->writeSound(s);
		}
		else {
			lDisk->writeWav(s);
		}
		try {
			//::java::lang::Thread::sleep(500);
		}
		catch (exception e) {
			e.what();
		}
	}
    mpc->getLayeredScreen().lock()->removePopup();
}