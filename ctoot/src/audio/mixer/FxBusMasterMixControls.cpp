#include <audio/mixer/FxBusMasterMixControls.hpp>

using namespace std;
using namespace ctoot::audio::mixer;

FxBusMasterMixControls::FxBusMasterMixControls(bool hasMixControls, weak_ptr<MixerControls> mixerControls, int stripId, weak_ptr<BusControls> busControls, bool isMaster) : MixControls(mixerControls, stripId, busControls, isMaster)
{
	this->hasMixControls = hasMixControls;
}

bool FxBusMasterMixControls::canBeInsertedBefore()
{
    return !hasMixControls;
}

bool FxBusMasterMixControls::canBeMovedBefore()
{
    return !hasMixControls;
}

bool FxBusMasterMixControls::canBeMoved()
{
    return !hasMixControls;
}
