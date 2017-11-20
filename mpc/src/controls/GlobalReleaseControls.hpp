#pragma once

#include <string>
#include <memory>

namespace mpc {
	class Mpc;
	namespace ctootextensions {
		class MpcSoundPlayerChannel;
	}

	namespace ui {
		namespace sampler {
			class SamplerGui;
		}
	}
	namespace sampler {
		class Sampler;
		class Program;
	}
	namespace sequencer {
		class Sequencer;
		class Track;
	}
	namespace controls {

		class GlobalReleaseControls
		{

		protected:
			mpc::ui::sampler::SamplerGui* samplerGui{ nullptr };
			std::string csn{ "" };
			mpc::Mpc* mpc{ nullptr };
			std::weak_ptr<mpc::sampler::Sampler> sampler{ };
			std::weak_ptr<mpc::sequencer::Sequencer> sequencer{};
			std::weak_ptr<mpc::sequencer::Track> track{};
			mpc::ctootextensions::MpcSoundPlayerChannel* mpcSoundPlayerChannel{ nullptr };
			std::weak_ptr<mpc::sampler::Program> program{};
			int bank{ 0 };
			virtual void init();

		public:
			void keyEvent(unsigned char c);
			void pad(int i);

		private:
			void generateNoteOff(int nn);

		public:
			virtual void overDub();
			virtual void rec();
			virtual void tap();
			virtual void shift();
			virtual void erase();

		public:
			GlobalReleaseControls(mpc::Mpc* mpc);
			virtual ~GlobalReleaseControls();

		};

	}
}
