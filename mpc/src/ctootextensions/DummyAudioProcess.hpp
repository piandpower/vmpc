#pragma once
#include <audio/core/SimpleAudioProcess.hpp>

namespace mpc {
	namespace ctootextensions {

		class DummyAudioProcess
			: public ctoot::audio::core::SimpleAudioProcess
		{

		public:
			typedef ctoot::audio::core::SimpleAudioProcess super;
			int processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) override;

			DummyAudioProcess();

		};

	}
}
