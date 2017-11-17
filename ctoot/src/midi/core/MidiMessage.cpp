#include <midi/core/MidiMessage.hpp>

using namespace ctoot::midi::core;
using namespace std;

MidiMessage::MidiMessage() {
}

MidiMessage::MidiMessage(vector<char> data) {
	this->data = data;
	this->length = data.size();
}

MidiMessage* MidiMessage::clone() {
	MidiMessage* result;
	return result;
}

int MidiMessage::getLength() {
	return (int)(data.size());
}

vector<char>* MidiMessage::getMessage() {
	return &data;
}

int MidiMessage::getStatus() {
	return data[0] & 0xFF;
}

void MidiMessage::setMessage(vector<char> data, int length) {
	this->data = data;
}