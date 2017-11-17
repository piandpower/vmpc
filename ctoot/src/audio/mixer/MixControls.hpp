#pragma once

#include <audio/core/AudioControls.hpp>
#include <audio/mixer/MixVariables.hpp>
#include <audio/fader/FaderControl.hpp>
#include <audio/mixer/BusControls.hpp>
#include <audio/mixer/SoloIndicator.hpp>
#include <audio/mixer/MixerControls.hpp>
#include <control/EnumControl.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class LCRControl;
			class FrontRearControl;

			class MixControls
            : public virtual MixVariables, public ctoot::audio::core::AudioControls

			{

			private:
				static float HALF_ROOT_TWO_;
				std::shared_ptr<BooleanControl> soloControl{ nullptr };
				std::shared_ptr<BooleanControl> muteControl{ nullptr };
				std::shared_ptr<ctoot::audio::fader::FaderControl> gainControl{ nullptr };
				std::shared_ptr<LCRControl> lcrControl { nullptr } ;
				std::shared_ptr<FrontRearControl> frontRearControl{ nullptr };
				std::weak_ptr<BusControls> busControls{ };

			public:
				std::shared_ptr<MixerControls> mixerControls{ nullptr };
				std::weak_ptr<MixerControls> wMixerControls{};

			private:
				bool  master { false };
				int channelCount{ 0 };
				bool mute{ false }, solo{ false };
				float gain{ 0 }, left{ 0 }, right{ 0 }, front{ 0 }, rear{ 0 };

			public:
				void derive(Control* c) override;

			public:
				virtual BooleanControl* getSoloControl();
				virtual BooleanControl* getMuteControl();
				bool isMaster() override;
                ctoot::audio::core::ChannelFormat* getChannelFormat() override;
				bool canBypass() override;
				bool isAlwaysVertical() override;
				bool canBeDeleted() override;
				bool hasPresets() override;
				virtual bool isSolo();
				virtual bool isMute();
				bool isEnabled() override;
				virtual bool hasSolo();
				float getGain() override;
				void getChannelGains(std::vector<float>* dest) override;
				float getSmoothingFactor() override;

			public:
				virtual EnumControl* createRouteControl(int stripId);
				virtual BooleanControl* createMuteControl();
				virtual BooleanControl* createSoloControl();

			public:
				MixControls(std::weak_ptr<MixerControls> mixerControls, int stripId, std::weak_ptr<BusControls> busControls, bool isMaster);
				~MixControls();

			public:
				virtual std::string getName();

			private:
				static float& HALF_ROOT_TWO();
							friend class LCRControl;
							friend class PanControl;
				//			friend class BalanceControl;
							friend class FrontRearControl;
				friend class GainControl;
			};
		}
	}
}
