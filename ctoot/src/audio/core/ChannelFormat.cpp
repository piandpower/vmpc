#include <audio/core/ChannelFormat.hpp>
#include <audio/core/ChannelFormat_1.hpp>
#include <audio/core/ChannelFormat_2.hpp>
#include <audio/core/ChannelFormat_3.hpp>
#include <audio/core/ChannelFormat_4.hpp>
#include <cstdint>
#include <audio/core/AudioBuffer.hpp>

#include <Logger.hpp>

using namespace std;
using namespace ctoot::audio::core;

ChannelFormat::ChannelFormat()
{
}

int ChannelFormat::mix(ctoot::audio::core::AudioBuffer* destBuffer, ctoot::audio::core::AudioBuffer* sourceBuffer, vector<float> gain, int nFrames)
{
	auto doMix = destBuffer != sourceBuffer;
	auto snc = sourceBuffer->getChannelCount();
	auto dnc = destBuffer->getChannelCount();
	if (dnc > 4 && snc != dnc) dnc = 4;

	auto ns = destBuffer->getSampleCount();
	if (ns > nFrames) ns = nFrames;
	float g;
	auto k = static_cast<float>((snc)) / dnc;
	vector<float>* in;
	vector<float>* out;
	for (auto i = 0; i < dnc; i++) {
		g = gain[i] * k;
		g = 1.0f;
		in = sourceBuffer->getChannel(i % snc);
		out = destBuffer->getChannel(i);
		if (doMix) {
			for (auto s = 0; s < ns; s++) {
				(*out)[s] += (*in)[s] * g;
			}
		}
		else {
			for (auto s = 0; s < ns; s++) {
				(*out)[s] = (*in)[s] * g;
			}
		}
	}
	auto ret = 1;
	if (!doMix) ret |= 2;
	return ret;
}


ChannelFormat*& ChannelFormat::MONO()
{
    return MONO_;
}
ChannelFormat* ChannelFormat::MONO_ = new ChannelFormat_1();

ChannelFormat*& ChannelFormat::STEREO()
{
	return STEREO_;
}
ChannelFormat* ChannelFormat::STEREO_ = new ChannelFormat_2();


ChannelFormat*& ChannelFormat::QUAD()
{
	return QUAD_;
}
ChannelFormat* ChannelFormat::QUAD_ = new ChannelFormat_3();

ChannelFormat*& ChannelFormat::FIVE_1()
{
    return FIVE_1_;
}
ChannelFormat* ChannelFormat::FIVE_1_ = new ChannelFormat_4();