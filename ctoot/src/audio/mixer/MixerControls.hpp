#pragma once
#include <control/CompoundControl.hpp>
#include <audio/mixer/BusControls.hpp>
#include <audio/core/AudioControlsChain.hpp>
#include <audio/fader/FaderControl.hpp>

#include <memory>
#include <vector>
#include <string>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class MixerControls
				: public ctoot::control::CompoundControl
				, public std::enable_shared_from_this<MixerControls>
			{

			public:
				typedef control::CompoundControl super;

			private:
				bool canAddBusses{ true };
				std::shared_ptr<BusControls> mainBusControls{ nullptr };
				std::vector<std::shared_ptr<BusControls>> fxBusControls{};
				std::vector<std::shared_ptr<BusControls>> auxBusControls{};
				float smoothingFactor{ 0.05f };

			public:
				int getProviderId() override;
				virtual float getSmoothingFactor();
				virtual std::shared_ptr<BusControls> createFxBusControls(std::string name, ctoot::audio::core::ChannelFormat* format);
				virtual std::shared_ptr<BusControls> createAuxBusControls(std::string name, ctoot::audio::core::ChannelFormat* format);
				virtual std::weak_ptr<BusControls> getBusControls(std::string name);
				virtual std::weak_ptr<BusControls> getMainBusControls();
				virtual std::vector<std::shared_ptr<BusControls>> getFxBusControls();
				virtual std::vector<std::shared_ptr<BusControls>> getAuxBusControls();
				virtual std::weak_ptr<ctoot::audio::core::AudioControlsChain> createStripControls(int id, int index, std::string name);
				virtual std::weak_ptr<ctoot::audio::core::AudioControlsChain> createStripControls(int id, int index, std::string name, ctoot::audio::core::ChannelFormat* constraintFormat);
				virtual std::weak_ptr<ctoot::audio::core::AudioControlsChain> createStripControls(int id, int index, std::string name, bool hasMixControls, ctoot::audio::core::ChannelFormat* constraintFormat);
				virtual void addStripControls(std::shared_ptr<CompoundControl> cc);
				virtual void removeStripControls(std::shared_ptr<CompoundControl> cc);
				virtual void removeStripControls(std::string name);
				virtual void moveStripControls(std::string name, std::string beforeName);
				virtual std::shared_ptr<ctoot::audio::core::AudioControlsChain> getStripControls(std::string name);
				virtual std::shared_ptr<ctoot::audio::core::AudioControlsChain> getStripControls(int id, int index);
				virtual ctoot::audio::fader::FaderControl* createFaderControl(bool muted);

			private:
				static std::vector<std::string> opNames_; // { "Add", "Remove" };

			public:
				
				MixerControls(std::string name, std::string mainBusName, ctoot::audio::core::ChannelFormat* channelFormat);
				MixerControls(std::string name) : MixerControls(name, "Main", ctoot::audio::core::ChannelFormat::STEREO()) {};
				MixerControls(std::string name, float smoothingFactor) : MixerControls(name) { this->smoothingFactor = smoothingFactor; };
				virtual ~MixerControls();

			private:
				static std::vector<std::string> opNames();
				friend class Mutation;

			private:
				friend class AudioMixer;

			};

		}
	}
}
