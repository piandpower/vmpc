#pragma once
#include <controls/sequencer/AbstractSequencerControls.hpp>


namespace mpc {
	namespace controls {
		namespace sequencer {
			namespace dialog {

				class CopyTrackControls
					: public mpc::controls::sequencer::AbstractSequencerControls
				{

				public:
					typedef mpc::controls::sequencer::AbstractSequencerControls super;
					void function(int i) override;
					void turnWheel(int i) override;

					CopyTrackControls(mpc::Mpc* mpc);

				};

			}
		}
	}
}
