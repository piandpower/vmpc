#pragma once

#include <observer/Observable.hpp>

#include <vector>
#include <memory>
#include <string>

namespace mpc {

	class Mpc;

	namespace hardware {

		class Button;
		class DataWheel;
		class Pot;
		class Led;
		class Slider;
		class HwPad;

		class Hardware
			: public moduru::observer::Observable
		{

		private:
			std::vector<std::shared_ptr<HwPad> > pads;
			std::vector<std::shared_ptr<Button> > buttons;
			std::vector<std::shared_ptr<Pot> > pots;
			std::vector<std::shared_ptr<Led> > leds;

			std::shared_ptr<DataWheel> dataWheel;
			std::shared_ptr<Slider> slider;

		public:
			std::weak_ptr<HwPad> getPad(int index);
			std::weak_ptr<Button> getButton(std::string label);
			std::weak_ptr<DataWheel> getDataWheel();

		public:
			Hardware(mpc::Mpc* mpc);
			~Hardware();

		};

	}
}
