#pragma once
#include <observer/Observable.hpp>

namespace mpc {
	namespace ui {
		namespace sampler {

			class MixerSetupGui
				: public moduru::observer::Observable
			{

			private:
				int masterLevel{ 0 };
				int fxDrum{ 0 };
				bool stereoMixSourceDrum{ false };
				bool indivFxSourceDrum{ false };
				bool copyPgmMixToDrumEnabled{ false };
				bool recordMixChangesEnabled{ false };

			public:
				int getMasterLevel();
				void setMasterLevel(int i);
				int getFxDrum();
				void setFxDrum(int i);
				bool isStereoMixSourceDrum();
				void setStereoMixSourceDrum(bool b);
				bool isIndivFxSourceDrum();
				void setIndivFxSourceDrum(bool b);
				bool isCopyPgmMixToDrumEnabled();
				void setCopyPgmMixToDrumEnabled(bool b);
				bool isRecordMixChangesEnabled();
				void setRecordMixChangesEnabled(bool b);
				std::string getMasterLevelString();

				MixerSetupGui();

			public:
				static std::vector<std::string> masterLevelNames();

			};

		}
	}
}
