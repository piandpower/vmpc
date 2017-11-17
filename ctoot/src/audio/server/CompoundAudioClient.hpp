#pragma once
#include <audio/server/AudioClient.hpp>

#include <vector>
#include <memory>

namespace ctoot {
	namespace audio {
		namespace server {

			class CompoundAudioClient
				: public virtual AudioClient
			{

			private:
				std::vector<AudioClient*> clients{};
				bool enabled{ true };

			public:
				void work(int nFrames) override;
				void setEnabled(bool enable) override;
				void add(AudioClient* client);
				void remove(AudioClient*  client);

				CompoundAudioClient();
				~CompoundAudioClient();
			};

		}
	}
}
