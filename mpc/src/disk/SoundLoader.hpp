#pragma once

#include <memory>
#include <vector>

namespace moduru {
	namespace file {
		class File;
	}
}

namespace mpc {
	
	namespace maingui {
		class Gui;
	}

	namespace file {
		namespace wav {
			class WavFile;
		}
	}

	namespace sampler {
		class Sound;
	}

	namespace disk {

		class MpcFile;

		class SoundLoader
		{

		private:
			std::weak_ptr<mpc::maingui::Gui> gui{};
			static float rateToTuneBase;

			bool partOfProgram{ false };
			MpcFile* soundFile{ nullptr };
			std::vector<std::weak_ptr<mpc::sampler::Sound>> sounds{};
			bool preview{ false };
			bool replace{ false };
			int size{ 0 };

		public:
			static std::vector<float> getSampleDataFromWav(std::weak_ptr<moduru::file::File> wavFile);
			static void getSampleDataFromWav(std::weak_ptr<moduru::file::File> wavFile, std::vector<float>* dest);

			void setPartOfProgram(bool b);
			int loadSound(MpcFile* f);
			void setPreview(bool b);
			double logOfBase(float num, float base);
			int getSize();

			SoundLoader(std::weak_ptr<mpc::maingui::Gui> gui);
			SoundLoader(std::weak_ptr<mpc::maingui::Gui> gui, std::vector<std::weak_ptr<mpc::sampler::Sound>> sounds, bool replace);

		};

	}
}
