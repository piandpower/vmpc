#define _CRT_SECURE_NO_WARNINGS
#include <file/FileUtil.hpp>


#if defined (_WIN32)
#include <thirdp/dirent.h>
#endif

#if defined (__APPLE__)
#include <sys/dirent.h>
#include <sys/dir.h>
#endif

#if defined (__linux__)
#include <dirent.h>
#endif

using namespace moduru::file;
using namespace std;

bool FileUtil::Exists(std::string path) {

	DIR* dir = opendir(path.c_str());
	FILE* file = fopen(path.c_str(), "r");
	if (dir != NULL) {
		closedir(dir);
		return true;
	} else if (file != NULL) {
		fclose(file);
		return true;

	}
	return false;
}

int FileUtil::GetLastSeparator(std::string path) {
	return (int)(path.find_last_of(getSeparator()));
}

bool FileUtil::IsDirectory(std::string path) {
	DIR* d = opendir(path.c_str());
	if (d != NULL) {
		closedir(d);
		return true;
	}
	return false;
}

std::vector<std::string> FileUtil::splitName(std::string name) {
	std::vector<std::string> res(2);
	if (name.find(".") == std::string::npos) {
		res[0] = name;
		res[1] = "";
		return res;
	}
	auto i = name.find_last_of(".");
	res[0] = name.substr(0, i);
	res[1] = name.substr(i + 1);
	return res;
}

string FileUtil::getSeparator() {
#ifdef _WIN32
	return "\\";
#else
	return "/";
#endif
}
