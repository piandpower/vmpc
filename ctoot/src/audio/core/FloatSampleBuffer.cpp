#include <audio/core/FloatSampleBuffer.hpp>

using namespace ctoot::audio::core;
using namespace std;

FloatSampleBuffer::FloatSampleBuffer() 
{
	init_(0, 0, 1);
}


FloatSampleBuffer::FloatSampleBuffer(int channelCount, int sampleCount, float sampleRate)
{
	init_(channelCount, sampleCount, sampleRate, LAZY_DEFAULT);
}

FloatSampleBuffer::FloatSampleBuffer(vector<char> buffer, int offset, int byteCount, AudioFormat* format)
{
	FloatSampleBuffer(format->getChannels(), byteCount / (format->getSampleSizeInBits() / 8 * format->getChannels()), format->getSampleRate());
	initFromByteArray_(buffer, offset, byteCount, format);
}

void FloatSampleBuffer::init_(int channelCount, int sampleCount, float sampleRate)
{
    init_(channelCount, sampleCount, sampleRate, LAZY_DEFAULT);
}

void FloatSampleBuffer::init_(int channelCount, int sampleCount, float sampleRate, bool lazy)
{
	if (channelCount < 0 || sampleCount < 0) {
		string error = "invalid parameters in initialization of FloatSampleBuffer.";
		printf("ERROR: %s", error.c_str());
		return;
	}
	setSampleRate(sampleRate);
	if (getSampleCount() != sampleCount || getChannelCount() != channelCount) {
		createChannels(channelCount, sampleCount, lazy);
	}
}

void FloatSampleBuffer::createChannels(int channelCount, int sampleCount, bool lazy)
{
	this->sampleCount = sampleCount;
	this->channelCount = 0;
	channels.clear();
	for (auto ch = 0; ch < channelCount; ch++) {
		insertChannel(ch, false, lazy);
	}
	if (!lazy) {
		while (channels.size() > channelCount) {
			channels.erase(channels.begin() + (int) (channels.size()) - 1);
		}
	}
}

void FloatSampleBuffer::initFromByteArray_(vector<char> buffer, int offset, int byteCount, AudioFormat* format)
{
	initFromByteArray_(buffer, offset, byteCount, format, LAZY_DEFAULT);
}

void FloatSampleBuffer::initFromByteArray_(vector<char> buffer, int offset, int byteCount, AudioFormat* format, bool lazy)
{

	if (offset + byteCount > buffer.size()) {
		string error = "FloatSampleBuffer.initFromByteArray: buffer too small.";
		printf("ERROR: %s", error.c_str());
		return;
	}
	auto thisSampleCount = byteCount / format->getFrameSize();
	init_(format->getChannels(), thisSampleCount, format->getSampleRate(), lazy);
	originalFormatType = FloatSampleTools::getFormatType(format);
	FloatSampleTools::byte2float(buffer, offset, &channels, 0, sampleCount, format);
}

void FloatSampleBuffer::initFromFloatSampleBuffer(FloatSampleBuffer* source)
{
	init_(source->getChannelCount(), source->getSampleCount(), source->getSampleRate());
	for (auto ch = 0; ch < getChannelCount(); ch++) {
		for (int i = 0; i < sampleCount; i++) {
			getChannel(ch)[i] = source->getChannel(ch)[i];
		}
	}
}


void FloatSampleBuffer::reset()
{
    init_(0, 0, 1, false);
}

void FloatSampleBuffer::reset(int channels, int sampleCount, float sampleRate)
{
	init_(channels, sampleCount, sampleRate, false);
}

int FloatSampleBuffer::getByteArrayBufferSize(AudioFormat* format)
{
    return getByteArrayBufferSize(format, getSampleCount());
}

int FloatSampleBuffer::getByteArrayBufferSize(AudioFormat* format, int lenInSamples)
{
    return format->getFrameSize() * lenInSamples;
}

int FloatSampleBuffer::convertToByteArray_(vector<char>* buffer, int offset, AudioFormat* format)
{
    return convertToByteArray_(0, getSampleCount(), buffer, offset, format);
}

int FloatSampleBuffer::convertToByteArray_(int readOffset, int lenInSamples, vector<char>* buffer, int writeOffset, AudioFormat* format)
{
	int byteCount = getByteArrayBufferSize(format, lenInSamples);
	if (writeOffset + byteCount > buffer->size()) {
		string error = "FloatSampleBuffer.convertToByteArray: buffer too small.\n";
		printf("ERROR: %s", error.c_str());
		return -1;
	}
	if (format->getSampleRate() != getSampleRate()) {
		string error = "FloatSampleBuffer.convertToByteArray: different samplerates.\n";
		printf("ERROR: %s", error.c_str());
		return -1;
	}
	if (format->getChannels() != getChannelCount()) {
		string error = "FloatSampleBuffer.convertToByteArray: different channel count.\n";
		return -1;
	}
	FloatSampleTools::float2byte(channels, readOffset, buffer, writeOffset, lenInSamples, format, getConvertDitherBits(FloatSampleTools::getFormatType(format)));
	return byteCount;
}

vector<char> FloatSampleBuffer::convertToByteArray_(AudioFormat* format)
{
	auto res = vector<char>(getByteArrayBufferSize(format));
	convertToByteArray_(&res, 0, format);
	return res;
}

void FloatSampleBuffer::changeSampleCount(int newSampleCount, bool keepOldSamples)
{
	auto oldSampleCount = getSampleCount();
	if (oldSampleCount == newSampleCount) {
		return;
	}
	vector<vector<float>> oldChannels;
	if (keepOldSamples) {
		vector<vector<float>>* allChannels(getAllChannels());
		for (int i = 0; i < getAllChannels()->size(); i++) {
			vector<float> currentVector((*allChannels)[i]);
			for (int j = 0; j < currentVector.size(); j++)
				oldChannels[i][j] = currentVector[j];
		}
	}

	init_(getChannelCount(), newSampleCount, getSampleRate());
	if (keepOldSamples) {
		auto copyCount = newSampleCount < oldSampleCount ? newSampleCount : oldSampleCount;
		for (auto ch = 0; ch < getChannelCount(); ch++) {
			vector<float> oldSamples = oldChannels[ch];
			vector<float> newSamples = *getChannel(ch);
			if (oldSamples != newSamples) {
				for (int i = 0; i < copyCount; i++)
					newSamples[i] = oldSamples[i];
			}
			if (oldSampleCount < newSampleCount) {
				for (auto i = oldSampleCount; i < newSampleCount; i++) {
					newSamples[i] = 0.0f;
				}
			}
		}
	}
}

void FloatSampleBuffer::makeSilenceFrames(int nFrames)
{
	if (getChannelCount() > 0) {
		makeSilence(0, nFrames);
		for (int ch = 1; ch < getChannelCount(); ch++) {
			//copyChannel(0, ch);
			makeSilence(ch, nFrames);
		}
	}
}

void FloatSampleBuffer::makeSilence(int channel, int nFrames)
{
	vector<float>* samples = getChannel(channel);
	for (int i = 0; i < nFrames; i++) {
		(*samples)[i] = 0;
	}
	//channels[channel] = vector<float>(getSampleCount());
}

void FloatSampleBuffer::addChannel(bool silent)
{
    insertChannel(getChannelCount(), silent);
}


void FloatSampleBuffer::insertChannel(int index, bool silent)
{
    insertChannel(index, silent, LAZY_DEFAULT);
}

void FloatSampleBuffer::insertChannel(int index, bool silent, bool lazy)
{
	int physSize = channels.size();
	int virtSize = getChannelCount();
	vector<float> newChannel;
	if (physSize > virtSize) {
		for (int ch = virtSize; ch < physSize; ch++) {
			auto thisChannel = channels[ch];
			if ((lazy && thisChannel.size() >= getSampleCount()) || (!lazy && thisChannel.size() == getSampleCount())) {
				newChannel = thisChannel;
				channels.erase(channels.begin() + ch);
				break;
			}
		}
	}
	if (newChannel.size() == 0) {
		for (int i = 0; i < getSampleCount(); i++)
			newChannel.push_back(0);
	}
	channels.push_back(newChannel);
	this->channelCount++;
	if (silent) {
		makeSilence(index, getSampleCount());
	}
}

void FloatSampleBuffer::removeChannel(int channel)
{
    removeChannel(channel, LAZY_DEFAULT);
}

void FloatSampleBuffer::removeChannel(int channel, bool lazy)
{
	if (!lazy) {
		channels.erase(channels.begin() + channel);
	}
	else if (channel < getChannelCount() - 1) {
		vector<float> candidate = channels[channel];
		channels.erase(channels.begin() + channel);
		channels.push_back(candidate);
	}
	channelCount--;
}

void FloatSampleBuffer::copyChannel(int sourceChannel, int targetChannel)
{
	vector<float> source = *getChannel(sourceChannel);
	vector<float> target = *getChannel(targetChannel);
	target = source;
}

void FloatSampleBuffer::copy(int sourceIndex, int destIndex, int length)
{
	for (auto i = 0; i < getChannelCount(); i++) {
		copy(i, sourceIndex, destIndex, length);
	}
}

void FloatSampleBuffer::copy(int channel, int sourceIndex, int destIndex, int length)
{
	auto data = getChannel(channel);
	auto bufferCount = getSampleCount();
	if (sourceIndex + length > bufferCount || destIndex + length > bufferCount || sourceIndex < 0 || destIndex < 0 || length < 0) {
		string error = "parameters exceed buffer size";
		printf("ERROR: %s", error.c_str());
	}
	for (int i = 0; i < length; i++) {
		data[destIndex + i] = data[sourceIndex + i];
	}
}

void FloatSampleBuffer::expandChannel(int targetChannelCount)
{
	if (getChannelCount() != 1) {
		string error = "FloatSampleBuffer: can only expand channels for mono signals.";
		printf("ERROR: %s", error.c_str());
		return;
	}
	for (auto ch = 1; ch < targetChannelCount; ch++) {
		addChannel(false);
		copyChannel(0, ch);
	}
}

void FloatSampleBuffer::mixDownChannels()
{
	auto firstChannel = getChannel(0);
	auto sampleCount = getSampleCount();
	auto channelCount = getChannelCount();
	for (auto ch = channelCount - 1; ch > 0; ch--) {
		auto thisChannel = getChannel(ch);
		for (auto i = 0; i < sampleCount; i++) {
			(*firstChannel)[i] += (*thisChannel)[i];
		}
		removeChannel(ch);
	}
}

void ctoot::audio::core::FloatSampleBuffer::setSamplesFromBytes(vector<char> input, int inByteOffset, AudioFormat* format, int floatOffset, int frameCount)
{
	if (floatOffset < 0 || frameCount < 0 || inByteOffset < 0) {
		//        throw new ::java::lang::IllegalArgumentException(u"FloatSampleBuffer.setSamplesFromBytes: negative inByteOffset, floatOffset, or frameCount"_j);
		return;
	}
	if (inByteOffset + (frameCount * format->getFrameSize()) > sizeof(input)) {
		//      throw new ::java::lang::IllegalArgumentException(u"FloatSampleBuffer.setSamplesFromBytes: input buffer too small."_j);
		return;
	}
	if (floatOffset + frameCount > getSampleCount()) {
		//    throw new ::java::lang::IllegalArgumentException(u"FloatSampleBuffer.setSamplesFromBytes: frameCount too large"_j);
		return;
	}
	FloatSampleTools::byte2float(input, inByteOffset, &channels, floatOffset, frameCount, format);
}

int FloatSampleBuffer::getChannelCount()
{
	return channelCount;
}

int FloatSampleBuffer::getSampleCount()
{
	return sampleCount;
}

float FloatSampleBuffer::getSampleRate()
{
    return sampleRate;
}
void FloatSampleBuffer::setSampleRate(float sampleRate)
{
	if (sampleRate <= 0) {
		string error = "Invalid samplerate for FloatSampleBuffer.";
		printf("ERROR: %s", error.c_str());
		return;
	}
	this->sampleRate = sampleRate;
}

vector<float>* FloatSampleBuffer::getChannel(int channel)
{
	if (channel < 0 || channel >= getChannelCount()) {
		string error = "FloatSampleBuffer: invalid channel number.";
		printf("ERROR: %s", error.c_str());
		return nullptr;
	}
	return &channels[channel];
}

vector<vector<float>>* FloatSampleBuffer::getAllChannels()
{
	return &channels;
}

void FloatSampleBuffer::setDitherBits(float ditherBits)
{
	if (ditherBits <= 0) {
		string error = "DitherBits must be greater than 0";
		printf("ERROR: %s", error.c_str());
		return;
	}
	this->ditherBits = ditherBits;
}

float FloatSampleBuffer::getDitherBits()
{
    return ditherBits;
}

void FloatSampleBuffer::setDitherMode(int mode)
{
	if (mode != DITHER_MODE_AUTOMATIC && mode != DITHER_MODE_ON && mode != DITHER_MODE_OFF) {
		string error = "Illegal DitherMode";
		printf("ERROR: %s", error.c_str());
		return;
	}
	this->ditherMode = mode;
}

int FloatSampleBuffer::getDitherMode()
{
    return ditherMode;
}

float FloatSampleBuffer::getConvertDitherBits(int newFormatType)
{
	auto doDither = false;
	switch (ditherMode) {
	case DITHER_MODE_AUTOMATIC:
		doDither = (originalFormatType & FloatSampleTools::F_SAMPLE_WIDTH_MASK) > (newFormatType & FloatSampleTools::F_SAMPLE_WIDTH_MASK);
		break;
	case DITHER_MODE_ON:
		doDither = true;
		break;
	case DITHER_MODE_OFF:
		doDither = false;
		break;
	}
	return doDither ? ditherBits : 0.0f;
}

FloatSampleBuffer::~FloatSampleBuffer() {
}
