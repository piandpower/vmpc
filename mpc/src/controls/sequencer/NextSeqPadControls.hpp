#pragma once
#include <controls/sequencer/AbstractSequencerControls.hpp>

namespace mpc {
	namespace controls {
		namespace sequencer {

			class NextSeqPadControls
				: public AbstractSequencerControls
			{

			public:
				typedef AbstractSequencerControls super;
				void right() override;
				void turnWheel(int i) override;
				void function(int i) override;

				NextSeqPadControls(mpc::Mpc* mpc);

			public:
				void pad(int i, int velo, bool repeat, int tick) override;
			};

		}
	}
}
