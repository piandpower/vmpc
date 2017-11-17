#include <audio/core/ChannelFormat_1.hpp>

using namespace ctoot::audio::core;
using namespace std;

ChannelFormat_1::ChannelFormat_1()
{
}


int ChannelFormat_1::getCount()
{
    return 1;
}

bool ChannelFormat_1::isCenter(int chan)
{
    return true;
}

bool ChannelFormat_1::isLeft(int chan)
{
    return false;
}

bool ChannelFormat_1::isRight(int chan)
{
    return false;
}

bool ChannelFormat_1::isFront(int chan)
{
    return true;
}

bool ChannelFormat_1::isRear(int chan)
{
    return false;
}

bool ChannelFormat_1::isLFE(int chan)
{
    return false;
}

int ChannelFormat_1::getCenter()
{
    return 0;
}

int ChannelFormat_1::getLFE()
{
    return -1;
}

int8_t* ChannelFormat_1::getLeft()
{
    return empty;
}

int8_t* ChannelFormat_1::getRight()
{
    return empty;
}

string ChannelFormat_1::getName()
{
	return "Mono";
}

string ChannelFormat_1::getName(int chan)
{
	return "Centre";
}
