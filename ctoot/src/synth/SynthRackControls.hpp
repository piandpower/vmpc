#pragma once
#include <control/CompoundControl.hpp>

#include <string>
#include <vector>

namespace ctoot {
	namespace synth {

		class SynthControls;

		class SynthRackControls
			: public ctoot::control::CompoundControl
		{

		private:
			std::vector<std::shared_ptr<SynthControls>> synthControls{};
			int nsynths{ 0 };

		public:
			virtual int size();
			virtual std::shared_ptr<SynthControls> getSynthControls(int synth);
			virtual void setSynthControls(int synth, std::shared_ptr<SynthControls> controls);
			virtual void removeAll();
			int getProviderId() override;
			bool isPluginParent() override;
			std::string getPersistenceDomain() override;

			SynthRackControls(int nsynths);
			~SynthRackControls();
		};

	}
}
