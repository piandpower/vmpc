#include <observer/Observable.hpp>

#include <Logger.hpp>

using namespace moduru::observer;
using namespace std;

Observable::Observable() {
	changed = true;
}

void Observable::setChanged() {
	changed = true;
}

int Observable::countObservers() {
	return (int)(observers.size());
}

void Observable::notifyObservers() {
	notifyObservers(nullptr);
}

void Observable::notifyObservers(boost::any arg) {
	for (auto& a : observers)
		a->update(this, arg);
}

void Observable::addObserver(moduru::observer::Observer* o) {
	for (auto& obs : observers)
		if (obs == o) return;
	observers.push_back(o);
}

void Observable::deleteObserver(moduru::observer::Observer* o) {
	int counter = 0;
	for (auto& o1 : observers) {
		if (o1 == o) {
			observers.erase(observers.begin() + counter);
			break;
		}
		counter++;
	}
}

void Observable::deleteObservers() {
	observers.clear();
}
