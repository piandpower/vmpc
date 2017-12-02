#pragma once

#include <memory>
#include <string>

namespace mpc {

	class Mpc;

	namespace file {
		namespace aps {

			class ApsSaver
			{

			private:
				mpc::Mpc* mpc{ nullptr };
				std::string apsFileName{""};

			private:
				void saveAps();

			public:
				ApsSaver(mpc::Mpc* mpc, std::string apsFileName);

			};

		}
	}
}
