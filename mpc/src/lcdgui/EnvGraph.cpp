#include <lcdgui/EnvGraph.hpp>

#include <gui/Bressenham.hpp>

using namespace mpc::lcdgui;
using namespace std;

EnvGraph::EnvGraph() {
}

EnvGraph::EnvGraph(vector<vector<int> > coordinates) 
{
	this->coordinates = coordinates;
}

void EnvGraph::setCoordinates(vector<vector<int> > ia)
{
    coordinates = ia;
	SetDirty();
}

void EnvGraph::Draw(std::vector<std::vector<bool> >* pixels)
{
	int xoff = 76;
	int yoff = 16;
	int width = 44;
	int height = 28;
	for (int x = 0; x < width-1; x++) {
		for (int y = 0; y < height-1; y++) {
			int x1 = xoff + x;
			int y1 = yoff + y;
			if (y1 == 24 && x1 >= 93 && x1 <= 101) continue;
			pixels->at(x1)[y1] = false;
		}
	}
	for (auto& c : coordinates) {
		auto linePixels = moduru::gui::Bressenham::Line(c[0], c[1], c[2], c[3]);
		for (auto& l : linePixels) {
			pixels->at(l[0]).at(l[1]) = true;
		}
	}
	dirty = false;
}

EnvGraph::~EnvGraph() {
}
