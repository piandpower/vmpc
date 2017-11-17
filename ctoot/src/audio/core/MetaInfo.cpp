#include <audio/core/MetaInfo.hpp>

using namespace ctoot::audio::core;
using namespace std;

MetaInfo::MetaInfo(string sourceLabel, string sourceLocation) 
{
	this->sourceLabel = sourceLabel;
	this->sourceLocation = sourceLocation;
}

string ctoot::audio::core::MetaInfo::getSourceLabel()
{
    return sourceLabel;
}

string ctoot::audio::core::MetaInfo::getSourceLocation()
{
    return sourceLocation;
}
