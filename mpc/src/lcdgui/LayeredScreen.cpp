#include "lcdgui/LayeredScreen.hpp"

#include <Logger.hpp>

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

#include <ui/sequencer/AssignObserver.hpp>
#include <ui/sequencer/BarCopyObserver.hpp>
#include <ui/sequencer/EditSequenceObserver.hpp>
#include <ui/sequencer/NextSeqObserver.hpp>
#include <ui/sequencer/NextSeqPadObserver.hpp>
#include <ui/sequencer/SequencerObserver.hpp>
#include <ui/sequencer/SongObserver.hpp>
//#include <ui/sequencer/StepEditorObserver.hpp>
#include <ui/sequencer/TrMoveObserver.hpp>
#include <ui/sequencer/TrMuteObserver.hpp>
#include <ui/sequencer/UserObserver.hpp>
#include <ui/sequencer/window/Assign16LevelsObserver.hpp>
#include <ui/sequencer/window/EraseObserver.hpp>
#include <ui/sequencer/window/MetronomeSoundObserver.hpp>
#include <ui/sequencer/window/MidiMonitorObserver.hpp>
#include <ui/sequencer/window/SequencerWindowObserver.hpp>
//#include <ui/sequencer/window/StepWindowObserver.hpp>

/*
#include <ui/sampler/DrumObserver.hpp>
#include <ui/sampler/LoopObserver.hpp>
#include <ui/sampler/MixerObserver.hpp>
#include <ui/sampler/PgmAssignObserver.hpp>
#include <ui/sampler/PgmParamsObserver.hpp>
#include <ui/sampler/PurgeObserver.hpp>
#include <ui/sampler/SampleObserver.hpp>
#include <ui/sampler/SndParamsObserver.hpp>
#include <ui/sampler/SoundObserver.hpp>
#include <ui/sampler/TrimObserver.hpp>
#include <ui/sampler/ZoneObserver.hpp>
#include <ui/sampler/window/EditSoundObserver.hpp>
#include <ui/sampler/window/MuteAssignObserver.hpp>
#include <ui/sampler/window/SamplerWindowObserver.hpp>
#include <ui/sampler/window/ZoomObserver.hpp>

#include <ui/NameObserver.hpp>
#include <ui/disk/DiskObserver.hpp>
#include <ui/disk/window/LoadASequenceFromAllObserver.hpp>
#include <ui/disk/window/DeleteAllFilesObserver.hpp>
#include <ui/disk/window/DirectoryObserver.hpp>
#include <ui/disk/window/SaveAllFileObserver.hpp>
#include <ui/midisync/SyncObserver.hpp>
#include <ui/misc/PunchObserver.hpp>
#include <ui/misc/SecondSeqObserver.hpp>
#include <ui/misc/TransObserver.hpp>
#include <ui/other/OthersObserver.hpp>

#include <ui/vmpc/AudioObserver.hpp>
#include <ui/vmpc/BufferSizeObserver.hpp>
#include <ui/vmpc/DirectToDiskRecorderObserver.hpp>
#include <ui/vmpc/MidiObserver.hpp>
#include <ui/vmpc/VmpcDiskObserver.hpp>
*/


#include <rapidjson/filereadstream.h>

#include <cmath>

//using namespace mpc::ui;
using namespace rapidjson;
using namespace std;
using namespace mpc::lcdgui;

//using namespace mpc::ui::vmpc;
using namespace mpc::ui::sequencer;
using namespace mpc::ui::sequencer::window;
using namespace mpc::ui::sampler;
//using namespace mpc::ui::sampler::window;
//using namespace mpc::ui::midisync;
//using namespace mpc::ui::misc;
//using namespace mpc::ui::other;
//using namespace mpc::ui::disk;
//using namespace mpc::ui::disk::window;

LayeredScreen::LayeredScreen(mpc::Mpc* mpc) 
//	: IPanelControl(pPlug, *Constants::LCD_RECT(), Constants::LCD_OFF())
{
	this->mpc = mpc;
	pixels = std::vector < std::vector <bool>>(248, std::vector<bool>(60));
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
	if (currentScreenName.compare(screenName) == 0) return -1;
	
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

	initObserver();
	return currentLayer;
}

std::vector<std::vector<bool>>* LayeredScreen::getPixels() {
	return &pixels;
}

void LayeredScreen::Draw() {
	if (getLayer(0).getBackground()->IsDirty()) getLayer(0).getBackground()->Draw(&pixels);
	auto components = getLayer(0).getAllLabels();
	for (auto& c : components) {
		if (c.lock()->IsDirty()) c.lock()->Draw(&pixels);
	}
	components = getLayer(0).getAllFields();
	for (auto& c : components) {
		if (c.lock()->IsDirty())
			c.lock()->Draw(&pixels);
	}
}

bool LayeredScreen::IsDirty() {
	if (getLayer(0).getBackground()->IsDirty()) return true;
	
	auto components = getLayer(0).getAllLabels();
	for (auto& c : components) {
		if (c.lock()->IsDirty()) return true;
	}

	components = getLayer(0).getAllFields();
	for (auto& c : components) {
		if (c.lock()->IsDirty()) return true;
	}
	return false;
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

void LayeredScreen::initObserver()
{
	//removeObservers();

	auto csn = currentScreenName;

	if (activeObserver) {
		activeObserver.reset();
	}
	/*
	if (csn.compare("audio") == 0) {
		activeObserver = make_unique<AudioObserver>(mpc, this);
	}
	else if (csn.compare("buffersize") == 0) {
		activeObserver = make_unique<BufferSizeObserver>(this);
	}
	else if (csn.compare("midi") == 0) {
		activeObserver = make_unique<MidiObserver>(mpc, this);
	}
	else if (csn.compare("directtodiskrecorder") == 0) {
		activeObserver = make_unique<DirectToDiskRecorderObserver>(this);
	}
	else if (csn.compare("disk") == 0) {
		activeObserver = make_unique<VmpcDiskObserver>(this);
	}
	else if (csn.compare("punch") == 0) {
		activeObserver = make_unique<PunchObserver>(this);
	}
	else if (csn.compare("trans") == 0) {
		activeObserver = make_unique<TransObserver>(this);
	}
	else if (csn.compare("2ndseq") == 0) {
		activeObserver = make_unique<SecondSeqObserver>(this);
	}
	else if (csn.compare("others") == 0) {
		activeObserver = make_unique<OthersObserver>(this);
	}
	else if (csn.compare("erase") == 0) {
		activeObserver = make_unique<EraseObserver>(this);
	}
	else if (csn.compare("sync") == 0) {
		activeObserver = make_unique<SyncObserver>(this);
	}
	else if (csn.compare("assign") == 0) {
		activeObserver = make_unique<AssignObserver>(this);
	}
	else if (csn.compare("assign16levels") == 0) {
		activeObserver = make_unique<Assign16LevelsObserver>(this);
	}
	else if (csn.compare("metronomesound") == 0) {
		activeObserver = make_unique<MetronomeSoundObserver>(this);
	}
	else if (csn.compare("saveallfile") == 0) {
		activeObserver = make_unique<SaveAllFileObserver>(this);
	}
	else if (csn.compare("loadasequencefromall") == 0) {
		activeObserver = make_unique<LoadASequenceFromAllObserver>(this);
	}
	else if (csn.compare("nextseqpad") == 0) {
		activeObserver = make_unique<NextSeqPadObserver>(mpc->getSequencer(), this);
	}
	else if (csn.compare("nextseq") == 0) {
		activeObserver = make_unique<NextSeqObserver>(this);
	}
	else if (csn.compare("song") == 0) {
		activeObserver = make_unique<SongObserver>(mpc, this);
	}
	else if (csn.compare("trackmute") == 0) {
		activeObserver = make_unique<TrMuteObserver>(mpc->getSequencer(), this);
	}
	else if (checkActiveScreen(Gui::diskNames)) {
		activeObserver = make_unique<DiskObserver>(mpc, gui);
	}
	else if (checkActiveScreen(Gui::seqWindowNames)) {
		activeObserver = make_unique<SequencerWindowObserver>(mpc, this);
	}
	else if (checkActiveScreen(Gui::soundNames)) {
		activeObserver = make_unique<SoundObserver>(mpc->getSampler(), this);
	}
	else if (csn.compare("sample") == 0) {
		activeObserver = make_unique<SampleObserver>(this, mpc->getSampler());
	}
	else 
	*/
	if (csn.compare("sequencer") == 0) {
		activeObserver = make_unique<SequencerObserver>(mpc);
	}
	/*
	else if (csn.compare("directory") == 0) {
		activeObserver = make_unique<DirectoryObserver>(mpc->getDisk(), gui);
	}
	else if (csn.compare("programparams") == 0) {
		activeObserver = make_unique<PgmParamsObserver>(mpc, this);
	}
	else if (csn.compare("programassign") == 0) {
		activeObserver = make_unique<PgmAssignObserver>(mpc, this);
	}
	else if (csn.compare("sequencer_step") == 0) {
		activeObserver = make_unique<StepEditorObserver>(mpc, gui);
	}
	else if (csn.compare("step_tc") == 0 || csn.compare("editmultiple") == 0 || csn.compare("insertevent") == 0) {
		activeObserver = make_unique<StepWindowObserver>(mpc, this);
	}
	else if (csn.compare("mixer") == 0 || csn.compare("channelsettings") == 0 || csn.compare("mixersetup") == 0) {
		activeObserver = make_unique<MixerObserver>(mpc, gui);
	}
	else if (csn.compare("edit") == 0) {
		activeObserver = make_unique<EditSequenceObserver>(mpc->getSequencer(), mpc->getSampler(), this);
	}
	else if (csn.compare("name") == 0) {
		activeObserver = make_unique<ui::NameObserver>(mpc, this);
	}
	else if (csn.compare("midiinputmonitor") == 0 || csn.compare("midioutputmonitor") == 0) {
		activeObserver = make_unique<MidiMonitorObserver>(mpc, this);
	}
	else if (csn.compare("barcopy") == 0) {
		activeObserver = make_unique<BarCopyObserver>(this);
	}
	else if (csn.compare("trmove") == 0) {
		activeObserver = make_unique<TrMoveObserver>(mpc->getSequencer(), this);
	}
	else if (csn.compare("user") == 0) {
		activeObserver = make_unique<UserObserver>(mpc, this);
	}
	else if (csn.compare("trim") == 0) {
		activeObserver = make_unique<TrimObserver>(mpc->getSampler(), this);
	}
	else if (csn.compare("loop") == 0) {
		activeObserver = make_unique<LoopObserver>(mpc->getSampler(), this);
	}
	else if (csn.compare("editsound") == 0) {
		activeObserver = make_unique<EditSoundObserver>(mpc->getSampler(), this);
	}
	else if (csn.find("startfine") != string::npos || csn.find("endfine") != string::npos || csn.compare("looptofine") == 0) {
		activeObserver = make_unique<ZoomObserver>(mpc->getSampler(), this);
	}
	else if (csn.compare("zone") == 0 || csn.compare("numberofzones") == 0) {
		activeObserver = make_unique<ZoneObserver>(mpc->getSampler(), this);
	}
	else if (csn.compare("params") == 0) {
		activeObserver = make_unique<SndParamsObserver>(mpc->getSampler(), this);
	}
	else if (csn.compare("deleteallfiles") == 0) {
		activeObserver = make_unique<DeleteAllFilesObserver>(mpc, gui);
	}
	else if (checkActiveScreen(Gui::samplerWindowNames)) {
		activeObserver = make_unique<SamplerWindowObserver>(mpc, this);
	}
	else if (csn.compare("purge") == 0) {
		activeObserver = make_unique<PurgeObserver>(gui);
	}
	else if (csn.compare("drum") == 0) {
		activeObserver = make_unique<DrumObserver>(gui);
	}
	else if (csn.compare("muteassign") == 0) {
		activeObserver = make_unique<MuteAssignObserver>(this);
	}
	*/
}


LayeredScreen::~LayeredScreen() {
	if (currentBackground != nullptr) {
		delete currentBackground;
	}
}
