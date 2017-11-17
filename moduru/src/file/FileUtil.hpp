#pragma once
#include <string>
#include <vector>

#include <stdio.h>

namespace moduru {
	namespace file {

		class FileUtil {

		public:
			static bool Exists(std::string path);
			static std::vector<std::string> splitName(std::string name);
			static int GetLastSeparator(std::string path);
			static bool IsDirectory(std::string path);
			static std::string getSeparator();

		};

	}
}
