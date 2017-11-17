#pragma once

#include <audio/core/SimpleAudioProcess.hpp>

namespace mpc {
	namespace ctootextensions {

		class MpcMixerInterconnection;

		class InterconnectionInputProcess
			: public ctoot::audio::core::SimpleAudioProcess
		{

		public:
			typedef ctoot::audio::core::SimpleAudioProcess super;
			int processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) override;

			MpcMixerInterconnection* mmi{ nullptr };
			ctoot::audio::core::AudioBuffer* sharedBuffer{ nullptr };

			InterconnectionInputProcess(MpcMixerInterconnection* mmi, ctoot::audio::core::AudioBuffer* sharedBuffer);
			~InterconnectionInputProcess();

		private:
			//friend class MpcMixerInterconnection;
			//friend class MpcMixerInterconnection_InterconnectionOutputProcess;
		};
	}
}
