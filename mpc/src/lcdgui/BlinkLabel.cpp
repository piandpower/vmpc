#include <lcdgui/BlinkLabel.hpp>

#include <chrono>

using namespace mpc::lcdgui;
using namespace std;

BlinkLabel::BlinkLabel(string text) 
	: Label()
{
	setText(text);
}

void BlinkLabel::static_blink(void * args)
{
	static_cast<BlinkLabel*>(args)->runBlinkThread();
}

void BlinkLabel::runBlinkThread() {
	while (blinkingOn) {
		this_thread::sleep_for(chrono::milliseconds(BLINK_INTERVAL));
		Hide(!IsHidden());
		SetDirty();
	}
	Hide(false);
}

void BlinkLabel::setBlinking(bool flag)
{
    this->blinkingOn = flag;
	if (blinkingOn) {
		if (blinkThread.joinable()) blinkThread.join();
		blinkThread = thread(&BlinkLabel::static_blink, this);
	}
}

bool BlinkLabel::getBlinking()
{
    return blinkingOn;
}

BlinkLabel::~BlinkLabel() {
	if (blinkThread.joinable()) blinkThread.join();
}
