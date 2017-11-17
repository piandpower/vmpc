#include <midi/core/DefaultConnectedMidiSystem.hpp>

#include <midi/core/MidiConnection.hpp>
#include <midi/core/MidiDevice.hpp>
#include <midi/core/MidiInput.hpp>
#include <midi/core/MidiOutput.hpp>
#include <midi/core/MidiPort.hpp>

using namespace ctoot::midi::core;
using namespace std;

DefaultConnectedMidiSystem::DefaultConnectedMidiSystem() 
{
}

void DefaultConnectedMidiSystem::notifyObservers()
{
    checkConnections();
    super::notifyObservers();
}

void DefaultConnectedMidiSystem::checkConnections()
{
	vector<MidiInput*> inputs = getMidiInputs();
	vector<MidiOutput*> outputs = getMidiOutputs();
	vector<MidiConnection*> remove;
	for (auto& connection : connections) {
		auto input = connection->getMidiInput();
		auto output = connection->getMidiOutput();
		if (find(inputs.begin(), inputs.end(), input) != inputs.end() && find(outputs.begin(), outputs.end(), output) != outputs.end())	continue;
		remove.push_back(connection);
	}
	for (int i = (int)(remove.size()) - 1; i >= 0; i--) {
		connections.erase(connections.begin() + i);
	}
}

vector<MidiConnection*> DefaultConnectedMidiSystem::getMidiConnections()
{
	return connections;
}

void DefaultConnectedMidiSystem::createMidiConnection(MidiOutput* from, MidiInput* to, int flags)
{
    auto connection = new MidiConnection(from, to, flags);
	connections.push_back(connection);
    setChanged();
    notifyObservers();
}

void DefaultConnectedMidiSystem::closeMidiConnection(MidiOutput* from, MidiInput* to)
{
    auto connection = getConnection(from, to);
	int i;
	for (i = 0; i < connections.size(); i++) {
		if (connections[i] == connection)
			break;
	}
	if (i >= connections.size()) return;
	connections.erase(connections.begin() + i);
    connection->close();
    setChanged();
    notifyObservers();
}

void DefaultConnectedMidiSystem::createMidiConnection(string* fromPortName, string* toPortName, int flags)
{
	auto from = dynamic_cast<MidiOutput*>(getPort(fromPortName, true));
	auto to = dynamic_cast<MidiInput*>(getPort(toPortName, false));
	if (from == nullptr) return;
	//        npc(::java::lang::System::err())->println(stringBuilder().append(fromPortName)->append(u" not found"_j)->toString());

	if (to == nullptr) return;
	//        npc(::java::lang::System::err())->println(stringBuilder().append(toPortName)->append(u" not found"_j)->toString());
	createMidiConnection(from, to, flags);
}

void DefaultConnectedMidiSystem::closeMidiConnection(string* fromPortName, string* toPortName)
{
	auto from = dynamic_cast<MidiOutput*>(getPort(fromPortName, true));
	auto to = dynamic_cast<MidiInput*>(getPort(toPortName, false));
	closeMidiConnection(from, to);
}

MidiConnection* DefaultConnectedMidiSystem::getConnection(MidiOutput* from, MidiInput* to)
{
	for (auto& c : connections) {
		if (c->getMidiOutput() == from && c->getMidiInput() == to) {
			return c;
		}
	}
	return nullptr;
	//    throw new ::java::lang::IllegalArgumentException(stringBuilder().append(u"MidiConnection from "_j)->append(npc(from)->getName())
	//        ->append(u" to "_j)
	//        ->append(npc(to)->getName())->toString());
}

MidiPort* DefaultConnectedMidiSystem::getPort(string* name, bool isOut)
{
	for (auto& device : getMidiDevices()) {
		vector<MidiPort*> ports = isOut ? vector<MidiPort*>(device->getMidiOutputs().begin(), device->getMidiOutputs().end()) : vector<MidiPort*>(device->getMidiInputs().begin(), device->getMidiInputs().end());
		for (auto& port : ports) {
			if (name->compare(port->getName()) == 0) {
				return port;
			}
		}
	}
	return nullptr;
}

DefaultConnectedMidiSystem::~DefaultConnectedMidiSystem() {
	connections.clear();
}
