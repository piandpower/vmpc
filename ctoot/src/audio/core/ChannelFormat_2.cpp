#include <audio/core/ChannelFormat_2.hpp>

using namespace ctoot::audio::core;
using namespace std;

ChannelFormat_2::ChannelFormat_2()
{
	left[0] = 0;
	right[0] = 1;
}

int ChannelFormat_2::getCount()
{
    return 2;
}

bool ChannelFormat_2::isCenter(int chan)
{
    return false;
}

bool ChannelFormat_2::isLeft(int chan)
{
    return chan == 0;
}

bool ChannelFormat_2::isRight(int chan)
{
    return chan == 1;
}

bool ChannelFormat_2::isFront(int chan)
{
    return true;
}

bool ChannelFormat_2::isRear(int chan)
{
    return false;
}

bool ChannelFormat_2::isLFE(int chan)
{
    return false;
}

int ChannelFormat_2::getCenter()
{
    return -1;
}

int ChannelFormat_2::getLFE()
{
    return -1;
}

int8_t* ChannelFormat_2::getLeft()
{
    return left;
}

int8_t* ChannelFormat_2::getRight()
{
    return right;
}

string ChannelFormat_2::getName()
{
	return "Stereo";
}

string ChannelFormat_2::getName(int chan)
{
    switch (chan) {
    case 0:
        return "Left";
    case 1:
        return "Right";
    default:
		return "illegal channel";
    }

}