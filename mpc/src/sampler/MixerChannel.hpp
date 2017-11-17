#pragma once

#include <observer/Observable.hpp>

namespace mpc {
	namespace sampler {

		class MixerChannel
			: public moduru::observer::Observable
		{

		private:
			int output{ 0 };
			int volumeIndividualOut{ 0 };
			int panning{ 0 };
			int level{ 0 };
			int fxPath{ 0 };
			int fxSendLevel{ 0 };
			bool followStereo{ false };
			bool stereo{ false };

		public:
			virtual void setFollowStereo(bool b);
			virtual bool isFollowingStereo();
			virtual void setStereo(bool b);
			virtual bool isStereo();
			int getOutput();
			void setOutput(int i);
			void setVolumeIndividualOut(int i);
			int getVolumeIndividualOut();
			void setPanning(int i);
			int getPanning();
			void setLevel(int i);
			int getLevel();
			void setFxPath(int i);
			int getFxPath();
			void setFxSendLevel(int i);
			int getFxSendLevel();

			MixerChannel();

		};
	}
}
