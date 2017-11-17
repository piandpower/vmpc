#pragma once
#include <audio/fader/FaderControl.hpp>

namespace mpc {
	namespace ctootextensions {

		class MpcFaderControl
			: public ctoot::audio::fader::FaderControl
		{

		private:
			static ctoot::control::ControlLaw mpcFaderLaw_;
			//static ctoot::control::ControlLaw*& mpcFaderLaw();

		public:
			void setValue(float value) override;

			MpcFaderControl(bool muted);
			~MpcFaderControl();

		};

	}
}
