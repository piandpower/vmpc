#include "IPlugConfigFile.h"

IPlugConfigFile::IPlugConfigFile()
{
	key.reserve(2048);
}

void IPlugConfigFile::UseEncryption(bool value)
{
	useEncryption = value;

	if (useEncryption)
	{
		if (key.size() == 0)
		{
			// Set default values
			key.push_back(7);
			key.push_back(6);
			key.push_back(5);
			key.push_back(1);
			key.push_back(3);
		}
	}
}

void IPlugConfigFile::SetEncriptionKey(string _key)
{
	key = _key;
}

void IPlugConfigFile::SetFilePath(string _filePath)
{
	filePath = _filePath;
}

void IPlugConfigFile::ReadFile()
{
	ReadFromPath(filePath);

	if (!GetGroupProps())
	{
		outStringVector.resize(0);
		WriteFile();
	}
}

void IPlugConfigFile::WriteFile()
{
	WriteToPath(filePath);
}

void IPlugConfigFile::WriteString(string groupName, string valueName, string value)
{
	int groupPropsPos = FindGroupPropsPos(groupName);

	if (groupPropsPos == -1)
	{
		CreateGroup(groupName);
		groupPropsPos = groupPropsVector.size() - 1;
	}

	UpdateValue(groupPropsPos, valueName, value);
}

string IPlugConfigFile::ReadString(string groupName, string valueName, string defaultValue)
{
	int groupPropsPos = FindGroupPropsPos(groupName);

	if (groupPropsPos == -1) return defaultValue;

	return GetValue(groupPropsPos, valueName, defaultValue);
}

void IPlugConfigFile::UpdateValue(size_t groupPropsPos, string valueName, string value)
{
	int groupLineIndex = groupPropsVector[groupPropsPos].groupLineIndex;
	int groupLastValueIndex = groupPropsVector[groupPropsPos].groupLastValueIndex;

	if (groupLineIndex != groupLastValueIndex)
	{
		for (int i = groupLineIndex + 1; i <= groupLastValueIndex; i++)
		{
			size_t pos = outStringVector[i].find(valueName + string("="));

			if (pos != string::npos)
			{
				outStringVector[i] = valueName + string("=") + value;
				return;
			}
		}
	}

	int pushNewValueToIndex = groupPropsVector[groupPropsPos].groupLastValueIndex + 1;

	if (pushNewValueToIndex >= outStringVector.size())
		outStringVector.push_back(valueName + string("=") + value);
	else
		outStringVector.insert(outStringVector.begin() + pushNewValueToIndex, valueName + string("=") + value);

	UpdateGroupPos(groupPropsPos, 1);
}

string IPlugConfigFile::GetValue(size_t groupPropsPos, string valueName, string defaultValue)
{
	int groupLineIndex = groupPropsVector[groupPropsPos].groupLineIndex;
	int groupLastValueIndex = groupPropsVector[groupPropsPos].groupLastValueIndex;

	if (groupLineIndex != groupLastValueIndex)
	{
		for (int i = groupLineIndex + 1; i <= groupLastValueIndex; i++)
		{
			size_t pos = outStringVector[i].find(valueName + string("="));

			if (pos != string::npos)
			{
				size_t valueStart = pos + valueName.size() + 1;

				return outStringVector[i].substr(valueStart, outStringVector[i].size() - valueStart);
			}
		}
	}

	return defaultValue;
}

void IPlugConfigFile::UpdateGroupPos(size_t fromGroup, size_t move)
{
	groupPropsVector[fromGroup].groupLastValueIndex += move;

	for (size_t i = fromGroup + 1; i < groupPropsVector.size(); i++)
	{
		groupPropsVector[i].groupLineIndex += move;
		groupPropsVector[i].groupLastValueIndex += move;
	}
}

int IPlugConfigFile::FindGroupPropsPos(string groupName)
{
	for (int i = 0; i < groupPropsVector.size(); i++)
	{
		if (groupPropsVector[i].groupName == groupName) return i;
	}

	return -1;
}

bool IPlugConfigFile::GetGroupProps()
{
	groupPropsVector.resize(0);

	for (int i = 0; i < outStringVector.size(); i++)
	{
		size_t groupPos = outStringVector[i].find("[");

		if (groupPos != string::npos)
		{
			GroupProps props;

			props.groupName = outStringVector[i].substr(1, outStringVector[i].size() - 2);
			props.groupLineIndex = i;
			props.groupLastValueIndex = 0;

			groupPropsVector.push_back(props);
		}
	}

	if (groupPropsVector.size() > 0)
	{
		// Set last value index in the props
		groupPropsVector.back().groupLastValueIndex = outStringVector.size() - 1;

		for (int i = groupPropsVector.size() - 1; i > 0; i--)
		{
			groupPropsVector[i - 1].groupLastValueIndex = groupPropsVector[i].groupLineIndex - 1;
		}

		return true;
	}

	return false;
}

void IPlugConfigFile::EncryptDecryptString(string &workingString)
{
	if (key.size() > 0)
	{
		int keyPos = 0;

		for (int i = 0; i < workingString.size(); i++)
		{
			char in = workingString[i];

			int keyChar = (int)key[keyPos];
			char encryptChar = char((i + keyChar) % 128);

			char out = in ^ encryptChar;

			workingString[i] = out;

			keyPos++;
			if (keyPos >= key.size()) keyPos = 0;
		}
	}
}

void IPlugConfigFile::ReadFromPath(string filePath)
{
	string input;
	outStringVector.resize(0);

	ifstream myfile(filePath, ios::in | ios::binary | ios::ate);

	if (myfile.is_open())
	{
		streampos size = myfile.tellg();
		input.resize(size);

		myfile.seekg(0, ios::beg);
		myfile.read(&input[0], size);
		myfile.close();
	}

	// Put encryption function here
	if (useEncryption) EncryptDecryptString(input);
	
	istringstream iss(input);

	string line;
	while (getline(iss, line))
	{
		outStringVector.push_back(line);
	}
}

void IPlugConfigFile::WriteToPath(string filePath)
{
	string output;

	for (int i = 0; i < outStringVector.size(); i++)
	{
		output.append(outStringVector[i]);
		output.append("\n");
	}

	// Put encryption function here
	if (useEncryption) EncryptDecryptString(output);
	
	ofstream myfile;
	myfile.open(filePath, ios::binary);
	if (myfile.is_open())
	{
		myfile.write(&output[0], output.size());
		myfile.close();
	}
}

void IPlugConfigFile::CreateGroup(string groupName)
{
	outStringVector.push_back(string("[") + groupName + string("]"));

	GroupProps props;
	props.groupName = groupName;
	props.groupLineIndex = outStringVector.size() - 1;
	props.groupLastValueIndex = outStringVector.size() - 1;
	
	groupPropsVector.push_back(props);
}

#ifdef __APPLE__
template <>
std::string IPlugConfigFile::string_to_T<std::string>(std::string const &val)
{
	return val;
}
#endif // __APPLE__