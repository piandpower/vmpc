#include <lcdgui/Knob.hpp>

#include <Util.hpp>
#include <gui/Bressenham.hpp>

#include <Logger.hpp>

#include <cmath>

using namespace mpc::lcdgui;
using namespace moduru::gui;
using namespace std;

Knob::Knob(MRECT rect) 
{
	this->rect = rect;
}

void Knob::setValue(int value)
{
    this->value = value;
	SetDirty();
}

void Knob::setColor(bool on)
{
    color = on;
	SetDirty();
}

void Knob::Draw(std::vector<std::vector<bool> >* pixels)
{
	if (IsHidden()) return;
	for (int i = rect.L-1; i < rect.R; i++) {
		for (int j = rect.T-1; j < rect.B; j++) {
			(*pixels)[i][j] = !color;
		}
	}
	vector<vector<vector<int>>> lines;
	lines.push_back(Bressenham::Line(0, 3, 0, 7));
	lines.push_back(Bressenham::Line(0, 7, 3, 10));
	lines.push_back(Bressenham::Line(3, 10, 7, 10));
	lines.push_back(Bressenham::Line(7, 10, 10, 7));
	lines.push_back(Bressenham::Line(10, 7, 10, 3));
	lines.push_back(Bressenham::Line(10, 3, 7, 0));
	lines.push_back(Bressenham::Line(7, 0, 3, 0));
	lines.push_back(Bressenham::Line(3, 0, 0, 3));
	
	int angle = (int) ((value * 3.1) - 245);
	float radius = 4.95f;
	int radiusInt = (int) (radius);
	float angleToRadians = ((float) (angle) / 180.0) * 3.14159265;
	int x0 = (int)(radius * cos(angleToRadians));
	int y0 = (int)(radius * sin(angleToRadians));
	lines.push_back(Bressenham::Line(5, 5, x0 + radiusInt + 1, y0 + radiusInt + 1));
	
	vector<bool> colors;
	for (auto& l : lines) {
		colors.push_back(color);
	}
	vector<int> offsetxy { rect.L, rect.T };
	mpc::Util::drawLines(pixels, &lines, colors, &offsetxy);
	dirty = false;
}

Knob::~Knob() {
}
