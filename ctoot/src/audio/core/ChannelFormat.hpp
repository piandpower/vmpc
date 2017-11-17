#pragma once
#include <string>
#include <cstdint>
#include <vector>

#include <audio/core/AudioBuffer.hpp>

namespace ctoot {
	namespace audio {
		namespace core {
            
			class ChannelFormat
			{

			public:
				virtual int getCount() = 0;
				virtual int getCenter() = 0;
				virtual int getLFE() = 0;
				virtual int8_t* getLeft() = 0;
				virtual int8_t* getRight() = 0;
				virtual std::string getName() = 0;
				virtual bool isCenter(int chan) = 0;
				virtual bool isLeft(int chan) = 0;
				virtual bool isRight(int chan) = 0;
				virtual bool isFront(int chan) = 0;
				virtual bool isRear(int chan) = 0;
				virtual bool isLFE(int chan) = 0;
				virtual std::string getName(int chan) = 0;

				virtual int mix(ctoot::audio::core::AudioBuffer* destBuffer, ctoot::audio::core::AudioBuffer* sourceBuffer, std::vector<float> gain, int nFrames);
			private:
				static ChannelFormat* STEREO_;
				static ChannelFormat* MONO_;
				static ChannelFormat* QUAD_;
				static ChannelFormat* FIVE_1_;
			public:
				ChannelFormat();

			public:
				static ChannelFormat*& MONO();
				static ChannelFormat*& STEREO();
				static ChannelFormat*& QUAD();
				static ChannelFormat*& FIVE_1();
			};

		}
	}
}
