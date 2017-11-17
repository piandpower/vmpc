#pragma once
#include <audio/server/IOAudioProcess.hpp>

#include <string>

namespace ctoot {
	namespace audio {

		namespace core {
			class ChannelFormat;
		}

		namespace server {

			class AudioServerProcess
				: public IOAudioProcess
			
			{

			private:
				std::string name{ "" };
			
			protected:
				//shared_ptr<ctoot::audio::core::MetaInfo> metaInfo{ nullptr };
				ctoot::audio::core::ChannelFormat* format{ nullptr };

			public:
				std::string getName() override;
				ctoot::audio::core::ChannelFormat* getChannelFormat();

				virtual void open() = 0;
				virtual void close() = 0;

				AudioServerProcess(std::string name, bool mono);
			};

		}
	}
}
