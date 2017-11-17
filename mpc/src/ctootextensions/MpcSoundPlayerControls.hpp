#pragma once
#include <synth/SynthChannelControls.hpp>
#include <audio/server/AudioServer.hpp>
#include <audio/mixer/AudioMixer.hpp>

#include <memory>

namespace mpc {

	namespace sampler {
		class Sampler;
	}

	namespace ctootextensions {
		
		class Voice;

		class MpcSoundPlayerControls
			: public ctoot::synth::SynthChannelControls
		{

		public:
			typedef ctoot::synth::SynthChannelControls super;

		public:
			static const int MPC_SOUND_PLAYER_CHANNEL_ID{ 8 };

		private:
			static std::string NAME_;
			std::weak_ptr<mpc::sampler::Sampler> sampler{};
			std::weak_ptr<ctoot::audio::mixer::AudioMixer> mixer{};
			int drumNumber{ 0 };
			std::vector<std::weak_ptr<mpc::ctootextensions::Voice>> voices{};
			std::weak_ptr<ctoot::audio::server::AudioServer> server{ };
			
		public:
			std::weak_ptr<mpc::sampler::Sampler> getSampler();
			int getDrumNumber();
			std::weak_ptr<ctoot::audio::mixer::AudioMixer> getMixer();
			std::vector<std::weak_ptr<mpc::ctootextensions::Voice>> getVoices();
			ctoot::audio::server::AudioServer* getServer();

			MpcSoundPlayerControls(std::weak_ptr<mpc::sampler::Sampler> sampler, int drumNumber, std::weak_ptr<ctoot::audio::mixer::AudioMixer> mixer, std::vector<std::weak_ptr<mpc::ctootextensions::Voice>> voices, std::weak_ptr<ctoot::audio::server::AudioServer> server);
			~MpcSoundPlayerControls();

		public:
			static std::string NAME();

		};

	}
}
