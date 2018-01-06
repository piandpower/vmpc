#pragma once

#include <fstream>
#include <vector>

class Windows_UTF_Converter
{
public:
#ifdef WIN32
	std::string utf16_to_utf8(const std::wstring& wstr)
	{
		std::string convertedString;
		int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<char> buffer(requiredSize);
			WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}
		return convertedString;
	}

	std::wstring utf8_to_utf16(const std::string& str)
	{
		std::wstring convertedString;
		int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
		if (requiredSize > 0)
		{
			std::vector<wchar_t> buffer(requiredSize);
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}

		return convertedString;
	}
#endif
};

namespace utf8
{
#ifdef WIN32
	class ifstream : public std::ifstream, Windows_UTF_Converter
	{
	public:
		ifstream() : std::ifstream() {}

		explicit ifstream(const char* fileName, std::ios_base::open_mode mode = std::ios_base::in) :
			std::ifstream(utf8_to_utf16(fileName), mode)
		{
		}

		explicit ifstream(const std::string& fileName, std::ios_base::open_mode mode = std::ios_base::in) :
			std::ifstream(utf8_to_utf16(fileName), mode)
		{
		}

		void open(const char* fileName, std::ios_base::open_mode mode = std::ios_base::in)
		{
			std::ifstream::open(utf8_to_utf16(fileName), mode);
		}

		void open(const std::string& fileName, std::ios_base::open_mode mode = std::ios_base::in)
		{
			std::ifstream::open(utf8_to_utf16(fileName), mode);
		}
	};

	class ofstream : public std::ofstream, Windows_UTF_Converter
	{
	public:
		ofstream() : std::ofstream() {}

		explicit ofstream(const char* fileName, std::ios_base::open_mode mode = std::ios_base::out) :
			std::ofstream(utf8_to_utf16(fileName), mode)
		{
		}

		explicit ofstream(const std::string& fileName, std::ios_base::open_mode mode = std::ios_base::out) :
			std::ofstream(utf8_to_utf16(fileName), mode)
		{
		}

		void open(const char* fileName, std::ios_base::open_mode mode = std::ios_base::out)
		{
			std::ofstream::open(utf8_to_utf16(fileName), mode);
		}

		void open(const std::string& fileName, std::ios_base::open_mode mode = std::ios_base::out)
		{
			std::ofstream::open(utf8_to_utf16(fileName), mode);
		}
	};
#else
	typedef std::ifstream ifstream;
	typedef std::ofstream ofstream;
#endif
}