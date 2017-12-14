#include "BMFParser.hpp"

#include <file/FileUtil.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

using namespace moduru::gui;
using namespace std;

BMFParser::BMFParser(string fontPath) {

	size_t fileSize = 0;
	char* data = GetFileData(fontPath.c_str(), &fileSize);
	if (GetBMFontData(data, fileSize, &loadedFont))
	{
		printf("Loaded BMFont data correctly.\n");
	}
	if (data != NULL)
		free(data);

	string bmpFileName = loadedFont.pages[0].name;
	bmpFileName = bmpFileName.substr(0, loadedFont.pages[0].length);
	string fontDir = fontPath.substr(0, moduru::file::FileUtil::GetLastSeparator(fontPath));
	atlas = BMPAsBoolArrays(fontDir + bmpFileName);
}

void BMFParser::OrderCharsByID(std::vector<bmfont_char>* chars) {
	std::vector<bmfont_char> result(255);
	for (int i = 0; i < chars->size(); i++) {
		auto destid = chars->at(i).id;
		result[destid] = chars->at(i);
	}
	chars->clear();
	for (int i = 0; i < result.size(); i++)
		chars->push_back(result[i]);
}

bool BMFParser::GetBMFontData(const char* pBinary, size_t fileSize, bmfont* pBMFont)
{
	if (!((pBinary[0] == 'B' && pBinary[1] == 'M' &&
		pBinary[2] == 'F' && pBinary[3] == 3)))
		return false;

	bmfont_stream stream((uint8_t*)pBinary, fileSize);
	stream.offsetBy(4);
	while (!stream.isEOF())
	{
		uint8_t blockID = stream.getU8();
		int32_t blockSize = stream.getU32();
		switch (blockID)
		{
		case BMFONT_BLOCK_TYPE_INFO:
		{
			pBMFont->info.fontSize = stream.getS16();
			pBMFont->info.bitField = stream.getU8();
			pBMFont->info.charSet = stream.getU8();
			pBMFont->info.stretchH = stream.getU16();
			pBMFont->info.aa = stream.getU8();
			pBMFont->info.paddingUp = stream.getU8();
			pBMFont->info.paddingRight = stream.getU8();
			pBMFont->info.paddingDown = stream.getU8();
			pBMFont->info.paddingLeft = stream.getU8();
			pBMFont->info.spacingHoriz = stream.getU8();
			pBMFont->info.spacingVert = stream.getU8();
			pBMFont->info.outline = stream.getU8();
			const char* name = (const char*)stream.getPtr();
			stream.offsetBy(strlen(name) + 1);
			break;
		}
		case BMFONT_BLOCK_TYPE_COMMON:
		{
			pBMFont->common.lineHeight = stream.getU16();
			pBMFont->common.base = stream.getU16();
			pBMFont->common.scaleW = stream.getU16();
			pBMFont->common.scaleH = stream.getU16();
			pBMFont->common.pages = stream.getU16();
			pBMFont->common.bitField = stream.getU8();
			pBMFont->common.alphaChnl = stream.getU8();
			pBMFont->common.redChnl = stream.getU8();
			pBMFont->common.greenChnl = stream.getU8();
			pBMFont->common.blueChnl = stream.getU8();
			break;
		}
		case BMFONT_BLOCK_TYPE_PAGES:
		{
			uint16_t pageCount = pBMFont->common.pages;
			for (uint16_t index = 0; index < pageCount; ++index)
			{
				const char* pageName = (const char*)stream.getPtr();
				uint8_t len = 0;
				while (pageName[len] != 0)
				{
					pBMFont->pages[index].name[len] = pageName[len];
                    len++;
				}
				pBMFont->pages[index].length = len;
				stream.offsetBy(len + 1);
			}
			break;
		}
		case BMFONT_BLOCK_TYPE_CHARS:
		{
			int32_t charCount = blockSize / 20;
			if (charCount > BMFONT_MAX_CHARS)
				return false;

			for (int32_t index = 0; index < charCount; ++index)
			{
				bmfont_char foo;
				pBMFont->chars.push_back(foo);
				pBMFont->chars[index].id = stream.getU32();
				pBMFont->chars[index].x = stream.getU16();
				pBMFont->chars[index].y = stream.getU16();
				pBMFont->chars[index].width = stream.getU16();
				pBMFont->chars[index].height = stream.getU16();
				pBMFont->chars[index].xoffset = stream.getS16();
				pBMFont->chars[index].yoffset = stream.getS16();
				pBMFont->chars[index].xadvance = stream.getS16();
				pBMFont->chars[index].page = stream.getU8();
				pBMFont->chars[index].chnl = stream.getU8();
			}

			OrderCharsByID(&pBMFont->chars);

			return true;
		}
		default:
			return false;
		}
	}
	return true;
}

std::vector<std::vector<bool>> BMFParser::BMPAsBoolArrays(std::string filePath) {

	std::vector<std::vector<bool>> result;

	const int infosize = 54;

	FILE* f = fopen(filePath.c_str(), "rb");

    if (f == nullptr) return result;

	unsigned char info[infosize];
	fread(info, sizeof(unsigned char), infosize, f); // read the 54-byte header

	std::vector<unsigned char> buf = { info[10], info[11], info[12], info[13] };
	uint32_t n = (buf[0]) | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
	int imageDataOffset = (int)n;

	buf = { info[18], info[19], info[20], info[21] };
	n = (buf[0]) | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
	int width = n;

	buf = { info[22], info[23], info[24], info[25] };
	n = (buf[0]) | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24);
	int height = n;
	int imageSize = width*height;

	result = std::vector < std::vector <bool>>(width, std::vector<bool>(height));

	fseek(f, imageDataOffset, 0);

	//unsigned char* tempdata = new unsigned char[imageSize / 8]; // allocate 1 byte per 8 pixels
	vector<unsigned char> tempdata(imageSize / 8);
	fread(&tempdata[0], sizeof(unsigned char), imageSize, f); // read the rest of the data at once
	fclose(f);

	int xcounter = 0;
	int ycounter = 0;

	int pixelcounter = 0;
	int bitcounter = 7;
	int charcounter = 0;

	// tot hier geen prob

	while (pixelcounter < imageSize) {
		if (bitcounter < 0) {
			bitcounter = 7;
			//++tempdata;
			charcounter++;
		}
		if (xcounter >= width) {
			xcounter = 0;
			ycounter++;
		}

		if ((tempdata.at(charcounter) >> bitcounter) & 1) result[xcounter][height - ycounter] = true;

		bitcounter--;
		xcounter++;
		pixelcounter++;
	}
	//tempdata -= charcounter;
	//free(tempdata); // this fucker crashes UE4!!!
	return result;
}

char* BMFParser::GetFileData(const char* pPath, size_t* pSize)
{
	char* pData = NULL;
	FILE* pFile = NULL;
#if defined(_MSC_VER)
	fopen_s(&pFile, pPath, "rb");
#else
	pFile = fopen(pPath, "rb");
#endif
	if (pFile == NULL)
	{
		*pSize = 0;
		return NULL;
	}
	fseek(pFile, 0, SEEK_END);
	*pSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	pData = (char*)malloc(*pSize);
	fread(pData, *pSize, 1, pFile);
	fclose(pFile);
	return pData;
}

bmfont BMFParser::getLoadedFont() {
	return loadedFont;
}

std::vector<std::vector<bool> > BMFParser::getAtlas() {
	return atlas;
}

BMFParser::~BMFParser() {

}
