#pragma once
#include <audio/core/ChannelFormat.hpp>
#include <cstdint>

namespace ctoot {
	namespace audio {
		namespace core {

			class ChannelFormat_4
				: public ChannelFormat
			{

			private:
				int8_t left[2];
				int8_t right[2];

			public:
				int getCount() override;
				bool isCenter(int chan) override;
				bool isLeft(int chan) override;
				bool isRight(int chan) override;
				bool isFront(int chan) override;
				bool isRear(int chan) override;
				bool isLFE(int chan) override;
				int getCenter() override;
				int getLFE() override;
				int8_t* getLeft() override;
				int8_t* getRight() override;
				std::string getName() override;
				std::string getName(int chan) override;
				ChannelFormat_4();
			};
		}
	}
}
