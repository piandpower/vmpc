#pragma once
#include <string>

#include <boost/any.hpp>

namespace moduru {

	namespace observer {

		class Observable;

		class Observer {

		public:
			virtual void update(std::string s) {}
			virtual void update(Observable* obs, boost::any arg) {}

		public:
			virtual ~Observer() {}
		};

	}
}
