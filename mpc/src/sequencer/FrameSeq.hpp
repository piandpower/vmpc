#pragma once
#include <audio/server/AudioClient.hpp>
#include <sequencer/Clock.hpp>

#include <memory>

namespace mpc {

	namespace sequencer {

		class Sequencer;

		class FrameSeq
			: public ctoot::audio::server::AudioClient
		{

		private:
			bool running{ false };
			bool metronome{ false };
			Clock clock;
			std::weak_ptr<Sequencer> sequencer{};
			int tickFrameOffset{ 0 };
			
			void move(int newTickPos);
			void repeatPad(int tick);
			void checkNextSq();

		public:

			void start();
			void startMetronome();
			void work(int nFrames) override;
			void setEnabled(bool b) override {};
			int getEventFrameOffset(int tick);
			void stop();
			bool isRunning();
			int getTickPosition();

			FrameSeq(std::weak_ptr<Sequencer> sequencer);
			~FrameSeq();

		};

	}
}
