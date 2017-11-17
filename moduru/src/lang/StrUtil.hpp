#pragma once
#include <string>

namespace moduru {

	namespace lang {

		class StrUtil {

		public:
			static std::string padLeft(std::string str, std::string pad, int size);
			static std::string padRight(std::string str, std::string pad, int size);
			static std::string trim(std::string str);
			static void		   trim(std::string* str);
			static std::string toUpper(std::string str);
			static std::string toLower(std::string str);
			static bool eqIgnoreCase(std::string s1, std::string s2);
			static std::string replaceAll(std::string str, char c, std::string replacement);
			static std::string TrimDecimals(std::string str, int decimalCount);
			static std::string TrimDecimals(float f, int decimalCount);
			
			static bool hasEnding(std::string const &fullString, std::string const &ending);

			static std::string subStr(const std::string str, int start, int end);
			static int cntByte(unsigned char cChar);
		};
	}
}
