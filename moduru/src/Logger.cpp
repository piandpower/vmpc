#define _CRT_SECURE_NO_WARNINGS
#include <Logger.hpp>

using namespace moduru;
using namespace std;

Logger::Logger()
{
#ifdef _WIN32
	this->path = "c:/temp/vmpc.log";
#else
	this->path = "/tmp/vmpc.log";
#endif
	remove(path.c_str());
}
Logger Logger::l;

Logger::Logger(string logFile) {
#ifdef _WIN32
	if (path.find("c:/temp/") != string::npos) {
#else
	if (path.find("/tmp/") != string::npos) {
#endif
		this->path = logFile;
		remove(path.c_str());
	}
}

void Logger::log(std::string s) {
#ifdef _WIN32
	if (path.find("c:/temp/") != string::npos) {
#else
	if (path.find("/tmp/") != string::npos) {
#endif
		fp = fopen(path.c_str(), "a+b");
		fwrite(s.c_str(), s.length(), 1, fp);
		fclose(fp);
	}
}
