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
//#include <ui/sampler/SamplerGui.hpp>
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

	/*
	soundGui = new mpc::ui::sampler::SoundGui();
	samplerGui = new mpc::ui::sampler::SamplerGui();
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

Uis::~Uis() {
}
