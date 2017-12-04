#include "lcdgui/LayeredScreen.hpp"

#include <Logger.hpp>

#include <Mpc.hpp>
#include <StartUp.hpp>

#include "Field.hpp"

#include <lcdgui/EnvGraph.hpp>
#include <lcdgui/Label.hpp>
#include <lcdgui/Underline.hpp>
#include <lcdgui/HorizontalBar.hpp>
#include <lcdgui/VerticalBar.hpp>
#include <lcdgui/MixerFaderBackground.hpp>
#include <lcdgui/MixerKnobBackground.hpp>
#include <lcdgui/Knob.hpp>
#include <lcdgui/TwoDots.hpp>
#include <lcdgui/Wave.hpp>
#include <lcdgui/Popup.hpp>
#include <lcdgui/SelectedEventBar.hpp>

#include <ui/sequencer/AssignObserver.hpp>
#include <ui/sequencer/BarCopyObserver.hpp>
#include <ui/sequencer/EditSequenceObserver.hpp>
#include <ui/sequencer/NextSeqObserver.hpp>
#include <ui/sequencer/NextSeqPadObserver.hpp>
#include <ui/sequencer/SequencerObserver.hpp>
#include <ui/sequencer/SongObserver.hpp>
#include <ui/sequencer/StepEditorObserver.hpp>
#include <ui/sequencer/TrMoveObserver.hpp>
#include <ui/sequencer/TrMuteObserver.hpp>
#include <ui/sequencer/UserObserver.hpp>
#include <ui/sequencer/window/Assign16LevelsObserver.hpp>
#include <ui/sequencer/window/EraseObserver.hpp>
#include <ui/sequencer/window/MetronomeSoundObserver.hpp>
#include <ui/sequencer/window/MidiMonitorObserver.hpp>
#include <ui/sequencer/window/SequencerWindowObserver.hpp>
#include <ui/sequencer/window/StepWindowObserver.hpp>

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
/*
#include <ui/misc/PunchObserver.hpp>
#include <ui/misc/SecondSeqObserver.hpp>
#include <ui/misc/TransObserver.hpp>
#include <ui/other/OthersObserver.hpp>
*/

#include <ui/vmpc/AudioObserver.hpp>
#include <ui/vmpc/BufferSizeObserver.hpp>
#include <ui/vmpc/DirectToDiskRecorderObserver.hpp>
//#include <ui/vmpc/MidiObserver.hpp>
#include <ui/vmpc/VmpcDiskObserver.hpp>


#include <rapidjson/filereadstream.h>

#include <cmath>

using namespace std;

using namespace rapidjson;

using namespace mpc::ui;
using namespace mpc::lcdgui;

using namespace mpc::ui::vmpc;
using namespace mpc::ui::sequencer;
using namespace mpc::ui::sequencer::window;
using namespace mpc::ui::sampler;
using namespace mpc::ui::sampler::window;
using namespace mpc::ui::midisync;
using namespace mpc::ui::misc;
using namespace mpc::ui::other;
using namespace mpc::ui::disk;
using namespace mpc::ui::disk::window;

static vector<string> samplerWindowNames = vector<string>{ "program", "deleteprogram", "deleteallprograms", "createnewprogram",
"copyprogram", "assignmentview", "initpadassign", "copynoteparameters", "velocitymodulation",
"veloenvfilter", "velopitch", "autochromaticassignment", "keeporretry" };

static vector<string> seqWindowNames = vector<string>{ "copysequence", "copytrack", "countmetronome", "timedisplay",
"tempochange", "timingcorrect", "changetsig", "changebars", "changebars2", "eraseallofftracks",
"transmitprogramchanges", "multirecordingsetup", "midiinput", "midioutput", "editvelocity", "sequence",
"deletesequence", "track", "deletetrack", "deleteallsequences", "deletealltracks", "loopbarswindow" };

static vector<string> diskNames = vector<string>{ "load", "save", "format", "setup", "device", "deleteallfiles", "loadaprogram",
"saveaprogram", "loadasound", "saveasound", "cantfindfile", "filealreadyexists", "loadasequence",
"saveasequence", "saveapsfile" };

static vector<string> soundNames = vector<string>{ "sound", "deletesound", "deleteallsound", "convertsound", "resample",
"stereotomono", "monotostereo", "copysound" };

static vector<string> soundGuiNames = vector<string>{ "trim", "loop", "zone" };

LayeredScreen::LayeredScreen(mpc::Mpc* mpc) 
{
	this->mpc = mpc;
	pixels = std::vector < std::vector <bool>>(248, std::vector<bool>(60));
	popup = make_unique<mpc::lcdgui::Popup>();
	popup->Hide(true);
	horizontalBarsTempoChangeEditor = vector<shared_ptr<HorizontalBar>>(4);
	horizontalBarsStepEditor = vector<shared_ptr<HorizontalBar>>(4);
	selectedEventBarsStepEditor = vector<shared_ptr<mpc::lcdgui::SelectedEventBar>>(4);

	verticalBarsMixer = vector<shared_ptr<VerticalBar>>(16);
	mixerKnobBackgrounds = vector<shared_ptr<MixerKnobBackground>>(16);
	mixerFaderBackgrounds = vector<shared_ptr<MixerFaderBackground>>(16);
	knobs = vector<shared_ptr<Knob>>(16);

	underline = make_shared<mpc::lcdgui::Underline>();
	envGraph = make_shared<mpc::lcdgui::EnvGraph>();

	//IRECT dotsRect(0, 0, 496, 50);

	twoDots = make_shared<TwoDots>();

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
	*/


	fineWave = make_shared <mpc::lcdgui::Wave>();
	fineWave->setFine(true);
	fineWave->Hide(true);

	wave = make_shared<mpc::lcdgui::Wave>();
	wave->Hide(true);
	wave->setFine(false);

	FILE* fp0 = fopen(string(mpc::StartUp::resPath + "mainpanel.json").c_str(), "r"); // non-Windows use "r"
	FILE* fp1 = fopen(string(mpc::StartUp::resPath + "windowpanel.json").c_str(), "r"); // non-Windows use "r"
	FILE* fp2 = fopen(string(mpc::StartUp::resPath + "dialogpanel.json").c_str(), "r"); // non-Windows use "r"
	FILE* fp3 = fopen(string(mpc::StartUp::resPath + "dialog2panel.json").c_str(), "r"); // non-Windows use "r"

	FILE* fPointers[LAYER_COUNT]{ fp0, fp1, fp2, fp3 };

	for (int i = 0; i < LAYER_COUNT; i++) {
		char readBuffer[65536 * 2];
		layers[i] = make_unique<Layer>(this);
		FileReadStream is(fPointers[i], readBuffer, sizeof(readBuffer));
		layerJsons[i].ParseStream(is);
		fclose(fPointers[i]);
	}
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

void LayeredScreen::transferFocus(bool backwards) {
	int currentIndex, candidateIndex;
	auto params = layers[currentLayer]->getParameters();
	int size = params.size();
	currentIndex = getCurrentParamIndex();
	if (currentIndex == -1) return;
	if (backwards && currentIndex == 0) return;
	if (!backwards && currentIndex == size - 1) return;

	bool success = false;
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
	if (!success) return;

	layers[currentLayer]->setFocus(params[candidateIndex]->getTf().lock()->getName());
}

int LayeredScreen::openScreen(string screenName) {
	if (currentScreenName.compare(screenName) == 0) return -1;
	
	setLastFocus(currentScreenName, getFocus());

	previousScreenName = currentScreenName;
	currentScreenName = screenName;
	std::string firstField = "";
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

	returnToLastFocus(firstField);

	initObserver();
	return currentLayer;
}

std::vector<std::vector<bool> >* LayeredScreen::getPixels() {
	return &pixels;
}

void LayeredScreen::Draw() {
	for (int i = currentLayer; i <= currentLayer; i++) {
		if (layers[i]->getBackground()->IsDirty()) layers[i]->getBackground()->Draw(&pixels);
		auto components = layers[i]->getAllLabels();
		for (auto& c : components) {
			if (c.lock()->IsDirty()) c.lock()->Draw(&pixels);
		}
		components = layers[i]->getAllFields();
		for (auto& c : components) {
			if (c.lock()->IsDirty())
				c.lock()->Draw(&pixels);
		}
		if (layers[i]->getFunctionKeys()->IsDirty()) layers[i]->getFunctionKeys()->Draw(&pixels);
		if (i == currentLayer && currentScreenName.compare("name") == 0 && underline->IsDirty()) underline->Draw(&pixels);
	}
	if (popup->IsDirty()) popup->Draw(&pixels);
	if (envGraph->IsDirty()) envGraph->Draw(&pixels);
}

bool LayeredScreen::IsDirty() {
	for (int i = currentLayer; i <= currentLayer; i++) {
		if (layers[i]->getBackground()->IsDirty() || layers[i]->getFunctionKeys()->IsDirty()) return true;

		auto components = layers[i]->getAllLabels();
		for (auto& c : components) {
			if (c.lock()->IsDirty()) return true;
		}

		components = layers[i]->getAllFields();
		for (auto& c : components) {
			if (c.lock()->IsDirty()) return true;
		}
		if (layers[i]->getFunctionKeys()->IsDirty()) return true;
		if (underline->IsDirty()) return true;
	}
	if (popup->IsDirty()) return true;
	if (envGraph->IsDirty()) return true;
	return false;
}

Layer* LayeredScreen::getLayer(int i) {
	return layers[i].get();
}

void LayeredScreen::createPopup(string text, int textXPos)
{
	popup->Hide(false);
	popup->setText(text, textXPos);
}

mpc::lcdgui::Background* LayeredScreen::getCurrentBackground()
{
	return layers[currentLayer]->getBackground();
}

void LayeredScreen::removeCurrentBackground()
{
	layers[currentLayer]->getBackground()->setName("");
}

void LayeredScreen::setCurrentBackground(string s)
{
	getCurrentBackground()->setName(s);
}

void LayeredScreen::removePopup()
{
	popup->Hide(true);
}

void LayeredScreen::setPopupText(string text)
{
	popup->setText(text, 0);
}

void LayeredScreen::returnToLastFocus(string firstFieldOfThisScreen)
{
	auto focusCounter = 0;
	for (auto& lf : lastFocus) {
		if (lf[0].compare(currentScreenName) == 0) {
			focusCounter++;
			setFocus(lf[1]);
		}
	}
	if (focusCounter == 0) {
		vector<string> sa(2);
		sa[0] = currentScreenName;
		sa[1] = firstFieldOfThisScreen;
		lastFocus.push_back(sa);
		setFocus(firstFieldOfThisScreen);
	}
}

void LayeredScreen::redrawEnvGraph(int attack, int decay)
{
	vector<int> line1 { 75, 43, 75 + (int)(attack * 0.17), 24 };
	vector<int> line2 { 119 - (int)(decay * 0.17), 24, 119, 43 };
	vector<int> line3  { 75 + (int)(attack * 0.17), 24, 119 - (int)(decay * 0.17), 24 };
	vector<vector<int> > lines = { line1, line2, line3 };
	envGraph->setCoordinates(lines);
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
	return popup.get();
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
	int minDistV = 7;
	int maxDistH = 6 * marginChars;
	weak_ptr<Field> result = tf0;
	auto lTf0 = tf0.lock();
	for (auto& a : layers[currentLayer]->getAllFields()) {
		auto tf1 = dynamic_pointer_cast<Field>(a.lock());
		auto B1 = tf1->getY() + tf1->getH();
		auto B0 = lTf0->getY() + lTf0->getH();
		auto MW1 = 0.5f * (float)(tf1->getX()*2 + tf1->getW());
		auto MW0 = 0.5f * (float)(lTf0->getX() * 2 + lTf0->getW());
		if (B1 - B0 >= minDistV) {
			if (abs((int)(MW1 - MW0)) <= maxDistH) {
				if (!tf1->IsHidden() && tf1->isFocusable()) {
					result = tf1;
					break;
				}
			}
		}
	}

	if (result.lock() == lTf0) {
		marginChars = 16;
		maxDistH = 6 * marginChars;
		for (auto& a : layers[currentLayer]->getAllFields()) {
			auto tf1 = dynamic_pointer_cast<Field>(a.lock());
			auto B1 = tf1->getY() + tf1->getH();
			auto B0 = lTf0->getY() + lTf0->getH();
			auto MW1 = 0.5f * (float)(tf1->getX() * 2 + tf1->getW());
			auto MW0 = 0.5f * (float)(lTf0->getX() * 2 + lTf0->getW());
			if (B1 - B0 >= minDistV) {
				if (abs((int)(MW1 - MW0)) <= maxDistH) {
					if (!tf1->IsHidden() && tf1->isFocusable()) {
						result = tf1;
						break;
					}
				}
			}
		}
	}
	return result;
}

weak_ptr<Field> LayeredScreen::findAbove(weak_ptr<Field> tf0) {
	int marginChars = 8;
	int minDistV = - 7;
	int maxDistH = 6 * marginChars;
	weak_ptr<Field> result = tf0;
	auto lTf0 = tf0.lock();
	auto revComponents = layers[currentLayer]->getAllFields();
	reverse(revComponents.begin(), revComponents.end());
	for (auto& a : revComponents) {
		auto tf1 = dynamic_pointer_cast<Field>(a.lock());
		auto B1 = tf1->getY() + tf1->getH();
		auto B0 = lTf0->getY() + lTf0->getH();
		auto MW1 = 0.5f * (float)(tf1->getX() * 2 + tf1->getW());
		auto MW0 = 0.5f * (float)(lTf0->getX() * 2 + lTf0->getW());
		if (B1 - B0 <= minDistV) {
			if (abs((int)(MW1 - MW0)) <= maxDistH) {
				if (!tf1->IsHidden() && tf1->isFocusable()) {
					result = tf1;
					break;
				}
			}
		}
	}
	if (result.lock() == lTf0) {
		marginChars = 16;
		maxDistH = 6 * marginChars;
		for (auto& a : revComponents) {
			auto tf1 = dynamic_pointer_cast<Field>(a.lock());
			auto B1 = tf1->getY() + tf1->getH();
			auto B0 = lTf0->getY() + lTf0->getH();
			auto MW1 = 0.5f * (float)(tf1->getX() * 2 + tf1->getW());
			auto MW0 = 0.5f * (float)(lTf0->getX() * 2 + lTf0->getW());
			if (B1 - B0 <= minDistV) {
				if (abs((int)(MW1 - MW0)) <= maxDistH) {
					if (!tf1->IsHidden() && tf1->isFocusable()) {
						result = tf1;
						break;
					}
				}
			}
		}
	}
	return result;
}

string LayeredScreen::findBelow(string tf0) {
	string result = tf0;
	for (auto& a : layers[currentLayer]->getAllFields()) {
		auto candidate = dynamic_pointer_cast<Field>(a.lock());
		if (candidate->getName().compare(tf0) == 0) {
			result = findBelow(candidate).lock()->getName();
			break;
		}
	}
	return result;
}

string LayeredScreen::findAbove(string tf0) {
	string result = tf0;
	for (auto& a : layers[currentLayer]->getAllFields()) {
		auto candidate = dynamic_pointer_cast<Field>(a.lock());
		if (candidate->getName().compare(tf0) == 0) {
			result = findAbove(candidate).lock()->getName();
			break;
		}
	}
	return result;
}

mpc::lcdgui::Underline* LayeredScreen::getUnderline() {
	return underline.get();
}

std::weak_ptr<Field> LayeredScreen::lookupField(std::string s)
{
	for (auto& a : layers[currentLayer]->getAllFields() ) {
		auto candidate = dynamic_pointer_cast<Field>(a.lock());
		if (candidate->getName().compare(s) == 0) {
			return candidate;
		}
	}
	return weak_ptr<Field>();
}

std::weak_ptr<Label> LayeredScreen::lookupLabel(std::string s)
{
	for (auto& a : layers[currentLayer]->getAllLabels()) {
		auto candidate = dynamic_pointer_cast<Label>(a.lock());
		if (candidate->getName().compare(s) == 0) {
			return candidate;
		}
	}
	return weak_ptr<Label>();
}

static inline bool checkActiveScreen(vector<string>* sa, string csn)
{
	for (auto s : *sa) {
		if (csn.compare(s) == 0)
			return true;
	}
	return false;
}

void LayeredScreen::initObserver()
{
	//removeObservers();

	auto csn = currentScreenName;

	if (activeObserver) {
		activeObserver.reset();
	}
	if (csn.compare("audio") == 0) {
		activeObserver = make_unique<AudioObserver>(mpc);
	}
	else if (csn.compare("buffersize") == 0) {
		activeObserver = make_unique<BufferSizeObserver>(mpc);
	}
	else if (csn.compare("midi") == 0) {
		//activeObserver = make_unique<MidiObserver>(mpc, this);
	}
	else if (csn.compare("directtodiskrecorder") == 0) {
		activeObserver = make_unique<DirectToDiskRecorderObserver>(mpc);
	}
	else if (csn.compare("disk") == 0) {
		activeObserver = make_unique<VmpcDiskObserver>(mpc);
	}
	else if (csn.compare("punch") == 0) {
		//activeObserver = make_unique<PunchObserver>(this);
	}
	else if (csn.compare("trans") == 0) {
		//activeObserver = make_unique<TransObserver>(this);
	}
	else if (csn.compare("2ndseq") == 0) {
		//activeObserver = make_unique<SecondSeqObserver>(this);
	}
	else if (csn.compare("others") == 0) {
		//activeObserver = make_unique<OthersObserver>(this);
	}
	else if (csn.compare("erase") == 0) {
		activeObserver = make_unique<EraseObserver>(mpc);
	}
	else if (csn.compare("sync") == 0) {
		activeObserver = make_unique<SyncObserver>(mpc);
	}
	else if (csn.compare("assign") == 0) {
		activeObserver = make_unique<AssignObserver>(mpc);
	}
	else if (csn.compare("assign16levels") == 0) {
		activeObserver = make_unique<Assign16LevelsObserver>(mpc);
	}
	else if (csn.compare("metronomesound") == 0) {
		activeObserver = make_unique<MetronomeSoundObserver>(mpc);
	}
	else if (csn.compare("saveallfile") == 0) {
		activeObserver = make_unique<SaveAllFileObserver>(mpc);
	}
	else if (csn.compare("loadasequencefromall") == 0) {
		activeObserver = make_unique<LoadASequenceFromAllObserver>(mpc);
	}
	else if (csn.compare("nextseqpad") == 0) {
		activeObserver = make_unique<NextSeqPadObserver>(mpc);
	}
	else if (csn.compare("nextseq") == 0) {
		activeObserver = make_unique<NextSeqObserver>(mpc);
	}
	else if (csn.compare("song") == 0) {
		activeObserver = make_unique<SongObserver>(mpc);
	}
	else if (csn.compare("trackmute") == 0) {
		activeObserver = make_unique<TrMuteObserver>(mpc);
	}
	else if (checkActiveScreen(&diskNames, currentScreenName)) {
		activeObserver = make_unique<DiskObserver>(mpc);
	}
	else if (checkActiveScreen(&seqWindowNames, currentScreenName)) {
		activeObserver = make_unique<SequencerWindowObserver>(mpc);
	}
	else if (checkActiveScreen(&soundNames, currentScreenName)) {
		activeObserver = make_unique<SoundObserver>(mpc);
	}
	else if (csn.compare("sample") == 0) {
		activeObserver = make_unique<SampleObserver>(mpc);
	}
	else 
	if (csn.compare("sequencer") == 0) {
		activeObserver = make_unique<SequencerObserver>(mpc);
	}
	else if (csn.compare("directory") == 0) {
		activeObserver = make_unique<DirectoryObserver>(mpc->getDisk(), mpc);
	}
	else if (csn.compare("programparams") == 0) {
		activeObserver = make_unique<PgmParamsObserver>(mpc);
	}
	else if (csn.compare("programassign") == 0) {
		activeObserver = make_unique<PgmAssignObserver>(mpc);
	}
	else if (csn.compare("sequencer_step") == 0) {
		activeObserver = make_unique<StepEditorObserver>(mpc);
	}
	else if (csn.compare("step_tc") == 0 || csn.compare("editmultiple") == 0 || csn.compare("insertevent") == 0) {
		activeObserver = make_unique<StepWindowObserver>(mpc);
	}
	else if (csn.compare("mixer") == 0 || csn.compare("channelsettings") == 0 || csn.compare("mixersetup") == 0) {
		activeObserver = make_unique<MixerObserver>(mpc);
	}
	else if (csn.compare("edit") == 0) {
		activeObserver = make_unique<EditSequenceObserver>(mpc);
	}
	else if (csn.compare("name") == 0) {
		activeObserver = make_unique<ui::NameObserver>(mpc);
	}
	else if (csn.compare("midiinputmonitor") == 0 || csn.compare("midioutputmonitor") == 0) {
		activeObserver = make_unique<MidiMonitorObserver>(mpc);
	}
	else if (csn.compare("barcopy") == 0) {
		activeObserver = make_unique<BarCopyObserver>(mpc);
	}
	else if (csn.compare("trmove") == 0) {
		activeObserver = make_unique<TrMoveObserver>(mpc);
	}
	else if (csn.compare("user") == 0) {
		activeObserver = make_unique<UserObserver>(mpc);
	}
	else if (csn.compare("trim") == 0) {
		activeObserver = make_unique<TrimObserver>(mpc);
	}
	else if (csn.compare("loop") == 0) {
		activeObserver = make_unique<LoopObserver>(mpc);
	}
	else if (csn.compare("editsound") == 0) {
		activeObserver = make_unique<EditSoundObserver>(mpc);
	}
	else if (csn.find("startfine") != string::npos || csn.find("endfine") != string::npos || csn.compare("looptofine") == 0) {
		activeObserver = make_unique<ZoomObserver>(mpc);
	}
	else if (csn.compare("zone") == 0 || csn.compare("numberofzones") == 0) {
		activeObserver = make_unique<ZoneObserver>(mpc);
	}
	else if (csn.compare("params") == 0) {
		activeObserver = make_unique<SndParamsObserver>(mpc);
	}
	else if (csn.compare("deleteallfiles") == 0) {
		activeObserver = make_unique<DeleteAllFilesObserver>(mpc);
	}
	else if (checkActiveScreen(&samplerWindowNames, csn)) {
		activeObserver = make_unique<SamplerWindowObserver>(mpc);
	}
	else if (csn.compare("purge") == 0) {
		activeObserver = make_unique<PurgeObserver>(mpc);
	}
	else if (csn.compare("drum") == 0) {
		activeObserver = make_unique<DrumObserver>(mpc);
	}
	else if (csn.compare("muteassign") == 0) {
		activeObserver = make_unique<MuteAssignObserver>(mpc);
	}
}

string LayeredScreen::getFocus() {
	return layers[currentLayer]->getFocus();
}

void LayeredScreen::setFocus(string focus) {
	string oldFocus = getFocus();
	layers[currentLayer]->setFocus(focus);
}

LayeredScreen::~LayeredScreen() {
	if (currentBackground != nullptr) {
		delete currentBackground;
	}
}
