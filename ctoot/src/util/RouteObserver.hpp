#pragma once
#include <observer/Observer.hpp>
#include <audio/mixer/MainMixProcess.hpp>
#include <audio/mixer/AudioMixer.hpp>

namespace ctoot {
	namespace util {

		class RouteObserver
			: public moduru::observer::Observer
		{

		private:
			ctoot::audio::mixer::MainMixProcess* mmp;
			ctoot::audio::mixer::AudioMixer* mixer;

		public:
			void update(moduru::observer::Observable* o, boost::any a) override;

			RouteObserver(ctoot::audio::mixer::MainMixProcess* mmp, ctoot::audio::mixer::AudioMixer* mixer);

		};
	}
}
