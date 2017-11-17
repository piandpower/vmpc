#include <audio/mixer/Mutation.hpp>
#include <audio/mixer/MixerControls.hpp>

using namespace std;
using namespace ctoot::audio::mixer;

Mutation::Mutation(int operation, shared_ptr<ctoot::control::CompoundControl> control) 
{
	this->operation = operation;
	this->control = control;
}

const int Mutation::ADD;

const int Mutation::REMOVE;

int Mutation::getOperation()
{
    return operation;
}

shared_ptr<ctoot::control::CompoundControl> Mutation::getControl()
{
    return control;
}

string Mutation::toString()
{
	string result = ctoot::audio::mixer::MixerControls::opNames()[operation].append(" ").append(control->getName());
	return result;
}

Mutation::~Mutation() {
}
