#include <audio/core/AudioControlsChain.hpp>
#include <audio/core/AudioControls.hpp>
//#include <audio/core/Descriptors.hpp>
//#include <audio/core/AudioServices.hpp>
#include <audio/core/ChannelFormat.hpp>
//#include <audio/spi/AudioControlServiceDescriptor.hpp>
//#include <misc/Localisation.hpp>
//#include <service/ServiceDescriptor.hpp>


using namespace ctoot::audio::core;
//using namespace ctoot::audio::spi;
using namespace std;

AudioControlsChain::AudioControlsChain(int id, string name) : CompoundControlChain(id, name)
{
}


AudioControlsChain::AudioControlsChain(int id, int index, string name, ChannelFormat* constraintFormat) : CompoundControlChain(id, index, name)
{
	constraintChannelFormat = constraintFormat;
}

void AudioControlsChain::setMetaInfo(weak_ptr<MetaInfo> wMetaInfo)
{
	/*
	auto metaInfo = wMetaInfo.lock();
	if(metaInfo != nullptr) {
		sourceLabel = metaInfo->getSourceLabel();
		sourceLocation = metaInfo->getSourceLocation();
    } else {
		sourceLabel = "";
        sourceLocation = "";
    }
    setChanged();
	notifyObservers();
	*/
}

string ctoot::audio::core::AudioControlsChain::getSourceLabel()
{
    return sourceLabel;
}

string ctoot::audio::core::AudioControlsChain::getSourceLocation()
{
    return sourceLocation;
}


ChannelFormat* ctoot::audio::core::AudioControlsChain::getConstraintChannelFormat()
{
    return constraintChannelFormat;
}


ctoot::control::CompoundControl* AudioControlsChain::createControl(string name)
{
//    return AudioServices::createControls(name);
	return nullptr;
}

int ctoot::audio::core::AudioControlsChain::getMaxInstance()
{
    return 1024 - 1;
}

string ctoot::audio::core::AudioControlsChain::getPersistenceDomain()
{
	return "audio";
}

AudioControlsChain::~AudioControlsChain() {
	constraintChannelFormat = nullptr;
}
