#pragma once
#include <control/CompoundControl.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class Mutation
			{

			public:
				static const int ADD{ 1 };
				static const int REMOVE{ 2 };

			private:
				int operation{ 0 };
				std::shared_ptr<control::CompoundControl> control{ nullptr };

			public:
				virtual int getOperation();
				virtual std::shared_ptr<control::CompoundControl> getControl();
				std::string toString();

				Mutation(int operation, std::shared_ptr<control::CompoundControl> control);
				~Mutation();

			private:
				friend class MixerControls;
			};

		}
	}
}
