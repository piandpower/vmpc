#include <audio/core/AudioFormat.hpp>

#include <audio/system/AudioSystem.hpp>

#include <cmath>

using namespace ctoot::audio::core;
using namespace std;

AudioFormat::AudioFormat(float sampleRate, int sampleSizeInBits, int channels, bool signed_, bool bigEndian) {
	
	encoding = signed_ ? Encoding::PCM_SIGNED() : Encoding::PCM_UNSIGNED();
	frameSize = (sampleSizeInBits + 7) / 8 * channels;
	this->sampleRate = sampleRate;
	this->frameRate = sampleRate;
	this->sampleSizeInBits = sampleSizeInBits;
	this->channels = channels;
	this->bigEndian = bigEndian;
}

AudioFormat::AudioFormat(Encoding* encoding, float sampleRate, int sampleSizeInBits, int channels, int frameSize, float frameRate, bool bigEndian) {
	this->sampleRate = sampleRate;
	this->sampleSizeInBits = sampleSizeInBits;
	this->channels = channels;
	this->bigEndian = bigEndian;
	this->encoding = encoding;
	this->frameSize = frameSize;
	this->frameRate = frameRate;

}

int AudioFormat::getChannels()
{
	return channels;
}

Encoding* AudioFormat::getEncoding()
{
	return encoding;
}

float AudioFormat::getFrameRate()
{
	return frameRate;
}


int AudioFormat::getFrameSize()
{
	return frameSize;
}

float AudioFormat::getSampleRate()
{
	return sampleRate;
}

int AudioFormat::getSampleSizeInBits()
{
	return sampleSizeInBits;
}

bool AudioFormat::isBigEndian()
{
	return bigEndian;
}


bool AudioFormat::matches(AudioFormat* fmt)
{
	if (encoding != fmt->encoding
		|| channels != fmt->channels
		|| sampleSizeInBits != fmt->sampleSizeInBits
		|| frameSize != fmt->frameSize)
		return false;
	if (sampleRate != NOT_SPECIFIED
		&& fmt->sampleRate != NOT_SPECIFIED
		&& sampleRate != fmt->sampleRate)
		return false;
	if (frameRate != NOT_SPECIFIED
		&& fmt->frameRate != NOT_SPECIFIED
		&& frameRate != fmt->frameRate)
		return false;
	if (sampleSizeInBits > 8)
		return bigEndian == fmt->bigEndian;
	return true;
}

string AudioFormat::toString()
{
	string sEncoding = "";
	if (getEncoding() != nullptr) {
		sEncoding = getEncoding()->toString() + " ";
	}

	string sSampleRate;
	if (getSampleRate() == (float)-1) {
		sSampleRate = "unknown sample rate, ";
	}
	else {
		sSampleRate = "" + to_string(getSampleRate()) + " Hz, ";
	}

	string sSampleSizeInBits;
	if (getSampleSizeInBits() == (float)-1) {
		sSampleSizeInBits = "unknown bits per sample, ";
	}
	else {
		sSampleSizeInBits = "" + to_string(getSampleSizeInBits()) + " bit, ";
	}

	string sChannels;
	if (getChannels() == 1) {
		sChannels = "mono, ";
	}
	else
		if (getChannels() == 2) {
			sChannels = "stereo, ";
		}
		else {
			if (getChannels() == -1) {
				sChannels = " unknown number of channels, ";
			}
			else {
				sChannels = "" + to_string(getChannels()) + " channels, ";
			}
		}

		string sFrameSize;
		if (getFrameSize() == (float)-1) {
			sFrameSize = "unknown frame size, ";
		}
		else {
			sFrameSize = "" + to_string(getFrameSize()) + " bytes/frame, ";
		}

		string sFrameRate = "";
		if (abs(getSampleRate() - getFrameRate()) > 0.00001) {
			if (getFrameRate() == (float)-1) {
				sFrameRate = "unknown frame rate, ";
			}
			else {
				sFrameRate = to_string(getFrameRate()) + " frames/second, ";
			}
		}

		string sEndian = "";
		if ((getEncoding()->toString().compare(Encoding::PCM_SIGNED()->toString()) == 0
			|| getEncoding()->toString().compare(Encoding::PCM_UNSIGNED()->toString()) == 0)
			&& ((getSampleSizeInBits() > 8)
				|| (getSampleSizeInBits() == -1))) {
			if (isBigEndian()) {
				sEndian = "big-endian";
			}
			else {
				sEndian = "little-endian";
			}
		}

		return sEncoding
			+ sSampleRate
			+ sSampleSizeInBits
			+ sChannels
			+ sFrameSize
			+ sFrameRate
			+ sEndian;
}

AudioFormat::~AudioFormat() {

}
