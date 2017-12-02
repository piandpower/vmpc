#pragma once

#include <memory>
#include <vector>
#include <string>

namespace mpc {

	class Mpc;

	namespace file {
		namespace aps {

			class ApsGlobalParameters
			{

			public:
				bool padToInternalSound_{};
				bool padAssignMaster_{};
				bool stereoMixSourceDrum_{};
				bool indivFxSourceDrum_{};
				bool copyPgmMixToDrum_{};
				bool recordMixChanges_{};
				int masterLevel{};
				int fxDrum{};

			private:
				static std::vector<char> TEMPLATE;

			public:
				std::vector<char> saveBytes{};

			private:
				int readFxDrum(char b);

			public:
				int getFxDrum();
				bool isPadToIntSoundEnabled();
				bool isPadAssignMaster();
				bool isStereoMixSourceDrum();
				bool isIndivFxSourceDrum();
				bool copyPgmMixToDrum();
				bool recordMixChanges();
				int getMasterLevel();

			public:
				std::vector<char> getBytes();

			public:
				ApsGlobalParameters(std::vector<char> loadBytes);
				ApsGlobalParameters(mpc::Mpc* mpc);

			};
		}
	}
}
