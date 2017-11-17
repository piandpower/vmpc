#pragma once
#include <audio/core/FloatSampleBuffer.hpp>
#include <audio/core/MetaInfo.hpp>

#include <string>

#include <memory>

namespace ctoot {
	namespace audio {
		namespace core {
            
            class ChannelFormat;

			class AudioBuffer
				: public FloatSampleBuffer
			{

			private:
				std::weak_ptr<MetaInfo> metaInfo{ };
				ChannelFormat* channelFormat{ nullptr };
				bool realTime{ true };
				std::string name{ "" };

			public:
				std::string getName();

			protected:
				void setChannelCount(int count);

			public:
				void setMetaInfo(std::weak_ptr<MetaInfo> info);
				std::weak_ptr<MetaInfo> getMetaInfo();
				bool isRealTime();
				void setRealTime(bool realTime);
			public:
				ChannelFormat* guessFormat();

			public:
				ChannelFormat* getChannelFormat();
				void setChannelFormat(ChannelFormat* format);
				void monoToStereo();
				void convertTo(ChannelFormat* format);
				void swap(int a, int b);
				float square();
				bool encodeMidSide();
				bool decodeMidSide();
				void copyFrom(AudioBuffer* src, int nFrames);

				AudioBuffer(std::string name, int channelCount, int sampleCount, float sampleRate);
				~AudioBuffer();
			private:
				friend class MetaInfo;

			};

		}
	}
}
