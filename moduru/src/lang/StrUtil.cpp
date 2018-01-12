#include <lang/StrUtil.hpp>

#ifdef __linux__
#include <cstring>
#endif

using namespace moduru::lang;
using namespace std;

string StrUtil::replaceAll(string str, char c, string replacement) {
	if (replacement.length() > 1) return "<error>";
	string res;
	for (char ch : str) {
		if (ch == c) {
			if (replacement.length() == 0 || replacement == "") {
				continue;
			}
			else {
				res.push_back(replacement[0]);
			}
		}
		else {
			res.push_back(ch);
		}
	}
	return res;
}

bool StrUtil::eqIgnoreCase(string s1, string s2) {
	if (s1.length() != s2.length()) return false;
	for (int i = 0; i < s1.length(); i++) {
		if (s1[i] != tolower(s2[i]) && s1[i] != toupper(s2[i])) return false;
	}
	return true;
}

string StrUtil::toUpper(string str) {
	string res = str;
	for (int i = 0; i < res.length(); i++)
		res[i] = toupper(res[i]);
	return res;
}

string StrUtil::toLower(string str) {
	string res = str;
	for (int i = 0; i < res.length(); i++)
		res[i] = tolower(res[i]);
	return res;
}

string StrUtil::padLeft(string str, string pad, int size) {
	string result = "";
	for (int i = 0; i < size - str.size(); i++) {
		result.append(pad);
	}
	int counter = 0;
	for (int i = size - str.size(); i < size; i++) {
		result.append(str.substr(counter++, 1));
	}
	return result;
};

string StrUtil::padRight(string str, string pad, int size) {
	string result = "";
	for (int i = 0; i < str.size(); i++) {
		result.append(str.substr(i, 1));
	}
	for (int i = str.size(); i < size; i++) {
		result.append(pad);
	}
	return result;
}

string StrUtil::trim(string str) {
	string copy = str;
	while (!copy.empty() && copy.back() >=0 && copy.back() <= 255 && isspace(copy.back())) copy.pop_back();
	return copy;
}

void StrUtil::trim(string* str) {
	while (!str->empty() && isspace(str->back())) str->pop_back();
}

string StrUtil::TrimDecimals(string str, int count) {
	float f = NULL;
	string copy = str;
	try {
		f = stof(str);
	}
	catch (exception e) {
		   //e can be discarded
		return str;
	}
	if (f != NULL) {
		int index = str.find(".");
		int counted = (int) (str.length()) - index + 1;
		if (counted <= count) return str; // no trimming possible
		copy = copy.substr(0, index + 1 + count - 0);
		return copy;
	}
	return str;
}

string StrUtil::TrimDecimals(float f, int count) {
	return TrimDecimals(to_string(f), count);
}

bool StrUtil::hasEnding(std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

string StrUtil::subStr(const string str, int start, int end)
{
	static char cRes[1024];
	char* pRes = cRes;
	int i = 0, iPos = 0;
	int iByte;
	auto cStr = str.c_str();
	auto length = end - start;
	while (cStr[i] != '\0') {
		iByte = cntByte(cStr[i]);
		if (start <= iPos && iPos < start + length) {
			memcpy(pRes, (cStr + i), iByte);
			pRes += iByte;
		}
		i += iByte;
		iPos++;
	}
	*pRes = '\0';
	return cRes;
}

int StrUtil::cntByte(unsigned char cChar)
{
	int iByte;

	if ((cChar >= 0x00) && (cChar <= 0x7f)) {
		iByte = 1;
	}
	else if ((cChar >= 0xc2) && (cChar <= 0xdf)) {
		iByte = 2;
	}
	else if ((cChar >= 0xe0) && (cChar <= 0xef)) {
		iByte = 3;
	}
	else if ((cChar >= 0xf0) && (cChar <= 0xf7)) {
		iByte = 4;
	}
	else if ((cChar >= 0xf8) && (cChar <= 0xfb)) {
		iByte = 5;
	}
	else if ((cChar >= 0xfc) && (cChar <= 0xfd)) {
		iByte = 6;
	}
	else {
		iByte = 0;
	}

	return iByte;
}
