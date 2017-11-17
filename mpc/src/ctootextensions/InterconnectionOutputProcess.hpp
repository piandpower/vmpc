#pragma once
#include <audio/core/SimpleAudioProcess.hpp>

namespace mpc {
	namespace ctootextensions {

		class MpcMixerInterconnection;
		class InterconnectionOutputProcess
			: public ctoot::audio::core::SimpleAudioProcess
		{

		public:
			typedef ctoot::audio::core::SimpleAudioProcess super;
			int processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) override;

			MpcMixerInterconnection* mmi{ nullptr };
			ctoot::audio::core::AudioBuffer* sharedBuffer{ nullptr };

			InterconnectionOutputProcess(MpcMixerInterconnection* mmi, ctoot::audio::core::AudioBuffer* sharedBuffer);
			~InterconnectionOutputProcess();

		private:
			//friend class MpcMixerInterconnection;
			//friend class MpcMixerInterconnection_InterconnectionInputProcess;
	
		};
	
	}
}
