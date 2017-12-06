#pragma once
#include <audio/server/AudioServer.hpp>
#include <audio/server/AudioClient.hpp>
#include <audio/mixer/MixControls.hpp>
#include <audio/mixer/MixerControlsObserver.hpp>

#include <thirdp/concurrentqueue.h>
#include <vector>

namespace ctoot {
	namespace audio {
		namespace mixer {

            class Mutation;
			class AudioMixerStrip;
			class AudioMixerBus;
			class AudioMixer
				: public virtual ctoot::audio::server::AudioClient
			{

			private:
				std::shared_ptr<MixerControls> controls{ nullptr };

			public:
				std::shared_ptr<AudioMixerBus> mainBus{ nullptr };
				std::vector<std::shared_ptr<AudioMixerBus>> busses{};
				std::vector<std::shared_ptr<AudioMixerBus>> auxBusses{};
				std::vector<std::shared_ptr<AudioMixerBus>> fxBusses{};

			private:
				std::vector<std::shared_ptr<AudioMixerStrip>> strips{};
				std::vector<std::shared_ptr<AudioMixerStrip>> channelStrips{};
				std::vector<std::shared_ptr<AudioMixerStrip>> groupStrips{};
				std::vector<std::shared_ptr<AudioMixerStrip>> fxStrips{};
				std::vector<std::shared_ptr<AudioMixerStrip>> auxStrips{};
				std::shared_ptr<AudioMixerStrip> mainStrip{ nullptr };
				std::weak_ptr<ctoot::audio::server::AudioServer> server{};
                ctoot::audio::core::AudioBuffer* sharedAudioBuffer{ nullptr };
				moodycamel::ConcurrentQueue<Mutation*> mutationQueue{};

				bool enabled{ true };
				std::unique_ptr<MixerControlsObserver> observer{ nullptr };

			public:
				virtual std::shared_ptr<MixerControls> getMixerControls();

			public:
				virtual ctoot::audio::core::AudioBuffer* getSharedBuffer();
				virtual ctoot::audio::core::AudioBuffer* createBuffer(std::string name);
				virtual void removeBuffer(ctoot::audio::core::AudioBuffer* buffer);

			public:
				virtual bool isMutating();
				virtual void waitForMutations();
				virtual std::weak_ptr<AudioMixerStrip> getStrip(std::string name);
				virtual std::weak_ptr<AudioMixerStrip> getStripImpl(std::string name);
				virtual std::vector<std::shared_ptr<AudioMixerStrip>> getStrips();
				virtual std::weak_ptr<AudioMixerStrip> getUnusedChannelStrip();
				void work(int nFrames) override;

			public:
				virtual void processMutations();
				virtual void processMutation(Mutation* m);
				virtual void evaluateStrips(std::vector<std::shared_ptr<AudioMixerStrip>>* evalStrips, int nFrames);
				virtual void silenceStrips(std::vector<std::shared_ptr<AudioMixerStrip>>* evalStrips);
				virtual void writeBusBuffers(int nFrames);
				virtual void createBusses(std::shared_ptr<MixerControls> mixerControls);
				virtual std::shared_ptr<AudioMixerBus> createBus(std::weak_ptr<BusControls> busControls);

			public:
				virtual std::shared_ptr<AudioMixerBus> getBus(std::string name);
				virtual std::shared_ptr<AudioMixerBus> getMainBus();
				virtual std::shared_ptr<AudioMixerStrip> getMainStrip();

			public:
				virtual void createStrips(std::shared_ptr<MixerControls> mixerControls);
				virtual std::shared_ptr<AudioMixerStrip> createStrip(std::shared_ptr<ctoot::audio::core::AudioControlsChain> controls);
				virtual void removeStrip(std::shared_ptr<ctoot::audio::core::AudioControlsChain> controls);

			public:
				virtual void close();
				virtual bool isEnabled();
				void setEnabled(bool enabled) override;
				//virtual MixerInterconnection* createInterconnection(std::string name);

				AudioMixer(std::shared_ptr<MixerControls> controls, std::shared_ptr<ctoot::audio::server::AudioServer> server);
				~AudioMixer();

			private:
				friend class AudioMixer_createStrip_1;
				friend class MixerControlsObserver;
				friend class AudioMixer_DefaultMixerInterconnection;
				friend class AudioMixer_DefaultMixerInterconnection_DefaultMixerInterconnection_1;
				friend class AudioMixer_DefaultMixerInterconnection_DefaultMixerInterconnection_2;
			};

		}
	}
}
