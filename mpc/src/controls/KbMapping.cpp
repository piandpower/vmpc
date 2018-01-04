#include "KbMapping.hpp"
#include <thirdp/wrpkey/key.hxx>

using namespace mpc::controls;
using namespace WonderRabbitProject::key;
using namespace std;

KbMapping::KbMapping()
{
	labelKeyMap["left"] = kh->code("left");
	labelKeyMap["right"] = kh->code("right");
	labelKeyMap["up"] = kh->code("up");
	labelKeyMap["down"] = kh->code("down");
	labelKeyMap["rec"] = kh->code("l");
	labelKeyMap["overdub"] = kh->code("semicolon");
	labelKeyMap["stop"] = kh->code("quote");
	labelKeyMap["play"] = kh->code("space");
	labelKeyMap["playstart"] = kh->code("backslash");
	labelKeyMap["mainscreen"] = kh->code("escape");
	labelKeyMap["openwindow"] = kh->code("i");
	labelKeyMap["prevstepevent"] = kh->code("q");
	labelKeyMap["nextstepevent"] = kh->code("w");
	labelKeyMap["goto"] = kh->code("e");
	labelKeyMap["prevbarstart"] = kh->code("r");
	labelKeyMap["nextbarend"] = kh->code("t");
	labelKeyMap["tap"] = kh->code("y");
	labelKeyMap["nextseq"] = kh->code("[");
	labelKeyMap["trackmute"] = kh->code("]");
	labelKeyMap["fulllevel"] = kh->code("o");
	labelKeyMap["sixteenlevels"] = kh->code("p");
	labelKeyMap["f1"] = kh->code("f1");
	labelKeyMap["f2"] = kh->code("f2");
	labelKeyMap["f3"] = kh->code("f3");
	labelKeyMap["f4"] = kh->code("f4");
	labelKeyMap["f5"] = kh->code("f5");
	labelKeyMap["f6"] = kh->code("f6");
	labelKeyMap["shift"] = kh->code("left shift");
	labelKeyMap["enter"] = kh->code("enter");
	labelKeyMap["undoseq"] = kh->code("f10");
	labelKeyMap["erase"] = kh->code("f11");
	labelKeyMap["after"] = kh->code("f12");
	labelKeyMap["banka"] = kh->code("home");
	labelKeyMap["bankb"] = kh->code("end");
	labelKeyMap["bankc"] = kh->code("insert");
	labelKeyMap["bankd"] = kh->code("delete");
	labelKeyMap["0"] = kh->code("0");
	labelKeyMap["1"] = kh->code("1");
	labelKeyMap["2"] = kh->code("2");
	labelKeyMap["3"] = kh->code("3");
	labelKeyMap["4"] = kh->code("4");
	labelKeyMap["5"] = kh->code("5");
	labelKeyMap["6"] = kh->code("6");
	labelKeyMap["7"] = kh->code("7");
	labelKeyMap["8"] = kh->code("8");
	labelKeyMap["9"] = kh->code("9");
}
const key_helper_t* mpc::controls::KbMapping::kh = &key_helper_t::instance();

int KbMapping::getKeyCodeFromLabel(std::string label) {
	if (labelKeyMap.find(label) == labelKeyMap.end()) return -1;
	return labelKeyMap[label];
}

std::string KbMapping::getLabelFromKeyCode(int keyCode) {
	for (std::pair<std::string, int> x : labelKeyMap) {
		std::string key = x.first;
		if (keyCode == x.second) return key;
	}
	return "";
}

std::string KbMapping::getKeyCodeString(int keyCode) {
	return kh->name(keyCode);
}

int KbMapping::left() {
	return kh->code("left");
}

int KbMapping::right() {
	return kh->code("right");
}

int KbMapping::up() {
	return kh->code("up");
}

int KbMapping::down() {
	return kh->code("down");
}

int KbMapping::dataWheelBack()
{
    return kh->code("minus");
}

int KbMapping::dataWheelForward()
{
    
	return kh->code("equals");
}

vector<int> KbMapping::bankKeys()
{
	return vector<int>{ kh->code("home"), kh->code("end"), kh->code("insert"), kh->code("delete") };
}

vector<int> KbMapping::padKeys()
{
    return vector<int>{ kh->code("z"), kh->code("x"), kh->code("c"), kh->code("v"), kh->code("a"), kh->code("s"), kh->code("d"), kh->code("f"), kh->code("b"), kh->code("n"), kh->code("m"), kh->code("comma"), kh->code("g"), kh->code("h"), kh->code("j"), kh->code("k") };
}

int KbMapping::rec()
{
    return kh->code("l");
}

int KbMapping::overdub()
{
	return kh->code("semicolon"); // semi-colon, 186
}

int KbMapping::stop()
{
	return kh->code("quote"); // quote, 222
}

int KbMapping::play()
{   
    return kh->code("space");
}

int KbMapping::playstart()
{   
    return kh->code("backslash"); // backslash, 220
}

int KbMapping::mainscreen()
{   
    return kh->code("escape");
}

int KbMapping::prevStepEvent()
{   
    return kh->code("q");
}

int KbMapping::nextStepEvent()
{   
    return kh->code("w");
}

int KbMapping::goTo()
{    
    return kh->code("e");
}

int KbMapping::prevBarStart()
{
    return kh->code("r");
}

int KbMapping::nextBarEnd()
{
    return kh->code("t");
}

int KbMapping::tap()
{
    return kh->code("y");
}

int KbMapping::nextSeq()
{
    return kh->code("[");
}

int KbMapping::trackMute()
{
    return kh->code("]");
}

int KbMapping::openWindow()
{
    return kh->code("i");
}

int KbMapping::fullLevel()
{
    return kh->code("o");
}

int KbMapping::sixteenLevels()
{
    return kh->code("p");
}

int KbMapping::f1()
{
    return kh->code("f1");
}

int KbMapping::f2()
{
    return kh->code("f2");
}

int KbMapping::f3()
{
    return kh->code("f3");
}

int KbMapping::f4()
{
    return kh->code("f4");
}

int KbMapping::f5()
{
    return kh->code("f5");
}

int KbMapping::f6()
{
    return kh->code("f6");
}

vector<int> KbMapping::numPad()
{
    return vector<int>{ kh->code("0"), kh->code("1"), kh->code("2"), kh->code("3"), kh->code("4"), kh->code("5"), kh->code("6"), kh->code("7"), kh->code("8"), kh->code("9") };
}

vector<int> KbMapping::altNumPad()
{
	return vector<int>{ 96, 97, 98, 99, 100, 101, 102, 103, 104, 105 };
}

int KbMapping::numPadShift()
{
    return kh->code("left shift");
}

int KbMapping::numPadEnter()
{
    return kh->code("enter");
}

int KbMapping::undoSeq()
{
    return kh->code("f10");
}

int KbMapping::erase()
{
	return kh->code("f11");
}

int KbMapping::after()
{
    return kh->code("f12");
}

int KbMapping::ctrl() {
	return kh->code("left control");
}

int KbMapping::alt() {
	return kh->code("left alternate");
}

int KbMapping::leftShift() {
	return kh->code("left shift");
}

int KbMapping::rightShift() {
	return kh->code("right shift");
}
