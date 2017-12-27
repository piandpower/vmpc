#include <audiomidi/DirectToDiskSettings.hpp>

using namespace mpc::audiomidi;
using namespace std;

DirectToDiskSettings::DirectToDiskSettings(int lengthInFrames, string outputFolder, bool split)
{
	this->lengthInFrames = lengthInFrames;
	this->outputFolder = outputFolder;
	this->split = split;
}
