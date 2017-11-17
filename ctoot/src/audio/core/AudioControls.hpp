#pragma once
#include <control/CompoundControl.hpp>
#include <control/BooleanControl.hpp>
#include <control/BypassControl.hpp>

namespace ctoot {
	namespace audio {
		namespace core {

			class AudioControls
				: public control::CompoundControl
			{

			public:
				typedef control::CompoundControl super;

			private:
                ctoot::control::BypassControl* bypassControl{ nullptr };

			public:
				virtual bool hasOrderedFrequencies();
				virtual bool canBypass();
				virtual void setBypassed(bool state);
				virtual bool isBypassed();
				virtual control::BooleanControl* getBypassControl();

			public:
				virtual bool canBeMinimized() override;
				virtual std::string getPersistenceDomain() override;

			public:
				AudioControls(int id, std::string name, int bypassId);
				AudioControls(int id, std::string name);
				virtual ~AudioControls();

			};
		}
	}
}
