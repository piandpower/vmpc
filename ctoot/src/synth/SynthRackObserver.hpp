#pragma once
#include <observer/Observer.hpp>

#include <memory>

namespace ctoot {
	namespace synth {

		class SynthRack;
		class SynthRackControls;

		class SynthRackObserver
			: public moduru::observer::Observer
		{

		public:
			SynthRack* sr{ nullptr };
			std::weak_ptr<SynthRackControls> controls{ };

		public:
			void update(moduru::observer::Observable* obs, boost::any a) override;

		public:
			SynthRackObserver(SynthRack* sr, std::weak_ptr<SynthRackControls> controls);
			~SynthRackObserver();

		};

	}
}
