#include <lcdgui/Popup.hpp>
#include <lcdgui/Background.hpp>

#include <cmath>

using namespace mpc::lcdgui;
using namespace std;

Popup::Popup() 
{
	bg = make_unique<Background>();
	bg->setName("popup");
	//plugBase->GetGUI()->AttachControl(bg.get());
	//int x = maingui::Constants::POPUP_RECT()->L + 20;
	//int y = maingui::Constants::POPUP_RECT()->T + 12;
	int textWidth = 200;
	int textHeight = 8;
	//textRect = IRECT(x, y, x + textWidth, y + textHeight);
}

void Popup::setText(string text, int pos)
{
	this->text = text;
	this->pos = pos;
	//SetDirty(true);
}

/*
bool Popup::Draw(IGraphics* g)
{
	bg->Draw(g);
	char* textChars = &text[0];
	g->DrawIText(Constants::FONT_OFF(), textChars, &textRect);
	return true;
}
*/