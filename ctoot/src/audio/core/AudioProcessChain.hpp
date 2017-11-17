#pragma once
#include <audio/core/AudioProcess.hpp>
#include <audio/core/AudioControlsChain.hpp>
#include <audio/core/AudioControls.hpp>

#include <observer/Observer.hpp>
#include <thirdp/concurrentqueue.h>

#include <cstdint>

namespace ctoot {

	namespace control {
		class ChainMutation;
	}

	namespace audio {
		namespace core {

			class AudioProcessChain
				: public virtual AudioProcess
			{

			public:
				std::shared_ptr<AudioControlsChain> controlChain{ nullptr };
				std::vector<AudioProcess*> processes{};

			private:
				moodycamel::ConcurrentQueue<ctoot::control::ChainMutation*> mutationQueue {};
				moduru::observer::Observer* controlChainObserver{ nullptr };
				bool debugTimes{ false };
				std::vector <int64_t> t{};

			public:
				void open() override;

			private:
				int debugIndex{};
				int64_t prevNanos{};
				int64_t tstart{};
				int64_t elapsed{};

			public:
				int processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) override;
				virtual int debugProcessAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames);
				void close() override;
				virtual std::string getName();

			public:
				virtual AudioProcess* createProcess(AudioControls* controls);
				virtual void processMutations();

			public:
				AudioProcessChain(std::shared_ptr<AudioControlsChain> controlChain);
				~AudioProcessChain();

			private:
				friend class ControlChainObserver;

			};

		}
	}
}
