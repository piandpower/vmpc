#include <raw/fat/ShortNameGenerator.hpp>

#include <raw/fat/ShortName.hpp>

#include <lang/StrUtil.hpp>

#include <file/File.hpp>

using namespace moduru::raw::fat;
using namespace moduru::lang;
using namespace moduru::file;
using namespace std;

ShortNameGenerator::ShortNameGenerator(set<string> usedNames)
{
	this->usedNames = usedNames;
}

bool ShortNameGenerator::validChar(char toTest)
{
    if(toTest >= 'A' && toTest <= 'Z')
        return true;

    if(toTest >= '0' && toTest <= '9')
        return true;

    return (toTest == '_' || toTest == '^' || toTest == '$' || toTest == '~' || toTest == '!' || toTest == '#' || toTest == '%' || toTest == '&' || toTest == '-' || toTest == '{' || toTest == '}' || toTest == '(' || toTest == ')' || toTest == '@' || toTest == '\'' || toTest == '`');
}

bool ShortNameGenerator::isSkipChar(char c)
{
	return (c == '.') || (c == ' ');
}

string ShortNameGenerator::tidyString(string dirty)
{
	string result = "";
	for (int src = 0; src < dirty.length(); src++) {
		char toTest = toupper(dirty[src]);
		if (isSkipChar(toTest))
			continue;

		if (validChar(toTest)) {
			result.push_back(toTest);
		}
		else {
			result.push_back('_');
		}
	}
	return result;
}

bool ShortNameGenerator::cleanString(string s)
{
	for (auto i = 0; i < s.length(); i++) {
		if (isSkipChar(s[i]))
			return false;

		if (!validChar(s[i]))
			return false;

	}
	return true;
}

string ShortNameGenerator::stripLeadingPeriods(string str)
{
	string sb = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] != '.') {
			sb = str.substr(i);
			break;
		}
	}
	return sb;
}

ShortName* ShortNameGenerator::generateShortName(string longFullName)
{
	longFullName = StrUtil::toUpper(stripLeadingPeriods(longFullName));
	string longName;
	string longExt;
	size_t dotIdx = longFullName.find_last_of('.');
	bool forceSuffix = false;
	if (dotIdx == string::npos) {
		forceSuffix = !cleanString(longFullName);
		longName = tidyString(longFullName);
		longExt = "";
	}
	else {
		forceSuffix = !cleanString(longFullName.substr(0, dotIdx));
		longName = tidyString(longFullName.substr(0, dotIdx));
		longExt = tidyString(longFullName.substr(dotIdx + 1));
	}
	string shortExt = (longExt.length() > 3) ? longExt.substr(0, 3) : longExt;
	if (forceSuffix || (longName.length() > 8) || usedNames.find(StrUtil::toLower(ShortName(longName, shortExt).asSimpleString())) != usedNames.end()) {
		auto const maxLongIdx = longName.length() < 8 ? longName.length() : 8;
		for (int i = 1; i < 99999; i++) {
			string serial = "~" + to_string(i);
			int serialLen = serial.length();
			int trimIndex = maxLongIdx < 8 - serialLen ? maxLongIdx : 8 - serialLen;
			auto const shortName = longName.substr(0, trimIndex) + serial;
			auto const result = new ShortName(shortName, shortExt);
			if (usedNames.find(StrUtil::toLower(result->asSimpleString())) == usedNames.end()) {
				return result;
			}
		}
		string error = "could not generate short name for " + longFullName;
		return nullptr;
	}
	return new ShortName(longName, shortExt);
}
