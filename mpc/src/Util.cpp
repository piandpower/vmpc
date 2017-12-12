#include <Util.hpp>

#include <cmath>
#include <cstdlib>

using namespace std;
using namespace mpc;

std::string Util::replaceDotWithSmallSpaceDot(std::string s) {
	string res;
	string tempodot = u8"\u00CB";
	auto dotindex = (int)(s.find('.'));
	auto part1 = s.substr(0, dotindex);
	auto part2 = s.substr(dotindex + 1);
	res = part1 + tempodot + part2;
	return res;
}

vector<char> Util::getPadAndVelo(int x, int y)
{
	int velocity;
	int padSize = 93;
	int emptySize = 23;
	int padPosX = 785;
	int padPosY = 343;
	int xPos = -1;
	int yPos = -1;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			int xborderl = padPosX + (i * padSize) + (i * (emptySize + (i * 2)));
			int xborderr = xborderl + padSize;
			int yborderu = padPosY + (j * padSize) + (j * (emptySize + (j * 2)));
			int yborderd = yborderu + padSize;
			int centerx = xborderl + (padSize / 2);
			int centery = yborderu + (padSize / 2);
			if (x > xborderl && x < xborderr && y > yborderu && y < yborderd) {
				xPos = i;
				yPos = j;
				int distcx = abs(centerx - x);
				int distcy = abs(centery - y);
				velocity = 127 - ((127.0 / 46.0) * ((distcx + distcy) / 2.0));
				break;
			}
		}
	}
	if (yPos == -1 || yPos == -1) return vector<char>{ -1, -1 };
	int padNumber = -1;
	vector<char> column0 = { 12, 8, 4, 0 };
	vector<char> column1 = { 13, 9, 5, 1 };
	vector<char> column2 = { 14, 10, 6, 2 };
	vector<char> column3 = { 15, 11, 7, 3 };
	auto columns = vector < vector<char>>(4);
	columns[0] = column0;
	columns[1] = column1;
	columns[2] = column2;
	columns[3] = column3;
	padNumber = columns[xPos][yPos];
	return vector<char>{ (char)padNumber, (char)velocity };
}

string Util::getFileName(string s)
{
	string copy = s;
	copy = moduru::lang::StrUtil::trim(copy);
	for (auto c : copy) {
		c = toupper(c);
		if (c == ' ') c = '_';
	}
	return copy;
}

string* Util::splitName(string s) {
	if (s.find(".") == string::npos) {
		string res[2];
		res[0] = s;
		res[1] = "";
		return res;
	}
	size_t i = s.find_last_of(".");
	string res[2];
	res[0] = s.substr(0, i);
	res[1] = s.substr(i + 1);
	return res;
}

string Util::distributeTimeSig(string s) {
	auto pos = s.find("/");
	if (pos == string::npos) return s;
	auto s0 = s.substr(0, pos);
	auto s1 = s.substr(pos + 1, s.length());
	if (s0.length() == 1) s0 = u8"\u00CE" + s0 + u8"\u00CE";
	if (s1.length() == 1) s1 = u8"\u00CE" + s1;
	return s0 + "/" + s1;
}

void Util::drawLine(std::vector<std::vector<bool> >* pixels, std::vector<std::vector<int> >* line, bool color) {
	for (auto& l : *line) {
		(*pixels)[l[0]][l[1]] = color;
	}
}

void Util::drawLine(std::vector<std::vector<bool> >* pixels, std::vector<std::vector<int> >* line, bool color, std::vector<int>* offsetxy) {
	for (auto& l : *line) {
		(*pixels)[l[0] + offsetxy->at(0)][l[1] + offsetxy->at(1)] = color;
	}
}

void Util::drawLines(std::vector<std::vector<bool> >* pixels, vector<vector<vector<int>>>* lines, vector<bool> colors) {
	int colorCounter = 0;
	for (auto line : *lines) {
		drawLine(pixels, &line, colors[colorCounter++]);
	}
}

void Util::drawLines(std::vector<std::vector<bool> >* pixels, vector<vector<vector<int>>>* lines, vector<bool> colors, std::vector<int>* offsetxy) {
	int colorCounter = 0;
	for (auto line : *lines) {
		drawLine(pixels, &line, colors[colorCounter++], offsetxy);
	}
}

/*
void Util::drawScaled(IGraphics* g, std::vector<std::vector<int>> line, unsigned int scale, IColor* color, vector<int> offsetxy) {
	int x, y;
	for (auto& v : line) {
		x = (v[0] * scale) + offsetxy[0];
		y = (v[1] * scale) + offsetxy[1];
		IRECT r(x, y, x + (scale-1), y + (scale-1));
		g->DrawRect(color, &r);
		if (scale > 2) g->FillIRect(color, &r);
	}
}

void Util::drawScaled(IGraphics* g, vector<vector<vector<int>>> lines, unsigned int scale, IColor* color, vector<int> offsetxy) {
	for (auto& line : lines) {
		drawScaled(g, line, scale, color, offsetxy);
	}
}

*/