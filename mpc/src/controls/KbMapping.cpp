#include <controls/KbMapping.hpp>
#include <thirdp/wrpkey/key.hxx>

using namespace mpc::controls;
using namespace WonderRabbitProject::key;
using namespace std;

KbMapping::KbMapping()
{
}
const key_helper_t* mpc::controls::KbMapping::kh = &key_helper_t::instance();

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
