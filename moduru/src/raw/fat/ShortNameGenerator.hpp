#pragma once

#include <set>
#include <string>

namespace moduru {
	namespace raw {
		namespace fat {

			class ShortName;

			class ShortNameGenerator
			{

			private:
				std::set<std::string> usedNames;

			public:
				static bool validChar(char toTest);
				static bool isSkipChar(char c);

			private:
				std::string tidyString(std::string dirty);
				bool cleanString(std::string s);
				std::string stripLeadingPeriods(std::string str);

			public:
				ShortName* generateShortName(std::string longFullName);

				ShortNameGenerator(std::set<std::string> usedNames);
			};

		}
	}
}
