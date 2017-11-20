#include "Layer.hpp"
#include "Field.hpp"
#include "Label.hpp"
//#include <maingui/Constants.hpp>

#include <string>

#include <Logger.hpp>

using namespace rapidjson;
//using namespace mpc::gui;
using namespace mpc::lcdgui;
using namespace std;

Layer::Layer(mpc::lcdgui::LayeredScreen* layeredScreen)
	//: IPanelControl(pPlug, *Constants::LCD_RECT(), Constants::LCD_OFF())
{
	bg = new Background();
	fb = new FunctionKeys();
	for (int i = 0; i < 100; i++) {
		unusedFields.push_back(make_shared<mpc::lcdgui::Field>(layeredScreen));
	}
	for (int i = 0; i < 100; i++) {
		unusedLabels.push_back(make_shared<mpc::lcdgui::Label>());
	}
	//blinkLabel = make_shared<mpc::lcdgui::BlinkLabel>("SOLO");
	//blinkLabel->initialize("soloblink", "SOLO", 133, 51, 4);
	//blinkLabel->Hide(true);
}

weak_ptr<mpc::lcdgui::Field> Layer::getUnusedField() {
	auto res = unusedFields[0];
	unusedFields.erase(unusedFields.begin());
	usedFields.push_back(res);
	//res->Hide(false);
	return res;
}

void Layer::unuseField(weak_ptr<mpc::lcdgui::Field> field) {
	auto lField = field.lock();
	//lField->Hide(true);
	lField->setFocusable(false);
		for (int i = 0; i < usedFields.size(); i++) {
		if (usedFields[i] == lField) {
			unusedFields.push_back(lField);
			usedFields.erase(usedFields.begin() + i);
			break;
		}
	}
}

weak_ptr<mpc::lcdgui::Label> Layer::getUnusedLabel() {
	auto res = unusedLabels[0];
	unusedLabels.erase(unusedLabels.begin());
	usedLabels.push_back(res);
	//res->Hide(false);
	return res;
}

void Layer::unuseLabel(weak_ptr<mpc::lcdgui::Label> label) {
	auto lLabel = label.lock();
	//lLabel->Hide(true);
	for (int i = 0; i < usedLabels.size(); i++) {
		if (usedLabels[i] == lLabel) {
			unusedLabels.push_back(lLabel);
			usedLabels.erase(usedLabels.begin() + i);
		}
	}
}

vector<mpc::lcdgui::Parameter*> Layer::getParameters() {
	auto res = vector<mpc::lcdgui::Parameter*>();
	for (auto& p : params)
		res.push_back(p.get());
	return res;
}

vector<mpc::lcdgui::Info*> Layer::getInfos() {
	auto res = vector<mpc::lcdgui::Info*>();
	for (auto& p : infos)
		res.push_back(p.get());
	return res;
}

mpc::lcdgui::Background* Layer::getBackground() {
	return bg;
}

mpc::lcdgui::FunctionKeys* Layer::getFunctionKeys() {
	return fb;
}

void Layer::setFocus(string textFieldName) {

	string oldFocus = focus;
	// First make sure the desired focus field exists and is focusable
	bool exists = false;
	for (auto& a : getAllFields()) {
		auto tf = dynamic_pointer_cast<Field>(a.lock());
		//if (!tf->IsHidden() && tf->getName().compare(textFieldName) == 0 && tf->isFocusable()) {
		if (tf->getName().compare(textFieldName) == 0 && tf->isFocusable()) {
			exists = true;
			break;
		}
	}
	if (!exists) return;
	for (auto& a : getAllFields()) {
		auto tf = dynamic_pointer_cast<Field>(a.lock());
		if (tf->getName().compare(focus) == 0 && tf->isFocusable()) {
			tf->loseFocus(textFieldName);
			break;
		}
	}
	focus = textFieldName;
	for (auto& a : getAllFields()) {
		auto tf = dynamic_pointer_cast<Field>(a.lock());
		if (tf->getName().compare(textFieldName) == 0 && tf->isFocusable()) {
			tf->takeFocus(oldFocus);
			break;
		}
	}
}

string Layer::getFocus() {
	return focus;
}

string Layer::openScreen(Value& screenJson, string screenName) {
	moduru::Logger::l.log("Opening screen " + screenName + "\n");
	focus = "";
	//bg->Hide(false);
	bg->setName(screenName);
	Value& labels = screenJson["labels"];
	Value& x = screenJson["x"];
	Value& y = screenJson["y"];
	Value& parameters = screenJson["parameters"];
	Value& tfsize = screenJson["tfsize"];

	if (screenName.compare("sequencer") == 0) {
		//blinkLabel->setBlinking(false);
		//blinkLabel->Hide(false);
	}
	else {
		//if (!blinkLabel->IsHidden()) blinkLabel->Hide(true);
	}

	string firstField = "";
	for (int i = 0; i < labels.Size(); i++) {
		if (i == 0) {
			firstField = parameters[i].GetString();
		}
		params.push_back(make_unique<lcdgui::Parameter>(getUnusedField(), getUnusedLabel(), labels[i].GetString()
			, parameters[i].GetString()
			, x[i].GetInt()
			, y[i].GetInt()
			, tfsize[i].GetInt()
			));
	}

	if (screenJson.HasMember("infowidgets")) {
		Value& infoNames = screenJson["infowidgets"];
		Value& infoSize = screenJson["infosize"];
		Value& infoX = screenJson["infox"];
		Value& infoY = screenJson["infoy"];
		for (int i = 0; i < infoNames.Size(); i++) {
			infos.push_back(make_unique<lcdgui::Info>(getUnusedLabel(), infoNames[i].GetString()
				, infoX[i].GetInt()
				, infoY[i].GetInt()
				, infoSize[i].GetInt()));
		}
	}

	if (screenJson.HasMember("fblabels")) {
		Value& fblabels = screenJson["fblabels"];
		Value& fbtypes = screenJson["fbtypes"];
		//fb->Hide(false);
		fb->initialize(fblabels, fbtypes);
	}
	else {
		//fb->Hide(true);
	}
	return firstField;
}

void Layer::clear() {
	for (auto& p : params) {
		unuseField(p->getTf());
		unuseLabel(p->getLabel());
	}
	params.clear();
	for (auto& i : infos) {
		unuseLabel(i->getLabel());
	}
	infos.clear();
	//bg->Hide(true);
	//fb->Hide(true);
	//SetDirty(false);
}

vector<weak_ptr<mpc::lcdgui::Component>> Layer::getAllFields() {
	vector<weak_ptr<mpc::lcdgui::Component>> result;
	for (auto& f : usedFields)
		result.push_back(f);
	return result;
}

vector<weak_ptr<mpc::lcdgui::Component>> Layer::getAllLabels() {
	vector<weak_ptr<mpc::lcdgui::Component>> result;
	for (auto& l : usedLabels)
		result.push_back(l);
	//if (blinkLabel) result.push_back(blinkLabel);
	return result;
}


Layer::~Layer() {
	if (bg != nullptr) delete bg;
	if (fb != nullptr) delete fb;
}
