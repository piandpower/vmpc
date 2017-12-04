#include <controls/sampler/dialog/ResampleControls.hpp>

//#include <disk/MpcFile.hpp>
//#include <disk/SoundLoader.hpp>
#include <ui/NameGui.hpp>
#include <lcdgui/Field.hpp>
#include <ui/sampler/SoundGui.hpp>
#include <sampler/Sampler.hpp>
#include <sampler/Sound.hpp>

//#include <libsamplerate/src/samplerate.h>

using namespace mpc::controls::sampler::dialog;
using namespace std;

ResampleControls::ResampleControls(mpc::Mpc* mpc) 
	: AbstractSamplerControls(mpc)
{
}

void ResampleControls::turnWheel(int i)
{
	init();
	auto lLs = ls.lock(); 
	if (param.compare("newfs") == 0) {
		soundGui->setNewFs(soundGui->getNewFs() + i);
	}
	else if (param.compare("newbit") == 0) {
		soundGui->setNewBit(soundGui->getNewBit() + i);
	}
	else if (param.compare("quality") == 0) {
		soundGui->setQuality(soundGui->getQuality() + i);
	}
	else if (param.compare("newname") == 0) {
		nameGui->setName(lLs->lookupField("newname").lock()->getText());
		nameGui->setParameterName("newname");
		lLs->openScreen("name");
	}
}

void ResampleControls::function(int i)
{
	init();
	//mpc::disk::SoundLoader* sl;
	auto lSampler = sampler.lock();
	auto lLs = ls.lock();
	vector<float> newSampleData;
	mpc::sampler::Sound* sound;
	switch (i) {
	case 3:
		lLs->openScreen("sound");
		break;
	case 4:
		auto snd = lSampler->getSound(soundGui->getSoundIndex()).lock();
		auto destSnd = lSampler->addSound().lock();
		destSnd->setName(soundGui->getNewName());

		vector<float>* source = snd->getSampleData();

		float* srcArray = &(*source)[0];
		/*
		SRC_DATA srcData;
		srcData.data_in = srcArray;
		srcData.input_frames = source->size();
		srcData.src_ratio = (double)(soundGui->getNewFs()) / (double)(snd->getSampleRate());
		srcData.output_frames = (floor)(source->size() * srcData.src_ratio);

		vector<float>* dest = destSnd->getSampleData();
		dest->resize(srcData.output_frames);

		float* destArray = &(*dest)[0];
		srcData.data_out = destArray;

		auto error = src_simple(&srcData, 0, 1);
		if (error != 0) {
			const char* errormsg = src_strerror(error);
			string errorStr(errormsg);
		}
		destSnd->setSampleRate(soundGui->getNewFs());
		destSnd->setMono(snd->isMono());

		if (soundGui->getNewBit() == 1) {
			lSampler->process12Bit(destSnd->getSampleData());
		}
		*/
		//if (soundGui->getNewBit() == 2)
		//	newSampleData = *lSampler->process8Bit(&newSampleData);

		//sound = mpc::sampler::Sound(soundGui->getNewFs(), lSampler->getSoundCount());
		//sound->setSampleData(newSampleData);
		//sound->setName(*soundGui->getNewName());
		//sound->setMono(lSampler->getSound(soundGui->getSoundIndex())->isMono());
		//lSampler->getSounds()->push_back(sound);
		//soundGui->setSoundIndex(lSampler->getSoundCount() - 1);
		lLs->openScreen("sound");
		break;
	}
}
