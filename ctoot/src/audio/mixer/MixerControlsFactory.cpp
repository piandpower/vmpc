#include <audio/mixer/MixerControlsFactory.hpp>

#include <audio/core/ChannelFormat.hpp>
#include <audio/mixer/BusControls.hpp>
#include <audio/mixer/MainMixControls.hpp>
#include <audio/mixer/MixControls.hpp>
#include <audio/mixer/FxBusMasterMixControls.hpp>
#include <audio/mixer/MixerControlsIds.hpp>
#include <audio/mixer/PostFadeMixControls.hpp>
#include <control/CompoundControl.hpp>

using namespace ctoot::audio::mixer;
using namespace ctoot::audio::core;
using namespace ctoot::control;
using namespace std;

MixerControlsFactory::MixerControlsFactory()
{
}

void MixerControlsFactory::createBusses(weak_ptr<MixerControls> mixerControls, int nsends, int naux)
{
	auto lMixerControls = mixerControls.lock();
	string base = "FX";	
	for (int i = 0; i < nsends; i++) {
		string res = base + "#" + to_string(i + 1);
		lMixerControls->createFxBusControls(res, ChannelFormat::STEREO());
	}
    base = "Aux";
	for (int i = 0; i < naux; i++) {
		string res = base + "#" + to_string(i + 1);
		lMixerControls->createAuxBusControls(res, ChannelFormat::STEREO());
	}
}

void MixerControlsFactory::createBusStrips(weak_ptr<MixerControls> mixerControls)
{
	createBusStrips(mixerControls, "L-R", ChannelFormat::STEREO(), (int)(mixerControls.lock()->getFxBusControls().size()));
}

void MixerControlsFactory::createBusStrips(weak_ptr<MixerControls> mixerControls, string mainStripName, ChannelFormat* mainFormat, int nreturns)
{
	auto lMixerControls = mixerControls.lock();
	lMixerControls->createStripControls(MixerControlsIds::MAIN_STRIP, 0, mainStripName, mainFormat);
	auto auxControls = lMixerControls->getAuxBusControls();
	int naux = auxControls.size();
	for (int i = 0; i < naux; i++) {
		auto busControls = auxControls[i];
		lMixerControls->createStripControls(MixerControlsIds::AUX_STRIP, i, busControls->getName(), false, busControls->getChannelFormat());
	}
	auto fxControlsList = lMixerControls->getFxBusControls();
	int nsends = fxControlsList.size();
	for (int i = 0; i < nsends; i++) {
		auto busControls = fxControlsList[i];
		lMixerControls->createStripControls(MixerControlsIds::FX_STRIP, i, busControls->getName(), i < nreturns, busControls->getChannelFormat());
	}
}

void MixerControlsFactory::createGroupStrips(weak_ptr<MixerControls> mixerControls, int ngroups)
{
	auto lMixerControls = mixerControls.lock();
	auto mbc = lMixerControls->getMainBusControls().lock();
    auto mainFormat = mbc->getChannelFormat();
    for (auto i = 0; i < ngroups; i++) {
        lMixerControls->createStripControls(MixerControlsIds::GROUP_STRIP, i, string("A").append(to_string(i)), mainFormat);
    }
}

void MixerControlsFactory::createChannelStrips(weak_ptr<MixerControls> mixerControls, int nchannels)
{

	auto mbc = mixerControls.lock()->getMainBusControls().lock();
	auto mainFormat = mbc->getChannelFormat();
	for (int i = 0; i < nchannels; i++) {
		mixerControls.lock()->createStripControls(MixerControlsIds::CHANNEL_STRIP, i, to_string(1 + i), mainFormat);
	}
}

void MixerControlsFactory::addMixControls(weak_ptr<MixerControls> mixerControls, weak_ptr<AudioControlsChain> controls, bool hasMixControls)
{
	auto lMixerControls = mixerControls.lock();
	auto lControls = controls.lock();
	int stripId = lControls->getId();
	if (stripId == MixerControlsIds::FX_STRIP) {
		auto busControls = lMixerControls->getBusControls(lControls->getName()).lock();
		if (busControls) {
			lControls->add(make_shared<FxBusMasterMixControls>(hasMixControls, mixerControls, stripId, busControls, true));
		}
	}
	else if (stripId == MixerControlsIds::AUX_STRIP) {
		auto busControls = lMixerControls->getBusControls(lControls->getName()).lock();
		if (busControls) {
			lControls->add(make_shared<MixControls>(mixerControls, stripId, busControls, true));
		}
	}
	auto mainMixControls = make_shared<MainMixControls>(mixerControls, stripId, lMixerControls->getMainBusControls().lock(), stripId == MixerControlsIds::MAIN_STRIP);
	if (lMixerControls->getMainBusControls().lock()->getId() == MixerControlsIds::MAIN_BUS) {
		auto routeControl = mainMixControls->createRouteControl(stripId);
		if (routeControl != nullptr) {
			mainMixControls->add(shared_ptr<Control>(routeControl));
		}
	}

	if (stripId != MixerControlsIds::MAIN_STRIP && stripId != MixerControlsIds::AUX_STRIP) {
		for (int i = 0; i < lMixerControls->getAuxBusControls().size(); i++) {
			auto busControls = lMixerControls->getAuxBusControls()[i];
			if (hasMixControls) {
				lControls->add(make_shared<MixControls>(mixerControls, stripId, busControls, false));
			}
		}
		if (stripId != MixerControlsIds::FX_STRIP) {
			for (int i = 0; i < lMixerControls->getFxBusControls().size(); i++) {
				auto busControls = lMixerControls->getFxBusControls()[i];
				lControls->add(make_shared<PostFadeMixControls>(mixerControls, stripId, busControls, mainMixControls.get()));
			}
		}
	}

	if (hasMixControls) {
		lControls->add(mainMixControls);
	}
}
