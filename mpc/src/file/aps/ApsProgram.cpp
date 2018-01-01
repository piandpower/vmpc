#include <file/aps/ApsProgram.hpp>

#include <file/aps/ApsAssignTable.hpp>
#include <file/aps/ApsNoteParameters.hpp>
#include <file/aps/ApsSlider.hpp>
#include <file/pgmwriter/Pads.hpp>
#include <sampler/StereoMixerChannel.hpp>
#include <sampler/Program.hpp>

#include <lang/StrUtil.hpp>
#include <VecUtil.hpp>

using namespace mpc::file::aps;
using namespace moduru;
using namespace std;

ApsProgram::ApsProgram(vector<char> loadBytes) 
{
	index = loadBytes[INDEX_OFFSET];
	auto nameBytes = VecUtil::CopyOfRange(&loadBytes, NAME_OFFSET, NAME_OFFSET + NAME_LENGTH);
	name = "";
	for (char c : nameBytes) {
		if (c == 0x00) break;
		name.push_back(c);
	}

	slider = new ApsSlider(VecUtil::CopyOfRange(&loadBytes, SLIDER_OFFSET, SLIDER_OFFSET + SLIDER_LENGTH));
	for (int i = 0; i < 64; i++) {
		int offset = NOTE_PARAMETERS_OFFSET + (i * NOTE_PARAMETERS_LENGTH);
		noteParameters[i] = new ApsNoteParameters(VecUtil::CopyOfRange(&loadBytes, offset, offset + NOTE_PARAMETERS_LENGTH));
	}
	mixer = new ApsMixer(VecUtil::CopyOfRange(&loadBytes, MIXER_OFFSET, MIXER_END));
	assignTable = new ApsAssignTable(VecUtil::CopyOfRange(&loadBytes, ASSIGN_TABLE_OFFSET, ASSIGN_TABLE_OFFSET + ASSIGN_TABLE_LENGTH));
}

ApsProgram::ApsProgram(mpc::sampler::Program* program, int index)
{
	vector<vector<char>> byteList;
	this->index = index;
	byteList.push_back(vector<char>{ (char) index });
	byteList.push_back(UNKNOWN);
	string name = moduru::lang::StrUtil::padRight(program->getName(), " ", 16);
	for (char c : name) {
		byteList.push_back(vector<char>{ c });
	}
	byteList.push_back(vector<char>{ 0 });
	auto slider = new ApsSlider(program->getSlider());
	byteList.push_back(slider->getBytes());
	byteList.push_back(vector<char>{ 35, 64, 0, 26, 0 });
	for (int i = 0; i < 64; i++) {
		auto np = new ApsNoteParameters(program->getNoteParameters(i + 35));
		byteList.push_back(np->getBytes());
	}
	byteList.push_back(vector<char>{ 6 });
	auto smcs = vector<weak_ptr<mpc::sampler::StereoMixerChannel>>(64);
	auto ifmcs = vector<weak_ptr<mpc::sampler::IndivFxMixerChannel>>(64);
	for (int i = 0; i < 64; i++) {
		smcs[i] = program->getStereoMixerChannel(i);
		ifmcs[i] = program->getIndivFxMixerChannel(i);
	}

	auto mixer = new ApsMixer(smcs, ifmcs);
	byteList.push_back(mixer->getBytes());
	byteList.push_back(vector<char>{ 0, 64, 0 });
	auto assignTable = vector<int>(64);
	for (int i = 0; i < 64; i++) {
		assignTable[i] = program->getNoteFromPad(i);
	}

	auto table = new ApsAssignTable(assignTable);
	byteList.push_back(table->getBytes());
	byteList.push_back(mpc::file::pgmwriter::Pads::getFxBoardSettings());
	auto totalSize = 0;
	for (auto& ba : byteList) {
		totalSize += ba.size();
	}
	saveBytes = vector<char>(totalSize);
	auto counter = 0;
	for (auto& ba : byteList) {
		for (auto b : ba) {
			saveBytes[counter++] = b;
		}
	}
}

const int ApsProgram::NAME_OFFSET;
const int ApsProgram::NAME_LENGTH;
const int ApsProgram::PADDING0_LENGTH;
const int ApsProgram::SLIDER_OFFSET;
const int ApsProgram::SLIDER_LENGTH;
const int ApsProgram::PADDING1_LENGTH;
const int ApsProgram::NOTE_PARAMETERS_OFFSET;
const int ApsProgram::NOTE_PARAMETERS_LENGTH;
const int ApsProgram::PADDING2_LENGTH;
const int ApsProgram::MIXER_OFFSET;
const int ApsProgram::MIXER_END;
const int ApsProgram::PADDING3_LENGTH;
const int ApsProgram::ASSIGN_TABLE_OFFSET;
const int ApsProgram::ASSIGN_TABLE_LENGTH;

ApsNoteParameters* ApsProgram::getNoteParameters(int note)
{
    return noteParameters[note - 35];
}

ApsMixer* ApsProgram::getMixer()
{
    return mixer;
}

ApsAssignTable* ApsProgram::getAssignTable()
{
    return assignTable;
}

ApsSlider* ApsProgram::getSlider()
{
    return slider;
}

string ApsProgram::getName()
{
    return name;
}

vector<char> ApsProgram::getBytes()
{
    return saveBytes;
}

mpc::sampler::StereoMixerChannel* ApsProgram::getStereoMixerChannel(int note)
{
	return mixer->getStereoMixerChannel(note);
}

mpc::sampler::IndivFxMixerChannel* ApsProgram::getIndivFxMixerChannel(int note)
{
	return mixer->getIndivFxMixerChannel(note);
}
