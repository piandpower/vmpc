#include "lcdgui/LayeredScreen.hpp"

#include <Mpc.hpp>
#include <maingui/StartUp.hpp>
//#include <maingui/Constants.hpp>

#include "Field.hpp"
#include <lcdgui/Label.hpp>
//#include <lcdgui/Underline.hpp>
//#include <lcdgui/HorizontalBar.hpp>
//#include <lcdgui/VerticalBar.hpp>
//#include <lcdgui/MixerFaderBackground.hpp>
//#include <lcdgui/MixerKnobBackground.hpp>
//#include <lcdgui/Knob.hpp>
//#include <lcdgui/TwoDots.hpp>
//#include <lcdgui/Wave.hpp>
//#include <lcdgui/Popup.hpp>

//#include <lcdgui/SelectedEventBar.hpp>

//#include <ui/sequencer/SequencerObserver.hpp>

//#include <gui/BMFParser.hpp>

#include <rapidjson/filereadstream.h>

#include <cmath>

//using namespace mpc::ui;
using namespace rapidjson;
using namespace std;
using namespace mpc::lcdgui;

LayeredScreen::LayeredScreen(mpc::Mpc* mpc) 
//	: IPanelControl(pPlug, *Constants::LCD_RECT(), Constants::LCD_OFF())
{
	this->mpc = mpc;
	//popup = make_unique<mpc::lcdgui::Popup>();
	//popup->Hide(true);
	//horizontalBarsTempoChangeEditor = vector<shared_ptr<HorizontalBar>>(4);
	//horizontalBarsStepEditor = vector<shared_ptr<HorizontalBar>>(4);
	//selectedEventBarsStepEditor = vector<shared_ptr<mpc::lcdgui::SelectedEventBar>>(4);

	//verticalBarsMixer = vector<shared_ptr<VerticalBar>>(16);
	//mixerKnobBackgrounds = vector<shared_ptr<MixerKnobBackground>>(16);
	//mixerFaderBackgrounds = vector<shared_ptr<MixerFaderBackground>>(16);
	//knobs = vector<shared_ptr<Knob>>(16);

	//underline = make_shared<mpc::lcdgui::Underline>();

	//IRECT dotsRect(0, 0, 496, 50);

	//twoDots = make_shared<TwoDots>(dotsRect);

	int x, y, w, h;
	/*
	IRECT rect;
	for (int i = 0; i < 4; i++) {
		w = 160;
		h = 10;
		x = 382;
		y = 26 + (i * 18);
		rect = IRECT(x, y, x + w, y + h);
		horizontalBarsTempoChangeEditor[i] = make_shared<HorizontalBar>(50, rect, pPlug);
		horizontalBarsTempoChangeEditor[i]->Hide(true);

		x = 396;
		rect = IRECT(x, y, x + w, y + h);
		horizontalBarsStepEditor[i] = make_shared<HorizontalBar>(50, rect, pPlug);
		horizontalBarsStepEditor[i]->Hide(true);

		w = 496;
		h = 18;
		x = 0;
		y = 22 + (i * 18);
		rect = IRECT(x, y, x + w, y + h);
		selectedEventBarsStepEditor[i] = make_shared<mpc::lcdgui::SelectedEventBar>(rect, pPlug);
		selectedEventBarsStepEditor[i]->Hide(true);
	}

	for (int i = 0; i < 16; i++) {
		w = 10;
		h = 75;
		x = 24 + (i * 30);
		y = 33;
		rect = IRECT(x, y, x + w, y + h);
		verticalBarsMixer[i] = make_shared<VerticalBar>(rect, pPlug);
		verticalBarsMixer[i]->Hide(true);

		w = 28;
		h = 26;
		x = 8 + (i * 30);
		y = 0;
		rect = IRECT(x, y, x + w, y + h);
		mixerKnobBackgrounds[i] = make_shared<MixerKnobBackground>(rect, pPlug);
		mixerKnobBackgrounds[i]->Hide(true);

		h = 81;
		y = 30;
		rect = IRECT(x, y, x + w, y + h);
		mixerFaderBackgrounds[i] = make_shared<MixerFaderBackground>(rect, pPlug);
		mixerFaderBackgrounds[i]->Hide(true);

		w = 26;
		h = 26;
		x = 10 + (i * 30);
		y = 2;
		rect = IRECT(x, y, x + w, y + h);
		knobs[i] = make_shared<Knob>(rect, pPlug);
		knobs[i]->Hide(true);
	}

	fineWave = make_shared <mpc::lcdgui::Wave>(pPlug);
	fineWave->setFine(true);
	fineWave->Hide(true);

	wave = make_shared<mpc::lcdgui::Wave>(pPlug);
	wave->Hide(true);
	wave->setFine(false);
	*/

	FILE* fp0 = fopen(string(mpc::maingui::StartUp::resPath + "mainpanel.json").c_str(), "r"); // non-Windows use "r"
	FILE* fp1 = fopen(string(mpc::maingui::StartUp::resPath + "windowpanel.json").c_str(), "r"); // non-Windows use "r"
	FILE* fp2 = fopen(string(mpc::maingui::StartUp::resPath + "dialogpanel.json").c_str(), "r"); // non-Windows use "r"
	FILE* fp3 = fopen(string(mpc::maingui::StartUp::resPath + "dialog2panel.json").c_str(), "r"); // non-Windows use "r"

	FILE* fPointers[LAYER_COUNT]{ fp0, fp1, fp2, fp3 };

	for (int i = 0; i < LAYER_COUNT; i++) {
		char readBuffer[65536 * 2];
		layers[i] = make_unique<Layer>(this);
		FileReadStream is(fPointers[i], readBuffer, sizeof(readBuffer));
		layerJsons[i].ParseStream(is);
		fclose(fPointers[i]);
	}

	//gridBmp = GetGUI()->LoadIBitmap(GRID_ID, GRID_FN);
	//gridControl = make_shared<IBitmapControl>(GetGUI()->GetPlug(), Constants::LCD_RECT()->L, Constants::LCD_RECT()->T, 0, &gridBmp, IChannelBlend::kBlendNone);
}

int LayeredScreen::getCurrentParamIndex() {
	int currentIndex;
	Layer* l = layers[currentLayer].get();
	auto params = l->getParameters();
	int size = params.size();
	for (currentIndex = 0; currentIndex <= size; currentIndex++) {
		if (currentIndex == size) break;
		if (params[currentIndex]->getTf().lock()->getName().compare(l->getFocus()) == 0) {
			break;
		}
	}
	if (currentIndex == size) return -1;
	return currentIndex;
}

void LayeredScreen::attachControls() {
	/*
	layers[0]->attachBg();
	layers[0]->attachFb();
	for (int i = 0; i < 16; i++) {
		GetGUI()->AttachControl(mixerKnobBackgrounds[i].get());
		GetGUI()->AttachControl(knobs[i].get());
		GetGUI()->AttachControl(mixerFaderBackgrounds[i].get());
		GetGUI()->AttachControl(verticalBarsMixer[i].get());
	}
	for (int i = 0; i < 4; i++) {
		GetGUI()->AttachControl(horizontalBarsStepEditor[i].get());
		GetGUI()->AttachControl(selectedEventBarsStepEditor[i].get());
	}
	layers[0]->attachFields();
	layers[0]->attachLabels();

	for (int i = 1; i < 4; i++) {
		layers[i]->attachBg();
		layers[i]->attachFb();
		layers[i]->attachLabels();
		layers[i]->attachFields();
	}
	underline->Hide(true);
	GetGUI()->AttachControl(underline.get());
	for (int i = 0; i < 4; i++)
		GetGUI()->AttachControl(horizontalBarsTempoChangeEditor[i].get());

	GetGUI()->AttachControl(twoDots.get());
	GetGUI()->AttachControl(wave.get());
	GetGUI()->AttachControl(fineWave.get());
	GetGUI()->AttachControl(popup.get());
	GetGUI()->AttachControl(gridControl.get());
	*/
}

void LayeredScreen::transferFocus(bool backwards) {
	int currentIndex, candidateIndex;
	auto params = layers[currentLayer]->getParameters();
	int size = params.size();
	currentIndex = getCurrentParamIndex();
	if (currentIndex == -1) return;
	if (backwards && currentIndex == 0) return;
	if (!backwards && currentIndex == size - 1) return;

	bool success = false;
	/*
	while (success == false) {
		candidateIndex = backwards ? currentIndex-- - 1 : currentIndex++ + 1;
		if (candidateIndex >= 0 && candidateIndex < size) {
			if (!params[candidateIndex]->getTf().lock()->IsHidden()) {
				success = true;
			}
		}
		else {
			break;
		}
	}
	*/
	if (!success) return;

	layers[currentLayer]->setFocus(params[candidateIndex]->getTf().lock()->getName());
	//SetDirty(false);
}

int LayeredScreen::openScreen(string screenName) {
	//if (currentScreenName.compare(screenName) == 0) return -1;
	//When screenName equals currentScreenName, this can be used as a total refresh

	previousScreenName = currentScreenName;
	currentScreenName = screenName;
	firstField = "";
	int oldLayer = currentLayer;
	for (int i = 0; i < LAYER_COUNT; i++) {
		if (layerJsons[i].HasMember(screenName.c_str())) {
			currentLayer = i;
			if (oldLayer > currentLayer) {
				for (int j = oldLayer; j > currentLayer; j--) {
					layers[j]->clear();
				}
			}
			layers[i]->clear();
			firstField = layers[i]->openScreen(layerJsons[i][screenName.c_str()], screenName);
			break;
		}
	}

	returnToLastFocus();
	if (screenName.compare("sequencer") == 0) {
		//ui::sequencer::SequencerObserver foo(mpc, this);
	}
	return currentLayer;
}

Layer& LayeredScreen::getLayer(int i) {
	return *layers[i].get();
}

void LayeredScreen::createPopup(string text, int textXPos)
{
	//popup->Hide(false);
	//popup->setText(text, textXPos);
}

mpc::lcdgui::Background* LayeredScreen::getCurrentBackground()
{
	return layers[currentLayer]->getBackground();
}

void LayeredScreen::removeCurrentBackground()
{
	//    currentLayer->remove(currentBackground);
}

void LayeredScreen::setCurrentBackground(string s)
{
	getCurrentBackground()->setName(s);
}

void LayeredScreen::removePopup()
{
	//popup->Hide(true);
}

void LayeredScreen::setPopupText(string text)
{
	//popup->setText(text, 0);
}

void LayeredScreen::returnToLastFocus()
{
	auto focusCounter = 0;
	for (auto& lf : lastFocus) {
		if (lf[0].compare(currentScreenName) == 0) {
			focusCounter++;
			//mainFrame->setFocus(lf[1], currentLayer);
		}
	}
	if (focusCounter == 0) {
		vector<string> sa(2);
		sa[0] = currentScreenName;
		sa[1] = firstField;
		lastFocus.push_back(sa);
		//mainFrame->setFocus(firstField, currentLayer);
	}
}

void LayeredScreen::redrawEnvGraph(int attack, int decay)
{
	int line1[]{ 75, 43, 75 + (int)(attack * 0.17), 24 };
	int line2[]{ 119 - (int)(decay * 0.17), 24, 119, 43 };
	int line3[] = { 75 + (int)(attack * 0.17), 24, 119 - (int)(decay * 0.17), 24 };
	int* lines[] = { line1, line2, line3 };
	//envGraph->setCoordinates(lines);
}

void LayeredScreen::setLastFocus(string screenName, string tfName)
{
	for (auto& lf : lastFocus) {
		if (lf[0].compare(screenName) == 0) {
			lf[1] = tfName;
		}
	}
}

string LayeredScreen::getLastFocus(string screenName)
{
	string tfName = "";
	for (auto& lf : lastFocus) {
		if (lf[0].compare(screenName) == 0) {
			tfName = lf[1];
		}
	}
	return tfName;
}

void LayeredScreen::setCurrentScreenName(string screenName)
{
	currentScreenName = screenName;
}

string LayeredScreen::getCurrentScreenName()
{
	return currentScreenName;
}

void LayeredScreen::setPreviousScreenName(string screenName)
{
	previousScreenName = screenName;
}

string LayeredScreen::getPreviousScreenName()
{
	return previousScreenName;
}

mpc::lcdgui::Popup* LayeredScreen::getPopup() {
	//return popup.get();
	return nullptr;
}

mpc::lcdgui::EnvGraph* LayeredScreen::getEnvGraph()
{
	return envGraph.get();
}

string LayeredScreen::getPreviousFromNoteText()
{
	return previousFromNoteText;
}

void LayeredScreen::setPreviousFromNoteText(string text)
{
	previousFromNoteText = text;
}

void LayeredScreen::setPreviousViewModeText(string text)
{
	previousViewModeText = text;
}

string LayeredScreen::getPreviousViewModeText()
{
	return previousViewModeText;
}

vector<weak_ptr<mpc::lcdgui::HorizontalBar>> LayeredScreen::getHorizontalBarsTempoChangeEditor()
{
	auto res = vector<weak_ptr<mpc::lcdgui::HorizontalBar>>();
	for (auto& b : horizontalBarsTempoChangeEditor)
		res.push_back(b);
	return res;
}

vector<weak_ptr<mpc::lcdgui::HorizontalBar>> LayeredScreen::getHorizontalBarsStepEditor()
{
	auto res = vector<weak_ptr<mpc::lcdgui::HorizontalBar>>();
	for (auto& b : horizontalBarsStepEditor)
		res.push_back(b);
	return res;
}

vector<weak_ptr<mpc::lcdgui::VerticalBar>> LayeredScreen::getVerticalBarsMixer()
{
	auto res = vector<weak_ptr<mpc::lcdgui::VerticalBar>>();
	for (auto& b : verticalBarsMixer)
		res.push_back(b);
	return res;
}

vector<weak_ptr<mpc::lcdgui::SelectedEventBar>> LayeredScreen::getSelectedEventBarsStepEditor()
{
	auto res = vector<weak_ptr<mpc::lcdgui::SelectedEventBar>>();
	for (auto& b : selectedEventBarsStepEditor)
		res.push_back(b);
	return res;
}

FunctionKeys* LayeredScreen::getFunctionKeys()
{
	return layers[currentLayer]->getFunctionKeys();
}

vector<weak_ptr<mpc::lcdgui::Knob>> LayeredScreen::getKnobs()
{
	auto res = vector<weak_ptr<mpc::lcdgui::Knob>>();
	for (auto& b : knobs)
		res.push_back(b);
	return res;
}

vector<weak_ptr<mpc::lcdgui::MixerKnobBackground>> LayeredScreen::getMixerKnobBackgrounds()
{
	auto res = vector<weak_ptr<mpc::lcdgui::MixerKnobBackground>>();
	for (auto& b : mixerKnobBackgrounds)
		res.push_back(b);
	return res;
}

vector<weak_ptr<mpc::lcdgui::MixerFaderBackground>> LayeredScreen::getMixerFaderBackgrounds()
{
	auto res = vector<weak_ptr<mpc::lcdgui::MixerFaderBackground>>();
	for (auto& b : mixerFaderBackgrounds)
		res.push_back(b);
	return res;
}

void LayeredScreen::drawFunctionKeyses(string screenName)
{
	auto& screenJson = layerJsons[currentLayer][screenName.c_str()];
	Value& fblabels = screenJson["fblabels"];
	Value& fbtypes = screenJson["fbtypes"];
	getFunctionKeys()->initialize(fblabels, fbtypes);
}

mpc::lcdgui::TwoDots* LayeredScreen::getTwoDots()
{
	return twoDots.get();
}

Wave* LayeredScreen::getWave() {
	return wave.get();
}

Wave* LayeredScreen::getFineWave()
{
	return fineWave.get();
}

int LayeredScreen::getCurrentLayer() {
	return currentLayer;
}

weak_ptr<Field> LayeredScreen::findBelow(weak_ptr<Field> tf0) {
	int marginChars = 8;
	//int minDistV = maingui::Constants::TEXT_HEIGHT;
	//int maxDistH = maingui::Constants::TEXT_WIDTH * marginChars;
	weak_ptr<Field> result = tf0;
	auto lTf0 = tf0.lock();
	//for (auto& a : layers[currentLayer]->getAllFields()) {
	//	auto tf1 = dynamic_pointer_cast<Field>(a.lock());
	//	/*
	//	if (tf1->GetRECT()->B - lTf0->GetRECT()->B >= minDistV) {
	//		if (abs((int)(tf1->GetRECT()->MW() - lTf0->GetRECT()->MW())) <= maxDistH) {
	//			if (!tf1->IsHidden() && tf1->isFocusable()) {
	//				result = tf1;
	//				break;
	//			}
	//		}
	//	}
	//	*/
	//}

	//if (result.lock() == lTf0) {
	//	marginChars = 16;
	//	minDistV = Constants::TEXT_HEIGHT;
	//	maxDistH = Constants::TEXT_WIDTH * marginChars;
	//	for (auto& a : layers[currentLayer]->getAllFields()) {
	//		auto tf1 = dynamic_pointer_cast<Field>(a.lock());
	//		/*
	//		if (tf1->GetRECT()->B - lTf0->GetRECT()->B >= minDistV) {
	//			if (abs((int)(tf1->GetRECT()->MW() - lTf0->GetRECT()->MW())) <= maxDistH) {
	//				if (!tf1->IsHidden() && tf1->isFocusable()) {
	//					result = tf1;
	//					break;
	//				}
	//			}
	//		}
	//		*/
	//	}
	//}
	return result;
}

weak_ptr<Field> LayeredScreen::findAbove(weak_ptr<Field> tf0) {
	int marginChars = 8;
	//int minDistV = - maingui::Constants::TEXT_HEIGHT;
	//int maxDistH = maingui::Constants::TEXT_WIDTH * marginChars;
	weak_ptr<Field> result = tf0;
	auto lTf0 = tf0.lock();
	//auto revComponents = layers[currentLayer]->getAllFields();
	//reverse(revComponents.begin(), revComponents.end());
	//for (auto& a : revComponents) {
	//	auto tf1 = dynamic_pointer_cast<Field>(a.lock());
	//	/*
	//	if (tf1->GetRECT()->B - lTf0->GetRECT()->B <= minDistV) {
	//		if (abs((int)(tf1->GetRECT()->MW() - lTf0->GetRECT()->MW())) <= maxDistH) {
	//			if (!tf1->IsHidden() && tf1->isFocusable()) {
	//				result = tf1;
	//				break;
	//			}
	//		}
	//	}
	//	*/
	//}
	//if (result.lock() == lTf0) {
	//	marginChars = 16;
	//	minDistV = -Constants::TEXT_HEIGHT;
	//	maxDistH = Constants::TEXT_WIDTH * marginChars;
	//	for (auto& a : revComponents) {
	//		auto tf1 = dynamic_pointer_cast<Field>(a.lock());
	//		/*
	//		if (tf1->GetRECT()->B - lTf0->GetRECT()->B <= minDistV) {
	//			if (abs((int)(tf1->GetRECT()->MW() - lTf0->GetRECT()->MW())) <= maxDistH) {
	//				if (!tf1->IsHidden() && tf1->isFocusable()) {
	//					result = tf1;
	//					break;
	//				}
	//			}
	//		}
	//		*/
	//	}
	//}
	return result;
}

string LayeredScreen::findBelow(string tf0) {
	string result = tf0;
	//for (auto& a : layers[currentLayer]->getAllFields()) {
	//	auto candidate = dynamic_pointer_cast<Field>(a.lock());
	//	if (candidate->getName().compare(tf0) == 0) {
	//		result = findBelow(candidate).lock()->getName();
	//		break;
	//	}
	//}
	return result;
}

string LayeredScreen::findAbove(string tf0) {
	string result = tf0;
	//for (auto& a : layers[currentLayer]->getAllFields()) {
	//	auto candidate = dynamic_pointer_cast<Field>(a.lock());
	//	if (candidate->getName().compare(tf0) == 0) {
	//		result = findAbove(candidate).lock()->getName();
	//		break;
	//	}
	//}
	return result;
}

mpc::lcdgui::Underline* LayeredScreen::getUnderline() {
	return underline.get();
}

std::weak_ptr<Field> LayeredScreen::lookupField(std::string s)
{
	for (auto& a : getLayer(getCurrentLayer()).getAllFields() ) {
		auto candidate = dynamic_pointer_cast<Field>(a.lock());
		if (candidate->getName().compare(s) == 0) {
			return candidate;
		}
	}
	return weak_ptr<Field>();
}

std::weak_ptr<Label> LayeredScreen::lookupLabel(std::string s)
{
	for (auto& a : getLayer(getCurrentLayer()).getAllLabels()) {
		auto candidate = dynamic_pointer_cast<Label>(a.lock());
		if (candidate->getName().compare(s) == 0) {
			return candidate;
		}
	}
	return weak_ptr<Label>();
}

LayeredScreen::~LayeredScreen() {
	if (currentBackground != nullptr) {
		delete currentBackground;
	}
}
