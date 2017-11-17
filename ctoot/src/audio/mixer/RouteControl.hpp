#pragma once
#include <control/EnumControl.hpp>
#include <audio/mixer/MainMixControls.hpp>

#include <boost/any.hpp>

namespace ctoot {
	namespace audio {
		namespace mixer {

			class RouteControl
				: public EnumControl
			{

			private:
				MainMixControls* mmc;
				bool canRouteToGroups{};

			public:
				std::vector<boost::any> getValues();
				
			public:
				RouteControl(MainMixControls* mmc, std::string defaultRoute, bool canRouteToGroups);

				friend class MainMixControls;
			};

		}
	}
}
