#include <midi/core/LegacyDeviceAdaptor.hpp>

#include <midi/core/LegacyMidiInput.hpp>
#include <midi/core/LegacyMidiOutput.hpp>

using namespace ctoot::midi::core;
using namespace std;

LegacyDeviceAdaptor::LegacyDeviceAdaptor(string name, bool isInput)
	: super(name)
{
	name = simpleName(name);
    if (isInput) {
        input = new LegacyMidiInput(this, name);
        addMidiInput(input);
    }
	else {
        output = new LegacyMidiOutput(this, name);
        addMidiOutput(output);
    }
}

string  LegacyDeviceAdaptor::simpleName(string name)
{
	/*
    auto parts = name->split("\\s"_j);
    if(parts == nullptr)
        return name;

    if(name->startsWith("Microsoft "_j)) {
        return name->substring(npc("Microsoft "_j)->length(), name->length());
    }
    return name;
	*/
	return name;
}

void LegacyDeviceAdaptor::open()
{
	if (!isOpen()) {
		//device->open();
		//input->open();
	}
}

void LegacyDeviceAdaptor::closeMidi()
{
	if (input != nullptr) {
		input->closeInput();
		removeMidiInput(input);
		input = nullptr;
	}
	else if (output != nullptr) {
		output->closeOutput();
		removeMidiOutput(output);
		output = nullptr;
	}
}

bool LegacyDeviceAdaptor::isOpen()
{
	return input != nullptr || output != nullptr;
}

string  LegacyDeviceAdaptor::getProvider()
{
    //return device->getDeviceInfo()->getVendor();
	return "<tmp_provider>";
}

string  LegacyDeviceAdaptor::getDescription()
{
    //return device->getDeviceInfo()->getDescription();
	return "<tmp_description>";
}
