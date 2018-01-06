#pragma once

/*
Copyright (C) 2017 and later, Youlean

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. This notice may not be removed or altered from any source distribution.


HOW TO USE:

IPlugConfigFile cf;

If you are using encryption
--------------------------------
cf.UseEncryption(true);
cf.SetEncriptionKey("12345");
--------------------------------

cf.SetFilePath("C:/Test.txt");
cf.ReadFile();

cf.WriteValue("value", 1024);
int out = cf.ReadValue<int>("value", 0);

cf.WriteFile();

*/

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <vector>
#include "IPlugStructs.h"

using namespace std;

class IPlugConfigFile
{
public:
	IPlugConfigFile();

	void UseEncryption(bool value);
	void SetEncriptionKey(string _key);
	
	void SetFilePath(string _filePath);

	void ReadFile();
	void WriteFile();

	template<typename ValueType>
	void WriteValue(string valueName, ValueType value, string groupName = "Default Group")
	{
		WriteString(groupName, valueName, T_to_string(value));
	}

	template<typename ValueType>
	ValueType ReadValue(string valueName, ValueType defaultValue, string groupName = "Default Group")
	{
		return string_to_T<ValueType>(ReadString(groupName, valueName, T_to_string(defaultValue)));
	}
		
private:
	vector<string> outStringVector;

	struct GroupProps
	{
		string groupName;
		int groupLineIndex;
		int groupLastValueIndex;
	};

	string key;
	vector<GroupProps> groupPropsVector;
	
	bool useEncryption = false;
	string filePath;

	// Convert T, which should be a primitive, to a std::string.
	template<typename T>
	static std::string T_to_string(T const & val)
	{
		std::ostringstream ostr;
		ostr << val;

		return ostr.str();
	}
	
	// Convert a std::string to T.	
	template<typename T>
	static T string_to_T(std::string const & val)
	{
		std::istringstream istr(val);
		T returnVal;
		istr >> returnVal;

		return returnVal;
	}

#ifdef _WIN32
	template<>
	static std::string string_to_T(std::string const & val)
	{
		return val;
	}
#endif // __WIN32

	void EncryptDecryptString(string &workingString);

	void CreateGroup(string groupName);
	bool GetGroupProps();

	void WriteString(string groupName, string valueName, string value);
	string ReadString(string groupName, string valueName, string defaultValue);

	void UpdateValue(size_t groupPropsPos, string valueName, string value);
	string GetValue(size_t groupPropsPos, string valueName, string defaultValue);

	void UpdateGroupPos(size_t fromGroup, size_t move);
	int FindGroupPropsPos(string groupName);
	
	void ReadFromPath(string filePath);
	void WriteToPath(string filePath);
};
