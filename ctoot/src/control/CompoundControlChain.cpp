#include <control/CompoundControlChain.hpp>
#include <control/CompoundControl.hpp>
#include <control/ChainMutation.hpp>
#include <control/Control.hpp>
#include <algorithm>

using namespace ctoot::control;
using namespace std;

CompoundControlChain::CompoundControlChain(int id, string name) : CompoundControl(id, name)
{
}

CompoundControlChain::CompoundControlChain(int id, int index, string name) : CompoundControl(id, index, name)
{
}

void CompoundControlChain::add(shared_ptr<CompoundControl> control)
{
	if (find(control->getName()) != nullptr) {
		disambiguate(control.get());
	}
	super::add(control);
}

void CompoundControlChain::add(int index, shared_ptr<CompoundControl> control)
{
	controls.insert(controls.begin() + index, control);
	control->setParent(this);
}

void CompoundControlChain::insert(string insertName, string insertBeforeName)
{

	auto controlToInsert = createControl(insertName);
	if (controlToInsert == nullptr) {
		string error = getName() + ": insert failed to create " + insertName;
		printf("%s", error.c_str());
		return;
	}
	if (find(insertName) != nullptr)
		disambiguate(controlToInsert);
	insert(dynamic_cast<Control*>(controlToInsert), insertBeforeName);
}

void CompoundControlChain::insert(Control* controlToInsert, string insertBeforeName)
{
	auto insertionIndex = controls.size();
	auto controlToInsertBefore = find(insertBeforeName);
	if (controlToInsertBefore == nullptr) {
		string error1 = getName() + ": insert " + controlToInsert->getName() + ", " + insertBeforeName + " not found to insert before";
		printf("%s", error1.c_str());
		return;
	}
	if (dynamic_cast<CompoundControl*>(controlToInsertBefore)->canBeInsertedBefore()) {
		string error2 = getName() + ": insert " + controlToInsert->getName() + " before " + insertBeforeName + " note allowed";
		printf("%s", error2.c_str());
		return;
	}
	for (int i = 0; i < controls.size(); i++) {
		if (controls[i].get() == controlToInsertBefore) insertionIndex = i;
	}
	controls.insert(controls.begin() + insertionIndex, shared_ptr<Control>(controlToInsert));
	controlToInsert->setParent(this);
	ChainMutation* newccccm = new ChainMutation(ChainMutation::INSERT, (int)(insertionIndex));
	setChanged();
	notifyObservers(newccccm);
}

CompoundControl* CompoundControlChain::createControl(string name)
{
	return nullptr;
}


void CompoundControlChain::delete_(string deleteName)
{
	auto controlToDelete = find(deleteName);
	int index;
	for (index = 0; index < controls.size(); index++)
		if (controls[index].get() == controlToDelete) {
			break;
		}
	remove(shared_ptr<Control>(controlToDelete));
	setChanged();
	notifyObservers(new ChainMutation(ChainMutation::DELETE, index));
}

void CompoundControlChain::delete_(int indexToDelete)
{
	remove(controls[indexToDelete]);
	setChanged();
	notifyObservers(new ChainMutation(ChainMutation::DELETE, indexToDelete));
}

void CompoundControlChain::move(string moveName, string moveBeforeName)
{
	auto controlToMove = find(moveName);
	auto controlToMoveBefore = find(moveBeforeName);
	if (controlToMove == nullptr || controlToMoveBefore == nullptr) return;
	int indexToMove = 0;
	for (indexToMove = 0; indexToMove < controls.size(); indexToMove++)
		if (controls[indexToMove].get() == controlToMove) {
			break;
		}
	remove(shared_ptr<Control>(controlToMove));
	int insertionIndex = 0;
	for (insertionIndex = 0; insertionIndex < controls.size(); insertionIndex++) {
		if (controls[insertionIndex].get() == controlToMoveBefore) {
			break;
		}
	}
	controls.insert(controls.begin() + insertionIndex, shared_ptr<Control>(controlToMove));
	setChanged();
	notifyObservers(new ChainMutation(ChainMutation::MOVE, indexToMove, insertionIndex));
}

void CompoundControlChain::setMutating(bool mutating)
{
	setChanged();
	notifyObservers(mutating ? ChainMutation::COMMENCE_INSTANCE() : ChainMutation::COMPLETE_INSTANCE());
}


bool CompoundControlChain::isPluginParent()
{
    return true;
}

CompoundControlChain::~CompoundControlChain() {
}
