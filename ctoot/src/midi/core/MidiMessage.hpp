#pragma once

#include <vector>

namespace ctoot {
	namespace midi {
		namespace core {

			class MidiMessage

			{

			public:
				std::vector<char> data{};
				int length{};

			public:
				virtual int getLength();
				virtual std::vector<char>* getMessage();
				virtual int getStatus();

			public:
				virtual void setMessage(std::vector<char> data, int length);

				MidiMessage* clone();

				MidiMessage(std::vector<char> data);
				MidiMessage();

			};

		}
	}
}
