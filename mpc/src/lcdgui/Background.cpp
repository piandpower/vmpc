#include "Background.hpp"
//#include <maingui/Constants.hpp>
#include <maingui/StartUp.hpp>
//////#include <maingui/Gui.hpp>
#include <iostream>
//#include <IGraphics.h>

using namespace mpc::lcdgui;
using namespace std;

Background::Background() 
//	: IPanelControl(pPlug, *Constants::LCD_RECT(), Constants::LCD_OFF())
{
}

void Background::setName(std::string name)
{ 
	this->name = name;
	SetDirty();
}

void Background::Draw(std::vector< std::vector<bool> >* pixels)
{
	//if (IsHidden()) return true;
	//int xOffset = Constants::LCD_RECT()->L;
	//int yOffset = Constants::LCD_RECT()->T;

	string bgNameStr = mpc::maingui::StartUp::resPath + "bmp/" + name + ".bmp";
	const int infosize = 54;

	FILE* f = fopen(bgNameStr.c_str(), "rb");
	//if (f == nullptr) return true;
	unsigned char info[infosize];
	fread(info, sizeof(unsigned char), infosize, f); // read the 54-byte header
	int imageDataOffset = info[10];
	int width = info[18];
	int height = 256 - info[22];
	int imageSize = width*height;
	fseek(f, imageDataOffset, 0);

	//unsigned char* data = new unsigned char[imageSize]; // allocate 1 byte per pixel
	vector<unsigned char> data(imageSize);
	fread(&data[0], sizeof(unsigned char), imageSize, f); // read the rest of the data at once
	fclose(f);
	int colorCount = (imageDataOffset - infosize) / 4;

	//for (int y = 0; y < height * 2; y += 2) {
	//	for (int x = 0; x < width * 2; x += 2) {
	//		unsigned char value = *data;
 //           IRECT tmp(x + xOffset, y + yOffset, x + 1 + xOffset, y + 1 + yOffset);
	//		if ((colorCount <= 2 && value == 1) || (colorCount > 2 && value == 2)) {
	//			pGraphics->DrawRect(Constants::LCD_ON(), &tmp);
	//		}
	//		else if (value == 0) {
	//			pGraphics->DrawRect(Constants::LCD_OFF(), &tmp);
	//		}
	//		++data;
	//	}
	//}
	int charcounter = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			//unsigned char value = *data;
			auto value = data[charcounter++];
			if ((colorCount <= 2 && value == 1) || (colorCount > 2 && value == 2)) {
				(*pixels)[x][y] = true;
			}
			else if (value == 0) {
				(*pixels)[x][y] = false;
			}
			//++data;
		}
	}

	//data -= imageSize;
	//free(data);

	// clear bottom in case height != full LCD height
	if (name.compare("popup") != 0) {
		for (int y = 0; y < (60 - height); y++) {
			for (int x = 0; x < width; x++) {
			//	(*pixels)[x][y] = false;
			}
		}
	}
	//return true;
	dirty = false;
}

Background::~Background() {
}
