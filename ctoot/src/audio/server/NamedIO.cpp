#include "NamedIO.hpp"

using namespace std;
using namespace ctoot::audio::server;

NamedIO::NamedIO(string name, int first, int count) {
	this->name = name;
	this->first = first;
	this->count = count;
}
