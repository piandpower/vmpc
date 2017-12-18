#pragma once
#include <synth/SynthChannel.hpp>

//#include <thirdp/concurrentqueue.h>

namespace ctoot {

	namespace control {
		class CompoundControl;
	}

	namespace audio {

		namespace server {
			struct AudioServer;
		}

		namespace core {
			class MetaInfo;
		}

		namespace mixer {
			class AudioMixer;
			class MainMixControls;
		}
	}
}

namespace mpc {

	namespace sampler {
		class Sampler;
		class MixerChannel;
		class Program;
		class NoteParameters;
		class Sound;
	}

	namespace ctootextensions {

		class Voice;
		class MpcSoundPlayerControls;
		class MpcFaderControl;
		class MpcMixerInterconnection;
		class MpcSoundPlayerChannel
			: public ctoot::synth::SynthChannel
		{

		private:
			//static moodycamel::ConcurrentQueue<std::weak_ptr<mpc::ctootextensions::Voice>> unusedVoices;
			//static moodycamel::ConcurrentQueue<std::weak_ptr<mpc::ctootextensions::Voice>> voices;

			static std::vector<std::weak_ptr<Voice> > unusedVoices;
			static std::vector<std::weak_ptr<Voice> > voices;

			std::weak_ptr<MpcSoundPlayerControls> controls{};
			std::weak_ptr<mpc::sampler::Sampler> sampler{};
			std::weak_ptr<ctoot::audio::mixer::AudioMixer> mixer{};
			int programNumber{ 0 };
			bool receivePgmChange{ false };
			bool receiveMidiVolume{ false };
			std::shared_ptr<ctoot::audio::core::MetaInfo> info{};
			std::vector<std::shared_ptr<mpc::sampler::MixerChannel>> mixerChannels{};
			int index{ 0 };
			std::vector<MpcMixerInterconnection*> mixerConnections{};
			std::weak_ptr<mpc::sampler::Program> program{};
			int padNumber{ 0 };
			mpc::sampler::NoteParameters* np{ nullptr };
			int soundNumber{ 0 };
			//std::weak_ptr<mpc::ctootextensions::Voice> voice{};
			std::weak_ptr<mpc::sampler::Sound> vars{};
			std::weak_ptr<mpc::sampler::MixerChannel> pgmMixerChannel{};
			MpcFaderControl* faderControl{ nullptr };
			int auxBus{ 0 };
			ctoot::control::CompoundControl* compoundControl{ nullptr };
			ctoot::audio::server::AudioServer* server{ nullptr };

		public:
			void setProgram(int i);
			int getProgram() override;
			bool receivesPgmChange();
			void setReceivePgmChange(bool b);
			bool receivesMidiVolume();
			void setReceiveMidiVolume(bool b);
			void setLocation(std::string location) override;
			void noteOn(int note, int velo) override;
			virtual void mpcNoteOn(int track, int note, int velo, int varType, int varValue, int frameOffset);

		private:
			void checkForMutes(mpc::sampler::NoteParameters* np);
			void stopPad(int p, int o);

		public:
			void noteOff(int note) override;
			void allNotesOff() override;
			void allSoundOff() override;
			void allSoundOff(int frameOffset);
			void connectVoices();
			void kill(std::weak_ptr<mpc::ctootextensions::Voice> mpcVoice);
			std::weak_ptr<ctoot::audio::core::MetaInfo> getInfo();
			std::vector<std::weak_ptr<mpc::sampler::MixerChannel>> getMixerChannels();
			int getDrumNumber();

		public:
			void mpcNoteOff(int note, int frameOffset);

		private:
			void stopPad(int p, int o, int offset);

		public:
			MpcSoundPlayerChannel(std::weak_ptr<MpcSoundPlayerControls> controls);
			~MpcSoundPlayerChannel();

		};
	}
}
