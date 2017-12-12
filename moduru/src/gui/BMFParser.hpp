#pragma once

#include <vector>
#include <string>

#include "BMFStructs.hpp"

namespace moduru {
	namespace gui {
		class BMFParser
		{

		private:
			bmfont loadedFont;
			std::vector<std::vector<bool>> atlas;

		private:
			char* GetFileData(const char* pPath, size_t* pSize);
			void OrderCharsByID(std::vector<bmfont_char>* chars);
			bool GetBMFontData(const char* pBinary, size_t fileSize, bmfont* pBMFont);
			std::vector<std::vector<bool>> BMPAsBoolArrays(std::string filePath);

		public:
			std::vector<std::vector<bool>> getAtlas();
			bmfont getLoadedFont();

		public:
			BMFParser(std::string fontPath);
			~BMFParser();

		};
	}
}
