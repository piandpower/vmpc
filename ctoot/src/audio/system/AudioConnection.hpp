#pragma once

#include <string>

namespace ctoot {
	namespace audio {
		namespace system {

			class AudioConnection

			{

			public:
				static const int PLAYBACK{ 1 };
				static const int SYSTEM{ 2 };
				static const int OUTPUT_LOCKED{ 4 };
				static const int INPUT_LOCKED{ 8 };

			private:
				int flags{ 0 };

			public:
				virtual void close() = 0;
				virtual std::string getOutputName() = 0;
				virtual std::string getOutputLocation() = 0;
				virtual std::string getInputName() = 0;
				virtual bool isSystem();
				virtual bool isPlayback();

				AudioConnection(int flags);

			};

		}
	}
}
