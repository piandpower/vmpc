#include <file/all/BarList.hpp>

#include <file/all/Bar.hpp>
#include <sequencer/Sequence.hpp>

#include <VecUtil.hpp>

using namespace mpc::file::all;
using namespace std;

BarList::BarList(vector<char> loadBytes)
{
	Bar* previousBar = nullptr;
	for (int i = 0; i < 999; i++) {
		auto bar = new Bar(moduru::VecUtil::CopyOfRange(&loadBytes, i * 4, (i * 4) + 4), previousBar);
		if (bar->lastTick == 0) break;
		bars.push_back(bar);
		previousBar = bar;
	}
}

BarList::BarList(mpc::sequencer::Sequence* seq)
{
	saveBytes = vector<char>(3996);
	auto barLengths = seq->getBarLengths();
	auto ticksPerBeat = 0;
	auto lastTick = 0;
	for (int i = 0; i < seq->getLastBar() + 1; i++) {
		lastTick += (*barLengths)[i];
		ticksPerBeat = static_cast<int>((*barLengths)[i] / seq->getNumerator(i));
		auto bar = new Bar(ticksPerBeat, lastTick);
		for (auto j = 0; j < 4; j++)
			saveBytes[(i * 4) + j] = bar->getBytes()[j];

	}
	auto bar = new Bar(ticksPerBeat, 0);
	for (int i = 0; i < 4; i++)
		saveBytes[(seq->getLastBar() + 1) * 4 + i] = bar->getBytes()[i];

}

vector<Bar*> BarList::getBars()
{
    return bars;
}


vector<char> BarList::getBytes()
{
    return saveBytes;
}
