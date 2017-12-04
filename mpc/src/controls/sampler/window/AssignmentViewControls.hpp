#pragma once
#include <controls/sampler/AbstractSamplerControls.hpp>

namespace mpc {
	namespace controls {
		namespace sampler {
			namespace window {

				class AssignmentViewControls
					: public mpc::controls::sampler::AbstractSamplerControls
				{

				public:
					typedef mpc::controls::sampler::AbstractSamplerControls super;
					void up() override;
					void down() override;
					void left() override;
					void right() override;
					void turnWheel(int i) override;
					virtual void pad(int i, int velo);

					AssignmentViewControls(mpc::Mpc* mpc);

				};

			}
		}
	}
}
