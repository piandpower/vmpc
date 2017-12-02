#pragma once
#include <controls/sequencer/AbstractSequencerControls.hpp>

namespace mpc {
	namespace controls {
		namespace sequencer {
			namespace window {

				class MultiRecordingSetupControls
					: public AbstractSequencerControls
				{

				public:
					typedef AbstractSequencerControls super;

				private:
					int yPos{};

				public:
					void init() override;

				public:
					void left() override;
					void right() override;
					void turnWheel(int i) override;
					//void keyEvent(unsigned char c) override;

					MultiRecordingSetupControls(mpc::Mpc* mpc);

				};

			}
		}
	}
}
