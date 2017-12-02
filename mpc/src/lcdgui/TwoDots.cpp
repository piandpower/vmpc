#include <lcdgui/TwoDots.hpp>

#include <gui/Bressenham.hpp>

#include <Util.hpp>

using namespace mpc::lcdgui;
using namespace moduru::gui;
using namespace std;

TwoDots::TwoDots()
{
	selected = vector<bool>(4);
	visible = vector<bool>(4);
}

void TwoDots::setInverted(bool b)
{
    inverted = b;
	SetDirty();
}

void TwoDots::setSelected(int i, bool b)
{
    selected[i] = b;
	SetDirty();
}

void TwoDots::setVisible(int i, bool b)
{
    visible[i] = b;
	SetDirty();
}

//bool TwoDots::Draw(IGraphics* g)
//{
//	vector<vector<vector<int>>> lines;
//	vector<IColor*> colors;
//    for (int i = 0; i < 4; i++) {
//        if (selected[i]) {
//            color = inverted ? true : false;
//        } else {
//            color = inverted ? false : true;
//        }
//        
//        if (i == 0 && visible[i]) {
//            if (inverted) {
//				color = true;
//				colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//                lines.push_back(Bressenham::Line(25, 19, 73, 19));
//				color = false;
//            }
//			colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//            lines.push_back(Bressenham::Line(37, 19, 37, 19));
//			colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//			lines.push_back(Bressenham::Line(55, 19, 55, 19));
//        }
//        else if (i == 1 && visible[i]) {
//            if (inverted) {
//				color = true;
//				colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//				lines.push_back(Bressenham::Line(96 + 25, 19, 114 + 55, 19));
//				color = false;
//            }
//			colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//			lines.push_back(Bressenham::Line(96 + 37, 19, 96 + 37, 19));
//			colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//			lines.push_back(Bressenham::Line(114 + 37, 19, 114 + 37, 19));
//        }
//        else if (i == 2 && visible[i]) {
//            if (inverted) {
//                color = true;
//				colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//				lines.push_back(Bressenham::Line(155 + 25, 21, 173 + 57, 21));
//                color = false;
//            }
//			colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//			lines.push_back(Bressenham::Line(155 + 37, 21, 155 + 37, 21));
//			colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//			lines.push_back(Bressenham::Line(173 + 37, 21, 173 + 37, 21));
//        }
//        else if (i == 3 && visible[i]) {
//            if (inverted) {
//                color = true;
//				colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//				lines.push_back(Bressenham::Line(155 + 25, 30, 173 + 57, 30));
//                color = false;
//            }
//			colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//			lines.push_back(Bressenham::Line(155 + 37, 30, 155 + 37, 30));
//			colors.push_back(color ? Constants::LCD_ON() : Constants::LCD_OFF());
//			lines.push_back(Bressenham::Line(173 + 37, 30, 173 + 37, 30));
//        }
//    }
//
//	int xoff = mpc::maingui::Constants::LCD_RECT()->L + rect.L;
//	int yoff = mpc::maingui::Constants::LCD_RECT()->T + rect.T;
//
//	vector<int> offsetxy{ xoff, yoff };
//
//	mpc::Util::drawScaled(g, lines, 2, colors, offsetxy);
//	return true;
//}
