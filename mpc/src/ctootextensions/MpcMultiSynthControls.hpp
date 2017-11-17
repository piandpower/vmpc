#pragma once
#include <synth/synths/multi/MultiSynthControls.hpp>

using namespace ctoot::synth::synths::multi;

namespace mpc {
	namespace ctootextensions {

		class MpcMultiSynthControls 
			: public MultiSynthControls {

			int MPC_MULTI_SYNTH_ID{ 5 };

		public:
			std::string getName();

		public:
			int ID = MPC_MULTI_SYNTH_ID;
			std::string NAME = "MpcMultiSynth";

		public:
			MpcMultiSynthControls();
			~MpcMultiSynthControls();

		};

	}
}
