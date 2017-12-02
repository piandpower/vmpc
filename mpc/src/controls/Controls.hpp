#pragma once

#include <unordered_map>
#include <string>

namespace mpc {

	class Mpc;

	namespace controls {

		class AbstractControls;
		class GlobalReleaseControls;

		class Controls
		{

		private:
			std::unordered_map<std::string, controls::AbstractControls*> controls{};

		public:
			mpc::controls::AbstractControls* getControls(std::string s);
			mpc::controls::GlobalReleaseControls* getReleaseControls();

		public:
			Controls(Mpc* mpc);
			~Controls();
		};
	}
}
