#pragma once

#include <string>

namespace ctoot {
	namespace audio {
		namespace server {

			class NamedIO {

			protected:
				std::string name;
				int first;
				int count;

			protected:
				NamedIO(std::string name, int first, int count);

			private:
				friend class AsioServer;
                friend class CoreAudioServer;
				friend class PluginAudioServer;

			};

		}
	}
}
