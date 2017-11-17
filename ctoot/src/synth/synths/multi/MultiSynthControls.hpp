#pragma once
#include <synth/ChannelledSynthControls.hpp>
#include <string>
#include <cstdint>

namespace ctoot {
	namespace synth {
		namespace synths {
			namespace multi {

				class MultiSynthControls
					: public ChannelledSynthControls
				{

				public:
					static const int ID{ 1 };

				private:
					static std::string NAME_;

				public:
					void setChannelControls(int chan, std::shared_ptr<SynthChannelControls> c) override;
					bool isPluginParent() override;

					MultiSynthControls();

				public:
					static std::string NAME();

				};

			}
		}
	}
}
