#include <audio/core/AudioBuffer.hpp>
#include <audio/core/ChannelFormat.hpp>

using namespace ctoot::audio::core;
using namespace std;

AudioBuffer::AudioBuffer(string name, int channelCount, int sampleCount, float sampleRate) : FloatSampleBuffer(channelCount, sampleCount, sampleRate)
{
	this->name = name;
	realTime = true;
	channelFormat = guessFormat();
}

string AudioBuffer::getName()
{
   return name;
}

void AudioBuffer::setChannelCount(int count)
{
	if (count == getChannelCount()) return;

	if (count < getChannelCount()) {
		for (auto ch = getChannelCount() - 1; ch > count - 1; ch--) {
			removeChannel(ch);
		}
	}
	else {
		while (getChannelCount() < count) {
			addChannel(false);
		}
	}
}


void AudioBuffer::setMetaInfo(weak_ptr<MetaInfo> info)
{
    //metaInfo = info;
}

weak_ptr<MetaInfo> AudioBuffer::getMetaInfo()
{
    return metaInfo;
}

bool AudioBuffer::isRealTime()
{
    return realTime;
}

void AudioBuffer::setRealTime(bool realTime)
{
    this->realTime = realTime;
}


ChannelFormat* AudioBuffer::guessFormat()
{
    switch (getChannelCount()) {
    case 1:
        return ChannelFormat::MONO();
    case 2:
        return ChannelFormat::STEREO();
    case 4:
        return ChannelFormat::QUAD();
    case 6:
        return ChannelFormat::FIVE_1();
    }
    return ChannelFormat::STEREO();
}

ChannelFormat* AudioBuffer::getChannelFormat()
{
    return channelFormat;
}

void AudioBuffer::setChannelFormat(ChannelFormat* format)
{
    if(channelFormat == format)
        return;

    channelFormat = format;
    if(channelFormat != nullptr) {
        setChannelCount(channelFormat->getCount());
    }
}


void AudioBuffer::convertTo(ChannelFormat* format)
{
    if(channelFormat == format)
        return;

    if(format->getCount() == 1) {
        mixDownChannels();
        channelFormat = format;
    } else if(channelFormat->getCount() == 1) {
        auto nc = format->getCount();
        auto ns = getSampleCount();
        auto samples = getChannel(0);
        auto gain = 1.0f / nc;
        for (auto s = 0; s < ns; s++) {
            (*samples)[s] *= gain;
        }
        expandChannel(nc);
        channelFormat = format;
    } else {
        auto convertingFormat = channelFormat->getCount() > format->getCount() ? channelFormat : format;
    }
}


void AudioBuffer::monoToStereo()
{
    if(getChannelCount() < 2) {
        convertTo(ChannelFormat::STEREO());
   }
}

void AudioBuffer::swap(int a, int b)
{
    auto ns = getSampleCount();
    auto asamples = getChannel(a);
    auto bsamples = getChannel(b);
    float tmp;
    for (auto s = 0; s < ns; s++) {
        tmp = (*asamples)[s];
        (*asamples)[s] = (*bsamples)[s];
        (*bsamples)[s] = tmp;
    }
}

float AudioBuffer::square()
{
	auto ns = getSampleCount();
	auto nc = getChannelCount();
	auto sumOfSquares = 0.0f;
	vector<float>* samples;
	for (auto c = 0; c < nc; c++) {
		samples = getChannel(c);
		for (auto s = 0; s < ns; s++) {
			auto sample = (*samples)[s];
			sumOfSquares += sample * sample;
		}
	}
	return sumOfSquares / (nc * ns);
}

bool AudioBuffer::encodeMidSide()
{
	auto lefts = channelFormat->getLeft();
    auto rights = channelFormat->getRight();

	if(sizeof lefts == 0)
        return false;

	auto np = sizeof lefts;
    auto ns = getSampleCount();
    for (auto p = 0; p < np; p++) {
        auto left = getChannel(lefts[p]);
        auto right = getChannel(rights[p]);
        for (auto s = 0; s < ns; s++) {
            auto mid = 0.5f * ((*left)[s] + (*right)[s]);
            auto side = 0.5f * ((*left)[s] - (*right)[s]);
            (*left)[s] = mid;
            (*right)[s] = side;
        }
    }
	return true;
}


bool AudioBuffer::decodeMidSide()
{
	auto mids = channelFormat->getLeft();
    auto sides = channelFormat->getRight();
    if(sizeof mids == 0)
        return false;

    auto np = sizeof mids;
    auto ns = getSampleCount();
    for (auto p = 0; p < np; p++) {
        auto mid = getChannel(mids[p]);
        auto side = getChannel(sides[p]);
        for (auto s = 0; s < ns; s++) {
            auto left = (*mid)[s] + (*side)[s];
            auto right = (*mid)[s] - (*side)[s];
            (*mid)[s] = left;
            (*side)[s] = right;
        }
    }
    return true;
}

void AudioBuffer::copyFrom(AudioBuffer* src, int nFrames)
{
    if(src == nullptr) return;

    setChannelFormat(src->getChannelFormat());
    auto nc = getChannelCount();
    auto ns = getSampleCount();
	if (ns > nFrames) ns = nFrames;
    for (auto c = 0; c < nc; c++) {
        auto from = src->getChannel(c);
        auto to = getChannel(c);
		(*to) = (*from);
        //for (auto s = 0; s < ns; s++) {
        //   (*to)[s] = (*from)[s];
        //}
    }
}

AudioBuffer::~AudioBuffer() {
	if (channelFormat != nullptr) {
		channelFormat = nullptr;
	}
}
