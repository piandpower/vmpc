#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <audio/core/AudioFormat.hpp>
#include <audio/core/FloatSampleTools.hpp>

namespace ctoot {
	namespace audio {
		namespace core {

			class FloatSampleBuffer
			{

			private:
				std::vector<std::vector<float> > channels;
				int sampleCount = { 0 };
				float sampleRate = { 0 };
				int originalFormatType = { 0 };
				static const bool LAZY_DEFAULT = { true };
				int channelCount = { 0 };

			public:
				static const int DITHER_MODE_AUTOMATIC = { 0 };
				static const int DITHER_MODE_ON = { 1 };
				static const int DITHER_MODE_OFF = { 2 };

			private:
				float ditherBits = { DEFAULT_DITHER_BITS };
				int ditherMode = { DITHER_MODE_AUTOMATIC };


			protected:
				void init_(int channelCount, int sampleCount, float sampleRate);
				void init_(int channelCount, int sampleCount, float sampleRate, bool lazy);

			private:

				void createChannels(int channelCount, int sampleCount, bool lazy);


			public:
				void insertChannel(int index, bool silent);
				void insertChannel(int index, bool silent, bool lazy);
				void initFromByteArray_(std::vector<char> buffer, int offset, int byteCount, AudioFormat* format);
				void initFromByteArray_(std::vector<char> buffer, int offset, int byteCount, AudioFormat* format, bool lazy);
				void initFromFloatSampleBuffer(FloatSampleBuffer* source);
				void reset();
				void reset(int channels, int sampleCount, float sampleRate);
				int getByteArrayBufferSize(AudioFormat* format);
				int getByteArrayBufferSize(AudioFormat* format, int lenInSamples);
				int convertToByteArray_(std::vector<char>* buffer, int offset, AudioFormat* format);
				int convertToByteArray_(int readOffset, int lenInSamples, std::vector<char>* buffer, int writeOffset, AudioFormat* format);
				std::vector<char> convertToByteArray_(AudioFormat* format);

				void setSamplesFromBytes(std::vector<char> input, int inByteOffset, AudioFormat* format, int floatOffset, int frameCount);
				void changeSampleCount(int newSampleCount, bool keepOldSamples);
				void makeSilenceFrames(int nFrames);
				void makeSilence(int channel, int nFrames);
				void addChannel(bool silent);
				void expandChannel(int targetChannelCount);
				void mixDownChannels();
				void copyChannel(int sourceChannel, int targetChannel);
				void copy(int sourceIndex, int destIndex, int length);
				void copy(int channel, int sourceIndex, int destIndex, int length);
				void removeChannel(int channel);
				void removeChannel(int channel, bool lazy);

				int getChannelCount();
				std::vector<float>* getChannel(int channel);

				int getSampleCount();

				void setSampleRate(float sampleRate);
				float getSampleRate();
                std::vector<std::vector<float> >* getAllChannels();

				float getDitherBits();
				int getDitherMode();
				void setDitherBits(float ditherBits);
				void setDitherMode(int mode);

			public:
				float getConvertDitherBits(int newFormatType);

			public:
				FloatSampleBuffer();
				FloatSampleBuffer(int channelCount, int sampleCount, float sampleRate);
				FloatSampleBuffer(std::vector<char> buffer, int offset, int byteCount, AudioFormat* format);

				~FloatSampleBuffer();

			};

		}
	}
}
