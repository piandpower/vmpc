#pragma once
#include <observer/Observer.hpp>
#include <string>
#include <vector>
#include <memory>

#include <boost/any.hpp>

namespace moduru {

	namespace observer {

		class Observable
		{

		public:
			virtual void notifyObservers();
			virtual void notifyObservers(boost::any a);
			void setChanged();
			void addObserver(moduru::observer::Observer* o);
			void deleteObservers();
			void deleteObserver(moduru::observer::Observer* o);
			int countObservers();
			
		public:
			Observable();

		protected:
			bool changed{ false };

		private:
			std::vector<moduru::observer::Observer*> observers{};

		};
	}
}
