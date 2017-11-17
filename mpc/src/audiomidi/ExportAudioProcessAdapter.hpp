#pragma once
#include <audio/core/AudioProcessAdapter.hpp>
#include <audio/core/AudioProcess.hpp>

#include <fstream>
#include <thread>

#include <io/CircularBuffer.hpp>

namespace moduru {
	namespace io {
		class FileOutputStream;
	}
	namespace file {
		class File;
	}
}

namespace mpc {
	namespace audiomidi {

		class ExportAudioProcessAdapter
			: public ctoot::audio::core::AudioProcessAdapter

		{

		public:
			typedef ctoot::audio::core::AudioProcessAdapter super;

		public:
			std::string name{ "" };
			std::unique_ptr<moduru::io::CircularBuffer> circularBuffer{};
			std::weak_ptr<ctoot::audio::core::AudioFormat> format{ };
			bool reading{ false };
			bool writing{ false };
			moduru::file::File* file{ nullptr };

		private:
			moduru::io::FileOutputStream* fos1{ nullptr };
			moduru::io::FileOutputStream* fos2{ nullptr };
			std::thread* writeThread{ nullptr };
			std::fstream raf{};
			int lengthInBytes{};

			static void static_eapa(void * args);

		public:
			void prepare(moduru::file::File* file, int lengthInFrames);
			void start();
			int processAudio(ctoot::audio::core::AudioBuffer* buf, int nFrames) override;
			virtual void stop();
			void run();
			void writeWav();

			ExportAudioProcessAdapter(ctoot::audio::core::AudioProcess* process, std::weak_ptr<ctoot::audio::core::AudioFormat> format, std::string name);
			~ExportAudioProcessAdapter();

		};

	}
}
