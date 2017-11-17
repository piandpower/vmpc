#pragma once

namespace mpc {
	namespace ui {

		class NameGui;

		namespace vmpc {
			class DirectToDiskRecorderGui;
			class AudioGui;
			class DeviceGui;
			class MidiGui;
		}

		namespace misc {
			class PunchGui;
			class SecondSeqGui;
			class TransGui;
		}

		namespace other {
			class OthersGui;
		}

		namespace disk {
			class DiskGui;
			namespace window {
				class DiskWindowGui;
				class DirectoryGui;
			}
		}

		namespace sequencer {
			class StepEditorGui;
			class BarCopyGui;
			class EditSequenceGui;
			class SongGui;
			class TrMoveGui;
			class SequencerGui;

			namespace window {
				class SequencerWindowGui;
				class EraseGui;
			}
		}

		namespace sampler {
			class SamplerGui;
			class SoundGui;
			class MixerGui;
			class MixerSetupGui;

			namespace window {
				class SamplerWindowGui;
				class EditSoundGui;
				class ZoomGui;
			}
		}

		namespace midisync {
			class MidiSyncGui;
		}

		class Uis
		{

		private:
			mpc::ui::sequencer::SequencerGui* sequencerGui{};
			mpc::ui::sequencer::window::SequencerWindowGui* sequencerWindowGui{};
			mpc::ui::sequencer::StepEditorGui* stepEditorGui{};
			ui::sequencer::BarCopyGui* barCopyGui{};
			ui::sequencer::EditSequenceGui* editSequenceGui{};
			ui::sequencer::TrMoveGui* trMoveGui{};
			ui::sampler::SoundGui* soundGui{};
			ui::sampler::SamplerGui* samplerGui{};
			ui::sampler::window::EditSoundGui* editSoundGui{};
			ui::sampler::window::SamplerWindowGui* samplerWindowGui{};
			ui::sampler::window::ZoomGui* zoomGui{};
			ui::sampler::MixerGui* mixerGui{};
			ui::sampler::MixerSetupGui* mixerSetupGui{};
			ui::disk::DiskGui* diskGui{};
			ui::disk::window::DirectoryGui* directoryGui{};
			ui::disk::window::DiskWindowGui* diskWindowGui{};
			mpc::ui::midisync::MidiSyncGui* midiSyncGui{};
			mpc::ui::NameGui* nameGui{};
			ui::sequencer::SongGui* songGui{};
			ui::sequencer::window::EraseGui* eraseGui{};

			ui::other::OthersGui* othersGui{};
			ui::misc::PunchGui* punchGui{};
			ui::misc::SecondSeqGui* secondSeqGui{};
			ui::misc::TransGui* transGui{};

			ui::vmpc::DeviceGui* deviceGui{};
			ui::vmpc::MidiGui* midiGui{};
			ui::vmpc::AudioGui* audioGui{};
			ui::vmpc::DirectToDiskRecorderGui* d2dRecorderGui{};

		public:
			ui::sequencer::SequencerGui* getSequencerGui();
			ui::sequencer::StepEditorGui* getStepEditorGui();
			ui::sampler::MixerGui* getMixerGui();
			ui::sequencer::EditSequenceGui* getEditSequenceGui();
			ui::sequencer::window::SequencerWindowGui* getSequencerWindowGui();
			ui::midisync::MidiSyncGui* getMidiSyncGui();
			ui::sequencer::BarCopyGui* getBarCopyGui();
			ui::sequencer::TrMoveGui* getTrMoveGui();
			ui::sampler::SoundGui* getSoundGui();
			ui::sampler::window::EditSoundGui* getEditSoundGui();
			ui::sampler::window::ZoomGui* getZoomGui();
			ui::disk::DiskGui* getDiskGui();
			ui::disk::window::DirectoryGui* getDirectoryGui();
			ui::disk::window::DiskWindowGui* getDiskWindowGui();
			ui::sampler::window::SamplerWindowGui* getSamplerWindowGui();
			ui::sampler::SamplerGui* getSamplerGui();
			ui::NameGui* getNameGui();
			ui::sequencer::SongGui* getSongGui();
			ui::sampler::MixerSetupGui* getMixerSetupGui();
			ui::sequencer::window::EraseGui* getEraseGui();

			ui::other::OthersGui* getOthersGui();
			ui::vmpc::DeviceGui* getDeviceGui();
			ui::vmpc::AudioGui* getAudioGui();
			ui::vmpc::MidiGui* getMidiGui();
			ui::vmpc::DirectToDiskRecorderGui* getD2DRecorderGui();

			ui::misc::TransGui* getTransGui();
			ui::misc::SecondSeqGui* getSecondSeqGui();
			ui::misc::PunchGui* getPunchGui();

		public:
			Uis();
			~Uis();

		};
	}
}
