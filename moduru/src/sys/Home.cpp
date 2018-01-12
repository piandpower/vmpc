#define _CRT_SECURE_NO_WARNINGS
#include <sys/Home.hpp>

#ifdef __APPLE__
#include <pwd.h>
#include <unistd.h>
#endif

using namespace moduru::sys;
using namespace std;


#ifdef __linux__
#include <unistd.h>
#include <pwd.h>
#endif

#ifdef _WIN32
string Home::get() {
	auto homeDrive = string(getenv("HOMEDRIVE"));
	auto homePath = string(getenv("HOMEPATH"));
	return homeDrive + homePath;
}
#else
string Home::get() {
	char* home = getpwuid(getuid())->pw_dir;
	string res = string(home);
	return res;
}
#endif
