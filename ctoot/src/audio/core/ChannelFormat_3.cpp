#include <audio/core/ChannelFormat_3.hpp>

using namespace std;
using namespace ctoot::audio::core;

ChannelFormat_3::ChannelFormat_3()
{
	left[0] = 0;
	left[1] = 2;
	right[0] = 1;
	right[1] = 3;
}

int ChannelFormat_3::getCount()
{
    return 4;
}

bool ChannelFormat_3::isCenter(int chan)
{
    return false;
}

bool ChannelFormat_3::isLeft(int chan)
{
    return (chan & 1) == 0;
}

bool ChannelFormat_3::isRight(int chan)
{
    return (chan & 1) == 1;
}

bool ChannelFormat_3::isFront(int chan)
{
    return chan < 2;
}

bool ChannelFormat_3::isRear(int chan)
{
    return chan >= 2;
}

bool ChannelFormat_3::isLFE(int chan)
{
    return false;
}

int ChannelFormat_3::getCenter()
{
    return -1;
}

int ChannelFormat_3::getLFE()
{
    return -1;
}

int8_t* ChannelFormat_3::getLeft()
{
    return left;
}

int8_t* ChannelFormat_3::getRight()
{
    return right;
}

string ChannelFormat_3::getName()
{
    return "Quad";
}

string ChannelFormat_3::getName(int chan)
{
	switch (chan) {
	case 0:
		return "Front.Left";
	case 1:
		return "Front.Right";
	case 2:
		return "Rear.Left";
	case 3:
		return "Rear Right";
	default:
		return "illegal channel";
	}
}