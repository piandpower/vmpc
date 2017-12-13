#define _CRT_SECURE_NO_WARNINGS
#include <Logger.hpp>

using namespace moduru;
using namespace std;

Logger::Logger()
{
	remove(path.c_str());
}
Logger Logger::l;

void Logger::setPath(std::string s) {
	path = s;
	remove(path.c_str());
}

void Logger::log(std::string s) {
	if (path.compare("") == 0) return;
	s += "\n";
	fp = fopen(path.c_str(), "a+b");
	fwrite(s.c_str(), s.length(), 1, fp);
	fclose(fp);
}
