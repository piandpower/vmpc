#pragma once
#include <audio/mixer/MixerInterconnection.hpp>
#include <audio/server/AudioServer.hpp>

namespace mpc {
	namespace ctootextensions {

		class MpcMixerInterconnection
			: public virtual ctoot::audio::mixer::MixerInterconnection
		{

		public:
			ctoot::audio::core::AudioProcess* inputProcess{ nullptr };
			ctoot::audio::core::AudioProcess* outputProcess{ nullptr };

		private:
			bool leftEnabled{ true };
			bool rightEnabled{ true };

		public:
			ctoot::audio::core::AudioProcess* getInputProcess();// override;
			ctoot::audio::core::AudioProcess* getOutputProcess();// override;
			virtual void setLeftEnabled(bool b);
			virtual void setRightEnabled(bool b);

		public:
			MpcMixerInterconnection(std::string name, ctoot::audio::server::AudioServer* server);
			~MpcMixerInterconnection();

		private:
			friend class InterconnectionInputProcess;

		};
	}
}
