#pragma once

#include <vector>

namespace mpc {
	namespace file {
		namespace all {

			class Bar;

			class Bar
			{

			public:
				int ticksPerBeat{};
				int lastTick{};
				int barLength{};
				std::vector<char> saveBytes{};

			public:
				int getTicksPerBeat();
				int getDenominator();
				int getNumerator();
				int getLastTick();

			public:
				std::vector<char> getBytes();

				Bar(std::vector<char> bytes, Bar* previousBar);
				Bar(int ticksPerBeat, int lastTick);
			};

		}
	}
}
