#pragma once
#include <controls/sequencer/AbstractSequencerControls.hpp>

#include <memory>

namespace mpc {

	namespace sequencer {
		class Song;
	}

	namespace controls {
		namespace sequencer {

			class SongControls
				: public AbstractSequencerControls
			{

			public:
				typedef AbstractSequencerControls super;

			private:
				int step{};
				std::weak_ptr<mpc::sequencer::Song> s{};

			public:
				void init() override;

			public:
				void up() override;
				void openWindow() override;
				void down() override;
				void turnWheel(int i) override;
				void function(int i) override;

				SongControls(mpc::Mpc* mpc);

			};

		}
	}
}
