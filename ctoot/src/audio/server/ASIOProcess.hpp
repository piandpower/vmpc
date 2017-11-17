#pragma once

#include <string>

namespace ctoot {
	namespace audio {
		namespace server {

			class AsioChannel;
			class ChannelFormat;
			class ASIOProcess {

			private:
				std::string name;
			protected:
				AsioChannel* info0;
				AsioChannel* info1;
				ChannelFormat* format;

			public:
				ChannelFormat* getChannelFormat();
				std::string getName();
				void open();
				void close();

			public:
				//ASIOProcess(std::string name, AsioChannel info0, AsioChannel info1);

			};
		}
	}
}
