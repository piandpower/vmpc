#pragma once

#include <observer/Observable.hpp>
#include <sampler/MixerChannel.hpp>

#include <memory>

namespace mpc {

	class Mpc;

	namespace sampler {

		class Pad
			: public moduru::observer::Observable
		{

		private:
			int note{ 0 };
			std::shared_ptr<MixerChannel> mixerChannel{};
			int number{ 0 };

		public:
			void setNote(int i);
			int getNote();
			std::weak_ptr<MixerChannel> getMixerChannel();
			int getNumber();

		public:
			Pad(mpc::Mpc* mpc, int number);
			~Pad();

		};

	}
}
