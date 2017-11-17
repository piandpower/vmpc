#pragma once

#include <synth/ControlChange.hpp>
#include <control/CompoundControl.hpp>

#include <observer/Observer.hpp>
#include <thirdp/concurrentqueue.h>

namespace ctoot {
	namespace synth {

		class SynthChannelControls
			: public ctoot::control::CompoundControl
			, public moduru::observer::Observer
		{

		public:
			typedef ctoot::control::CompoundControl super;

		private:
			bool learn{ false };
			ctoot::control::Control* learnControl{ nullptr };
			std::vector<ctoot::control::Control*> map{};
			moodycamel::ConcurrentQueue<ControlChange*> changeQueue{};
			//std::thread* changeThread { nullptr };
			moduru::observer::Observable* observable{ nullptr };

		public:
			void close() override;

		public:
			void notifyParent(ctoot::control::Control* obj) override;
			virtual void ensureMapExists();

		public:
			void update(moduru::observer::Observable* o, boost::any a) override;
			//    void run() override;

		public:
			virtual void relay(ControlChange* change);

		public:
			virtual int getMappedControlId(int n);
			virtual void setMappedControlId(int n, int cid);
			bool canLearn() override;
			bool getLearn() override;
			void setLearn(bool learn) override;

		public:
			SynthChannelControls(int id, std::string name);
			virtual ~SynthChannelControls();

		};

	}
}
