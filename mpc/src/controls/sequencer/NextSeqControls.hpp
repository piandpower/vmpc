#pragma once
#include <controls/sequencer/AbstractSequencerControls.hpp>

namespace mpc {
	namespace controls {
		namespace sequencer {

			class NextSeqControls
				: public AbstractSequencerControls
			{

			public:
				typedef AbstractSequencerControls super;
				void turnWheel(int i) override;
				void function(int i) override;

				NextSeqControls(mpc::Mpc* mpc);

			};

		}
	}
}
