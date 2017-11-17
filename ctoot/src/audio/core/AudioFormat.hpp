#pragma once
#include <cstdint>
#include <audio/core/Encoding.hpp>

namespace ctoot {
	namespace audio {
		namespace core {

			class AudioFormat
			{

			protected:
				bool bigEndian{ false };
				int channels{ 0 };
				Encoding* encoding{ nullptr };
				float frameRate{ 0 };
				int frameSize{ 0 };

			private:
				//    ::java::util::HashMap* properties_ {  };
				const int NOT_SPECIFIED = -1;

			public:
				float sampleRate{ 0 };
				int sampleSizeInBits{ 0 };

			public:
				virtual int getChannels();
				virtual Encoding* getEncoding();
				virtual float getFrameRate();
				virtual int getFrameSize();
				//    virtual ::java::lang::Object* getProperty(string* key);
				virtual float getSampleRate();
				virtual int getSampleSizeInBits();
				virtual bool isBigEndian();
				virtual bool matches(AudioFormat* format);
				 //   virtual ::java::util::Map* properties();
				std::string toString();

				AudioFormat(float sampleRate, int sampleSizeInBits, int channels, bool signed_, bool bigEndian);
				AudioFormat(Encoding* encoding, float sampleRate, int sampleSizeInBits, int channels, int frameSize, float frameRate, bool bigEndian);
				//    AudioFormat(Encoding* encoding, float sampleRate, int sampleSizeInBits, int channels, int frameSize, float frameRate, bool bigEndian, ::java::util::Map* properties);
				~AudioFormat();
			};

		}
	}
}
