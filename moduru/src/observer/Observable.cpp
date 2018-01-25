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
	string observerClass = typeid(*o).name();
	string observableClass = typeid(*this).name();
	//if (observableClass.compare("class mpc::sampler::NoteParameters") == 0)
		//MLOG(observableClass + ": observer added of class " + observerClass);
	for (auto& obs : observers)
		if (obs == o) return;
	observers.push_back(o);
}

void Observable::deleteObserver(moduru::observer::Observer* o) {
	string observerClass = typeid(*o).name();
	string observableClass = typeid(*this).name();
	//if (observableClass.compare("class mpc::sampler::NoteParameters") == 0)
		//MLOG("\nAttempting to remove from: " + observableClass + " an observer of class " + observerClass);
	//int before = observers.size();
	int counter = 0;
	for (auto& o1 : observers) {
		if (o1 == o) {
			observers.erase(observers.begin() + counter);
			break;
		}
		counter++;
	}
	//if (before == observers.size()) {
//		MLOG("No observers actually removed!");
	//}
}

void Observable::deleteObservers() {
	observers.clear();
}
