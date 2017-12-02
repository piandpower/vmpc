#include <lcdgui/EnvGraph.hpp>

using namespace mpc::lcdgui;
using namespace std;

EnvGraph::EnvGraph(vector<vector<int>> coordinates) 
{
	this->coordinates = coordinates;
}

void EnvGraph::setCoordinates(vector<vector<int>> ia)
{
    coordinates = ia;
	SetDirty();
}

//bool EnvGraph::Draw(IGraphics* g1)
//{
//    //auto g = Graphics2D(g1);
//    //auto envGraph = new BufferedImage(248, 60);
//    //auto ig = envGraph->createGraphics();
//    //ig->setStroke(BasicStroke(1));
//	//ig->setColor(gui::Constants::LCD_ON());
//    for (int j = 0; j < coordinates.size(); j++) {
//        //ig->drawLine(coordinates[j][0], coordinates[j][1], coordinates[j][2], coordinates[j][3]);
//    }
//    //g->drawImage(envGraph, getScaleInstance(2, 2), this);
//	return true;
//}
