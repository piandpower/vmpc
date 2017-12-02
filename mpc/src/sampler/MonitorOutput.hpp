#pragma once

#include <audio/system/AudioOutput.hpp>
#include <audio/core/AudioBuffer.hpp>

#include <string>

namespace mpc {

	namespace sampler {

		class Sampler;

		class MonitorOutput
			: public virtual ctoot::audio::system::AudioOutput
		{

		private:
			Sampler* sampler{ nullptr };

		public: 
			bool closed{ false };

		public:
			std::string getName() override;
			void open() override;
			int32_t processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames) override;
			void close() override;
			std::string getLocation() override;

		public:
			MonitorOutput(Sampler* sampler);
			~MonitorOutput();

		};

	}
}
