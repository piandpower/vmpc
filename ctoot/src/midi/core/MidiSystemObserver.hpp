#pragma once
#include <observer/Observer.hpp>

namespace ctoot {
	namespace midi {
		namespace core {

			class DefaultMidiSystem;

			class MidiSystemObserver
				: public moduru::observer::Observer
			{

			public:
				DefaultMidiSystem* dms{ nullptr };

			public:
				void update(moduru::observer::Observable* o, boost::any a) override;

			public:
				MidiSystemObserver(DefaultMidiSystem* dms);
				~MidiSystemObserver();


			private:
				friend class DefaultMidiSystem;
			};
	
		}
	}
}
