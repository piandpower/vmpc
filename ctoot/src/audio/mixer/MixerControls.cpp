#include <audio/mixer/MixerControls.hpp>

#include <control/id/ProviderId.hpp>
#include <audio/mixer/BusControls.hpp>
#include <audio/mixer/Mutation.hpp>
#include <audio/core/ChannelFormat.hpp>

//#include <audio/id/ProviderId.hpp>
#include <audio/mixer/GainControl.hpp>
#include <audio/mixer/MixerControlsFactory.hpp>
#include <audio/mixer/MixerControlsIds.hpp>
#include <control/CompoundControl.hpp>
#include <control/Control.hpp>

#include <iostream>

using namespace ctoot::audio::mixer;
using namespace ctoot::audio::core;
using namespace ctoot::audio::fader;
using namespace std;

MixerControls::MixerControls(string name, string mainBusName, ChannelFormat* channelFormat) : CompoundControl(1, name)
{
	mainBusControls = make_shared<BusControls>(MixerControlsIds::MAIN_BUS, mainBusName, channelFormat);
}

int MixerControls::getProviderId()
{
    return control::id::ProviderId::TOOT_PROVIDER_ID;
}

float MixerControls::getSmoothingFactor()
{
    return smoothingFactor;
}

shared_ptr<BusControls> MixerControls::createFxBusControls(string name, ChannelFormat* format)
{
	if (!canAddBusses) {
		string error = "Can't add busses after adding strips";
		printf("ERROR: %s", error.c_str());
	}
	auto mainFormat = mainBusControls->getChannelFormat();
	if (format == nullptr) {
		format = mainFormat;
	}
	else if (format->getCount() > mainFormat->getCount()) {
		format = mainFormat;

		string error = name.append(" Bus limited to Main Bus channel format");
		printf("ERROR: %s", error.c_str());
	}
	auto busControls = make_shared<BusControls>(MixerControlsIds::FX_BUS, name, format);
	fxBusControls.push_back(busControls);
	return busControls;
}

shared_ptr<BusControls> MixerControls::createAuxBusControls(string name, ChannelFormat* format)
{
	if (!canAddBusses) {
		//		printf("%s", string("Can't add busses after adding strips").c_str());
		return nullptr;
	}
	auto busControls = make_shared<BusControls>(MixerControlsIds::AUX_BUS, name, format);
	auxBusControls.push_back(busControls);
	return busControls;
}

weak_ptr<BusControls> MixerControls::getBusControls(string name)
{
	auto mbc = mainBusControls;
	
	//std::cout << "bus controls name: " << name << std::endl;
	//std::cout << "mbc getName: " << mbc->getName() << std::endl;

	auto mbcName = mbc->getName();

	if (std::mismatch(mbcName.begin(), mbcName.end(), name.begin()).first == mbcName.end()) {
		return mainBusControls;
	}

	for (int i = 0; i < fxBusControls.size(); i++) {
		auto busControls = fxBusControls[i];
		if (busControls->getName().compare(name) == 0) {
			return busControls;
		}
	}
	for (int i = 0; i < auxBusControls.size(); i++) {
		auto busControls = auxBusControls[i];
		if (busControls->getName().compare(name) == 0) {
			return busControls;
		}
	}
	return weak_ptr<BusControls>();
}

weak_ptr<BusControls> MixerControls::getMainBusControls()
{
	return mainBusControls;
}

vector<shared_ptr<BusControls>> MixerControls::getFxBusControls()
{
	return fxBusControls;
}

vector<shared_ptr<BusControls>> MixerControls::getAuxBusControls()
{
	return auxBusControls;
}

weak_ptr<AudioControlsChain> MixerControls::createStripControls(int id, int index, string name)
{
    return createStripControls(id, index, name, true, ChannelFormat::STEREO());
}

weak_ptr<AudioControlsChain> MixerControls::createStripControls(int id, int index, string name, ChannelFormat* constraintFormat)
{
    return createStripControls(id, index, name, true, constraintFormat);
}

weak_ptr<AudioControlsChain> MixerControls::createStripControls(int id, int index, string name, bool hasMixControls, ChannelFormat* constraintFormat)
{
	if (getStripControls(name) != nullptr) {
		string error = name.append(" already exists");
		printf("ERROR: %s", error.c_str());
		return weak_ptr<AudioControlsChain>();
	}
	auto chain = make_shared<AudioControlsChain>(id, index, name, constraintFormat);
	MixerControlsFactory::addMixControls(shared_from_this(), chain, hasMixControls);
	addStripControls(chain);
	return chain;
}

void MixerControls::addStripControls(shared_ptr<CompoundControl> cc)
{
	canAddBusses = false;
	add(cc);
	//auto mcm = new Mutation(Mutation::ADD, cc);
	//setChanged();
	//notifyObservers(mcm);
}

void MixerControls::removeStripControls(shared_ptr<CompoundControl> cc)
{
    remove(cc);
    setChanged();
    notifyObservers(new Mutation(Mutation::REMOVE, cc));
}

void MixerControls::removeStripControls(string name)
{
	auto cc = getStripControls(name);
	if (cc != nullptr) {
		removeStripControls(cc);
	}
}

void MixerControls::moveStripControls(string name, string beforeName)
{
	auto cc = getStripControls(name);
	auto bc = getStripControls(beforeName);
	if (cc != nullptr && bc != nullptr) {
		remove(cc);
		int bi;
		for (bi = 0; bi < controls.size(); bi++) {
			auto currentControl = controls[bi];
			if (currentControl == bc) {
				break;
			}
		}
		controls.insert(controls.begin() + bi, cc);
		setChanged();
		notifyObservers(cc);
	}
}

shared_ptr<AudioControlsChain> MixerControls::getStripControls(string name)
{
	string size = to_string(getControls().size());
	for (int i = 0; i < (int)(getControls().size()); i++) {
		auto c = getControls()[i];
		if (c->getName().compare(name) == 0) {
			try {
				return dynamic_pointer_cast<AudioControlsChain>(c);
			}
			catch (bad_cast e) {
				//printf("\nMixerControls.getStripControls bad cast: %s\n", e.what());
			}
		}
	}
	return nullptr;
}

shared_ptr<AudioControlsChain> MixerControls::getStripControls(int id, int index)
{
	for (int i = 0; i < (int)(getControls().size()); i++) {
		auto c = getControls()[i];
		if (c->getId() == id && dynamic_pointer_cast<AudioControlsChain>(c)->getInstanceIndex() == index) {
			return dynamic_pointer_cast<AudioControlsChain>(c);
		}
	}
	return nullptr;
}


FaderControl* MixerControls::createFaderControl(bool muted)
{
    return new GainControl(muted);
}

vector<string> MixerControls::opNames()
{
    return opNames_;
}
vector<string> MixerControls::opNames_ = vector<string>({ "n/a", "Add", "Remove" });

MixerControls::~MixerControls() {
	for (auto& a : auxBusControls) {
		a.reset();
	}
	for (auto& f : fxBusControls) {
		f.reset();
	}
	mainBusControls.reset();
}
