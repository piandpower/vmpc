#pragma once
#include <string>

namespace ctoot {
	namespace audio {
		namespace core {

			class MetaInfo
			{

			private:
				std::string sourceLabel = { "" };
				std::string sourceLocation = { "" };

			public:
				std::string getSourceLabel();
				std::string getSourceLocation();


				MetaInfo(std::string sourceLabel, std::string sourceLocation);
				MetaInfo(std::string sourceLabel) : MetaInfo(sourceLabel, "") {};

			};

		}
	}
}
