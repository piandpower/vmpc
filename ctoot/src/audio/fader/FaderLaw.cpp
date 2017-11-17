#include <audio/fader/FaderLaw.hpp>

#include <algorithm>

using namespace ctoot::audio::fader;
using namespace std;

FaderLaw::FaderLaw(int resolution, float halfdB, float maxdB, float attenuationCutoffFactor) 
{
	this->resolution = resolution;
	this->halfdB = halfdB;
	this->maxdB = maxdB;
	this->attenuationCutoffFactor = attenuationCutoffFactor;
}

FaderLaw::FaderLaw(int resolution) 
{
	this->resolution = resolution;
}

/*
FaderLaw* FaderLaw::SEMI_LOG()
{
    return new FaderLaw(1024, -10.0f, 10.0f, 0.33f);
}

FaderLaw* FaderLaw::LOG()
{
    return new FaderLaw(1024, -20.0f, 15.0f, 0.2f);
}

FaderLaw* FaderLaw::BROADCAST()
{
    return new FaderLaw(1024, -30.0f, 15.0f, 0.125f);
}

float FaderLaw::ATTENUATION_CUTOFF()
{
    return 100.0f;
}

FaderLaw* FaderLaw::defaultLaw()
{
    return new FaderLaw(1024, -30.0f, 15.0f, 0.125f);
}
*/

int FaderLaw::getResolution()
{
    return resolution;
}

float FaderLaw::getMaximum()
{
    return maxdB;
}

float FaderLaw::getMinimum()
{
    return -100.0f;
}

string FaderLaw::getUnits()
{
	return "dB";
}

int FaderLaw::intValue(float v)
{
    if(floatValues.size() == 0) {
        floatValues = *createFloatValues();
    }

    if(v <= floatValues[0]) return 0;

    if(v >= floatValues[resolution - 1]) return resolution - 1;

    auto ret = binarySearch(floatValues, v, 0, 100);
    if(ret >= 0) return ret;

    return -(++ret);
}

int FaderLaw::binarySearch(vector<float> buf, float key, int min, int max)
{
	float mid;
	while (max >= min) {
		mid = (min + max) / 2;

		if (buf[mid] < key)
			min = mid + 1;
		else if (buf[mid] > key)
			max = mid - 1;

		else
			return mid;

	}
	return std::min(min, max);
}

float FaderLaw::userValue(int v)
{
	if (floatValues.size() == 0) {
		floatValues = *createFloatValues();
	}
	if (v < 0)
		return floatValues[0];

	if (v > resolution - 1)
		return floatValues[resolution - 1];

	return floatValues[v];
}

vector<float>* FaderLaw::createFloatValues()
{
	auto vals = new vector < float > ;
    for (auto i = 0; i < resolution; i++) {
        (*vals)[i] = calculateFloatValue(i);
    }
    return vals;
}

float FaderLaw::calculateFloatValue(int v)
{
	auto val = static_cast<float>(v) / (resolution - 1);
	val = maxdB + (2 * halfdB * (1 - val));
	auto linZerodB = maxdB + (2 * halfdB);
	auto cutoff = static_cast<int>((attenuationCutoffFactor * (resolution - 1)));
	if (v < cutoff) {
		auto attenFactor = static_cast<float>((cutoff - v)) / cutoff;
		attenFactor *= attenFactor;
		val -= (100.0f + linZerodB) * attenFactor;
	}
	return val;
}