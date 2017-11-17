#include <file/FileSystem.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
//osx
#endif

using namespace moduru::file;
using namespace std;



#ifdef _WIN32
vector<string> FileSystem::getRootDirectories() {
	int drives = 0;

	drives = GetLogicalDrives();

	// iterate over roots, ignoring those that the security manager denies
	vector<string> result;
	for (int i = 0; i <= 25; i++) {  // 0->A, 1->B, 2->C...
		if ((drives & (1 << i)) != 0) {
			string str = "";
			str.append(to_string((char)('A' + i)));
			str.append(":\\");
			result.push_back(str);
		}
	}
	return result;
}
#else
vector<string> FileSystem::getRootDirectories() {
	vector<string> result;
	return result;
}
#endif