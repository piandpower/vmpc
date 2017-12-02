#ifndef _LOGGER_
#define _LOGGER_

#pragma once
#include <stdio.h>
#include <string>

namespace moduru {

	class Logger {

	private:
		FILE* fp;
		std::string path = "";

	public:

		static Logger l;

		void log(std::string s);
	
		Logger();
		Logger(std::string logFile);

	};
}

#define MLOG(arg) moduru::Logger::l.log(arg);
#endif