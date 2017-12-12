#include "Component.hpp"

#include "BlinkLabel.hpp"
#include <Mpc.hpp>

#include <string>

using namespace std;
using namespace mpc::lcdgui;

void Component::Hide(bool b) 
{ 
	if (hidden != b) { 
		hidden = b;
		SetDirty();
		if (!rect.Empty()) {
			auto r = rect;
			MLOG("Component hide rect: " + to_string(r.L) + " " + to_string(r.T) + " " + to_string(r.W()) + " " + to_string(r.H()));
			clearRects.push_back(rect);
		}
	} 
}

void Component::SetDirty() 
{ 
	dirty = true; 
}

bool Component::IsHidden()
{ 
	return hidden; 
}

bool Component::IsDirty() { return dirty; }

bool Component::NeedsClearing()
{
	return clearRects.size() != 0;
}

void Component::Clear(std::vector<std::vector<bool> >* pixels) {
	if (dynamic_cast<lcdgui::BlinkLabel*>(this) != nullptr) {
		MLOG("Clearing blinklabel");
	}
	for (int k = 0; k < clearRects.size(); k++) {
		auto r = clearRects[k];
		bool alreadyDone = false;
		for (int k1 = 0; k1 < k; k1++) {
			if (clearRects[k1] == r) {
				alreadyDone = true;
				break;
			}
		}
		if (alreadyDone) continue;
		for (int i = r.L; i < r.R + 1; i++) {
			for (int j = r.T; j < r.B + 1; j++) {
				if (i < 0 || i > 247 || j < 0 || j > 59) continue;
				(*pixels)[i][j] = false;
			}
		}
	}
	clearRects.clear();
}

MRECT* Component::GetRECT()
{ 
	return &rect;
}
