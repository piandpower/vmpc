#pragma once

#include <vector>
#include <string>

//#include "RtAudio.h"

#ifndef __APPLE__
//#include "include/asiodrivers.h"
#endif

namespace ctoot {
	namespace audio {
		namespace server {

			class Scanner final {

			private:
				std::vector<int> directSoundInputDevices;
				std::vector<int> directSoundOutputDevices;
				std::vector<std::string> directSoundDeviceNames;

				std::vector<std::string> asioDeviceNames;

                std::vector<std::string> coreAudioInDeviceNames;
                std::vector<std::string> coreAudioOutDeviceNames;
                
			public:
				std::vector<std::string> getDirectSoundInputDeviceNames();
				std::vector<std::string> getDirectSoundOutputDeviceNames();

				std::vector<std::string> getAsioDeviceNames();
                
                std::vector<std::string> getCoreAudioInDeviceNames();
                std::vector<std::string> getCoreAudioOutDeviceNames();

			public:
				int getDirectSoundDeviceID(std::string name);
				//int getDeviceName(int id);

			private:
				void scanDirectSound();
				void scanAsio();
                void scanCoreAudio();

			public:
				void scan(); // must be run at least once before doing anything else


			};

		}
	}
}
