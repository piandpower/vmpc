#include <lcdgui/Knob.hpp>

#include <Util.hpp>
#include <gui/Bressenham.hpp>


#include <cmath>

using namespace mpc::lcdgui;
using namespace moduru::gui;
using namespace std;

Knob::Knob() 
{
}

void Knob::setValue(int value)
{
    this->value = value;
	//SetDirty(true);
}

void Knob::setColor(bool on)
{
    color = on;
	//SetDirty(false);
}

//bool Knob::Draw(IGraphics* g)
//{
//	vector<vector<vector<int>>> lines;
//	lines.push_back(Bressenham::Line(0, 3, 0, 7));
//	lines.push_back(Bressenham::Line(0, 7, 3, 10));
//	lines.push_back(Bressenham::Line(3, 10, 7, 10));
//	lines.push_back(Bressenham::Line(7, 10, 10, 7));
//	lines.push_back(Bressenham::Line(10, 7, 10, 3));
//	lines.push_back(Bressenham::Line(10, 3, 7, 0));
//	lines.push_back(Bressenham::Line(7, 0, 3, 0));
//	lines.push_back(Bressenham::Line(3, 0, 0, 3));
//
//	auto angle = (int) ((value * 3.1) - 245);
//	auto radius = 4.95f;
//	auto radiusInt = (int) (radius);
//	auto angleToRadians = angle / 180.0 * PI;
//	auto x0 = (int)(radius * cos(angleToRadians));
//	auto y0 = (int)(radius * sin(angleToRadians));
//	lines.push_back(Bressenham::Line(5, 5, x0 + radiusInt + 1, y0 + radiusInt + 1));
//
//	vector<int> offsetxy { mpc::maingui::Constants::LCD_RECT()->L + rect.L, mpc::maingui::Constants::LCD_RECT()->T + rect.T };
//	auto c = color ? Constants::LCD_ON() : Constants::LCD_OFF();
//	mpc::Util::drawScaled(g, lines, 2, c, offsetxy);
//	return true;
//}
