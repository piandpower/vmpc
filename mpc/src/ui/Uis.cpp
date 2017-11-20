#include <ui/Uis.hpp>

//#include <ui/NameGui.hpp>

//#include <ui/other/OthersGui.hpp>

//#include <ui/misc/PunchGui.hpp>
//#include <ui/misc/SecondSeqGui.hpp>
//#include <ui/misc/TransGui.hpp>

//#include <ui/disk/DiskGui.hpp>
//#include <ui/disk/window/DirectoryGui.hpp>
//#include <ui/disk/window/DiskWindowGui.hpp>

//#include <ui/midisync/MidiSyncGui.hpp>

#include <ui/sequencer/SequencerGui.hpp>
#include <ui/sequencer/SongGui.hpp>
#include <ui/sequencer/TrMoveGui.hpp>
#include <ui/sequencer/EditSequenceGui.hpp>
#include <ui/sequencer/BarCopyGui.hpp>
#include <ui/sequencer/StepEditorGui.hpp>

#include <ui/sequencer/window/EraseGui.hpp>
#include <ui/sequencer/window/SequencerWindowGui.hpp>

//#include <ui/sampler/window/ZoomGui.hpp>
//#include <ui/sampler/MixerSetupGui.hpp>
//#include <ui/sampler/MixerGui.hpp>
//#include <ui/sampler/window/SamplerWindowGui.hpp>
//#include <ui/sampler/window/EditSoundGui.hpp>
#include <ui/sampler/SamplerGui.hpp>
//#include <ui/sampler/SoundGui.hpp>

//#include <ui/vmpc/AudioGui.hpp>
//#include <ui/vmpc/DeviceGui.hpp>
//#include <ui/vmpc/DirectToDiskRecorderGui.hpp>
//#include <ui/vmpc/MidiGui.hpp>

using namespace mpc::ui;

Uis::Uis() 
{
	sequencerGui = new ui::sequencer::SequencerGui();
	editSequenceGui = new mpc::ui::sequencer::EditSequenceGui();
	songGui = new mpc::ui::sequencer::SongGui();
	barCopyGui = new mpc::ui::sequencer::BarCopyGui();
	trMoveGui = new mpc::ui::sequencer::TrMoveGui();
	eraseGui = new mpc::ui::sequencer::window::EraseGui();
	sequencerWindowGui = new mpc::ui::sequencer::window::SequencerWindowGui();

	samplerGui = new mpc::ui::sampler::SamplerGui();
	/*
	soundGui = new mpc::ui::sampler::SoundGui();
	editSoundGui = new mpc::ui::sampler::window::EditSoundGui();
	zoomGui = new mpc::ui::sampler::window::ZoomGui();
	samplerWindowGui = new mpc::ui::sampler::window::SamplerWindowGui();
	mixerGui = new mpc::ui::sampler::MixerGui();
	mixerSetupGui = new mpc::ui::sampler::MixerSetupGui();

	diskWindowGui = new mpc::ui::disk::window::DiskWindowGui();

	midiSyncGui = new mpc::ui::midisync::MidiSyncGui();
	nameGui = new mpc::ui::NameGui();

	punchGui = new mpc::ui::misc::PunchGui();
	transGui = new mpc::ui::misc::TransGui();

	midiGui = new mpc::ui::vmpc::MidiGui();
	secondSeqGui = new mpc::ui::misc::SecondSeqGui();
	deviceGui = new mpc::ui::vmpc::DeviceGui();
	*/
}

mpc::ui::sequencer::SequencerGui* Uis::getSequencerGui()
{
	return sequencerGui;
}

mpc::ui::sequencer::StepEditorGui* Uis::getStepEditorGui()
{
	return stepEditorGui;
}

mpc::ui::sampler::MixerGui* Uis::getMixerGui()
{
	return mixerGui;
}

mpc::ui::sequencer::EditSequenceGui* Uis::getEditSequenceGui()
{
	return editSequenceGui;
}

mpc::ui::sequencer::window::SequencerWindowGui* Uis::getSequencerWindowGui()
{
	return sequencerWindowGui;
}

mpc::ui::midisync::MidiSyncGui* Uis::getMidiSyncGui()
{
	return midiSyncGui;
}

mpc::ui::sequencer::BarCopyGui* Uis::getBarCopyGui()
{
	return barCopyGui;
}

mpc::ui::sequencer::TrMoveGui* Uis::getTrMoveGui()
{
	return trMoveGui;
}

mpc::ui::sampler::SoundGui* Uis::getSoundGui()
{
	return soundGui;
}

mpc::ui::sampler::window::ZoomGui* Uis::getZoomGui()
{
	return zoomGui;
}

mpc::ui::disk::DiskGui* Uis::getDiskGui()
{
	return diskGui;
}

mpc::ui::disk::window::DirectoryGui* Uis::getDirectoryGui()
{
	return directoryGui;
}

mpc::ui::disk::window::DiskWindowGui* Uis::getDiskWindowGui()
{
	return diskWindowGui;
}

mpc::ui::sampler::SamplerGui* Uis::getSamplerGui()
{
	return samplerGui;
}

mpc::ui::sampler::window::SamplerWindowGui* Uis::getSamplerWindowGui()
{
	return samplerWindowGui;
}

mpc::ui::NameGui* Uis::getNameGui()
{
	return nameGui;
}

mpc::ui::vmpc::DeviceGui* Uis::getDeviceGui()
{
	return deviceGui;
}

mpc::ui::sequencer::SongGui* Uis::getSongGui()
{
	return songGui;
}

mpc::ui::sampler::MixerSetupGui* Uis::getMixerSetupGui()
{
	return mixerSetupGui;
}

mpc::ui::sequencer::window::EraseGui* Uis::getEraseGui()
{
	return eraseGui;
}

mpc::ui::misc::PunchGui* Uis::getPunchGui()
{
	return punchGui;
}

mpc::ui::other::OthersGui* Uis::getOthersGui()
{
	return othersGui;
}

mpc::ui::vmpc::AudioGui* Uis::getAudioGui()
{
	return audioGui;
}

mpc::ui::vmpc::MidiGui* Uis::getMidiGui()
{
	return midiGui;
}

mpc::ui::vmpc::DirectToDiskRecorderGui* Uis::getD2DRecorderGui()
{
	return d2dRecorderGui;
}

mpc::ui::misc::TransGui* Uis::getTransGui()
{
	return transGui;
}

mpc::ui::misc::SecondSeqGui* Uis::getSecondSeqGui()
{
	return secondSeqGui;
}

mpc::ui::sampler::window::EditSoundGui* Uis::getEditSoundGui() {
	return editSoundGui;
}


Uis::~Uis() {
	if (stepEditorGui != nullptr) delete stepEditorGui;
	if (sequencerWindowGui != nullptr) delete sequencerWindowGui;
	if (sequencerGui != nullptr) delete sequencerGui;
	if (editSequenceGui != nullptr) delete editSequenceGui;
	if (barCopyGui != nullptr) delete barCopyGui;
	if (trMoveGui != nullptr) delete trMoveGui;
	if (soundGui != nullptr) delete soundGui;
	if (samplerGui != nullptr) delete samplerGui;
	if (editSoundGui != nullptr) delete editSoundGui;
	if (zoomGui != nullptr) delete zoomGui;
	if (samplerWindowGui != nullptr) delete samplerWindowGui;
	if (mixerGui != nullptr) delete mixerGui;
	if (diskGui != nullptr) delete diskGui;
	if (directoryGui != nullptr) delete directoryGui;
	if (diskWindowGui != nullptr) delete diskWindowGui;
	if (midiSyncGui != nullptr) delete midiSyncGui;
	if (nameGui != nullptr) delete nameGui;
	if (mixerSetupGui != nullptr) delete mixerSetupGui;
	if (eraseGui != nullptr) delete eraseGui;
	if (punchGui != nullptr) delete punchGui;
	if (midiGui != nullptr) delete midiGui;
	if (audioGui != nullptr) delete audioGui;
	if (d2dRecorderGui != nullptr) delete d2dRecorderGui;
	if (transGui != nullptr) delete transGui;
	if (secondSeqGui != nullptr) delete secondSeqGui;
	if (deviceGui != nullptr) delete deviceGui;
	if (songGui != nullptr) delete songGui;
	if (othersGui != nullptr) delete othersGui;
}
