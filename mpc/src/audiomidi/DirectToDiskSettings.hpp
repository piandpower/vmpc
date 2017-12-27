#pragma once
#include <string>

namespace mpc {
	namespace audiomidi {

		class DirectToDiskSettings

		{

		public:
			int lengthInFrames{};
			std::string outputFolder{};
			bool split{};

		public:
			DirectToDiskSettings(int lengthInFrames, std::string outputFolder, bool split);

		};
	}
}
