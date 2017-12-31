#pragma once

#include <vector>

namespace mpc {
	namespace sampler {

		class TimeStretch
		{

		private:
			static constexpr float TWO_PI{ 6.2831855f };
			int oldBlockSize{  };
			float overlap{  };
			std::vector<float> processedData{  };
			std::vector<float> newSampleData{  };
			int fadeLengthExpand{  };
			int specialFadeLength{  };
			int sampleRate{  };

		public:
			virtual std::vector<float> getProcessedData();

		private:
			std::vector<float> fade(float from, float to, int startIndex, int length, std::vector<float> fa);

		private:
			virtual float value(int length, int index);


		public:
			TimeStretch(std::vector<float> sampleData, float ratio, int sampleRate);

		};

	}
}
