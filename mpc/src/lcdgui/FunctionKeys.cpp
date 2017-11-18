#include "FunctionKeys.hpp"
//#include <maingui/Constants.hpp>

#include <maingui/StartUp.hpp>

using namespace mpc::lcdgui;
using namespace std;

FunctionKeys::FunctionKeys() 
	//: IPanelControl(pPlug, *Constants::FB_RECT(), Constants::LCD_OFF())
{
	enabled = vector<bool>{ false, false, false, false, false, false };
	names = vector<string>{ "", "", "", "", "", "" };
}

void FunctionKeys::initialize(rapidjson::Value& fbLabels, rapidjson::Value& fbTypes) {
	enabled = vector<bool>{ true, true, true, true, true , true };
	vector<int> fbTypesArray_ = vector<int>(fbTypes.Size());
	int count = fbTypes.Size();
	for (int i = 0; i < count; i++) {
		if (fbTypes[i].IsNull()) continue;
		fbTypesArray_[i] = fbTypes[i].GetInt();
	}
	box0 = fbTypesArray_[0];
	box1 = fbTypesArray_[1];
	box2 = fbTypesArray_[2];
	box3 = fbTypesArray_[3];
	box4 = fbTypesArray_[4];
	box5 = fbTypesArray_[5];
	auto fbLabelsArray_ = vector<string>(fbLabels.Size());
	count = fbLabels.Size();
	for (int i = 0; i < count; i++) {
		if (fbLabels[i].IsNull()) continue;
		fbLabelsArray_[i] = fbLabels[i].GetString();
	}
	auto counter = 0;
	for (auto b : enabled) {
		if (!b) {
			fbLabelsArray_[counter] = "";
		}
		counter++;
	}
	name0 = fbLabelsArray_[0];
	name1 = fbLabelsArray_[1];
	name2 = fbLabelsArray_[2];
	name3 = fbLabelsArray_[3];
	name4 = fbLabelsArray_[4];
	name5 = fbLabelsArray_[5];
	names = vector<string>{ name0, name1, name2, name3, name4, name5 };
	//SetDirty(false);
}

FunctionKeys::~FunctionKeys() {
	names.clear();
	enabled.clear();
}


void FunctionKeys::disable(int i)
{
	if (!enabled[i]) return;
	enabled[i] = false;
	//SetDirty(false);
}

void FunctionKeys::enable(int i)
{
    if(enabled[i]) return;
    enabled[i] = true;
	//SetDirty(false);
}

/*
bool FunctionKeys::Draw(IGraphics* ig) {
	vector<int> xPos = vector<int>{ 0, 82, 164, 246, 328, 410 };
	vector<int> types = vector<int>{ box0, box1, box2, box3, box4, box5 };
	for (int i = 0; i < xPos.size(); i++) {
		if (names[i] == "") continue;
		if (!enabled[i]) continue;

		auto stringSize = names[i].size();
		auto lengthInPixels = stringSize * 12;
		int offsetx = ((74 - lengthInPixels) / 2) + 3 + Constants::FB_RECT()->L;
		IColor* border = nullptr;
		IColor* bg = nullptr;
		IText* label = nullptr;
		if (types[i] == 0) {
			border = Constants::LCD_ON();
			bg = Constants::LCD_ON();
			label = Constants::FONT_OFF();
		}
		if (types[i] == 1) {
			border = Constants::LCD_ON();
			bg = Constants::LCD_OFF();
			label = Constants::FONT_ON();
		}
		if (types[i] == 2) {
			border = Constants::LCD_OFF();
			bg = Constants::LCD_OFF();
			label = Constants::FONT_ON();
		}
		IRECT tmp(xPos[i] + Constants::FB_RECT()->L, Constants::FB_RECT()->T, xPos[i] + 77 + Constants::FB_RECT()->L, 17 + Constants::FB_RECT()->T);
		ig->FillIRect(bg, &tmp);
        tmp = IRECT(xPos[i] + Constants::FB_RECT()->L+1, Constants::FB_RECT()->T+1, xPos[i] + 76 + Constants::FB_RECT()->L, 16 + Constants::FB_RECT()->T);
		ig->DrawRect(border, &tmp);
        tmp = IRECT(xPos[i] + Constants::FB_RECT()->L, Constants::FB_RECT()->T, xPos[i] + 77 + Constants::FB_RECT()->L, 17 + Constants::FB_RECT()->T);
		ig->DrawRect(border, &tmp);
        tmp = IRECT(xPos[i] + offsetx, Constants::FB_RECT()->T, xPos[i] + offsetx + 82, Constants::FB_RECT()->B);
		ig->DrawIText(label, &names[i][0], &tmp);
	}
	return true;
}
*/
