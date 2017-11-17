#pragma once
#include <audio/system/AudioSystem.hpp>

#include <vector>

namespace ctoot {
	namespace audio {
		namespace system {

			class AudioConnection;
			struct ConnectedAudioSystem
				: public AudioSystem
			{
				virtual std::vector<AudioConnection*>* getConnections() = 0;

			};

		}
	}
}
