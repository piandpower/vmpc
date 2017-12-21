#include <ctootextensions/Voice.hpp>

#include <sampler/NoteParameters.hpp>
#include <sampler/Sound.hpp>

#include <ctootextensions/MpcEnvelopeControls.hpp>
#include <ctootextensions/MpcEnvelopeGenerator.hpp>
#include <ctootextensions/MpcSoundPlayerChannel.hpp>
#include <ctootextensions/MuteInfo.hpp>
#include <audio/core/AudioBuffer.hpp>
#include <audio/core/ChannelFormat.hpp>
#include <control/BooleanControl.hpp>
#include <control/Control.hpp>
#include <control/FloatControl.hpp>
#include <synth/modules/filter/StateVariableFilter.hpp>
#include <synth/modules/filter/StateVariableFilterControls.hpp>

#include <cmath>

#include <file/File.hpp>

using namespace mpc::ctootextensions;
using namespace std;

Voice::Voice(int stripNumber, bool basic)
{
	EMPTY_FRAME = vector<float>{ 0.f, 0.f };
	tempFrame = vector<float>{ 0.f, 0.f };
	muteInfo = new mpc::ctootextensions::MuteInfo();
	this->stripNumber = stripNumber;
	this->basic = basic;
	staticEnvControls = new mpc::ctootextensions::MpcEnvelopeControls(0, "StaticAmpEnv", AMPENV_OFFSET);
	staticEnv = new mpc::ctootextensions::MpcEnvelopeGenerator(staticEnvControls);
	sattack = dynamic_pointer_cast<ctoot::control::FloatControl>(staticEnvControls->getControls()[ATTACK_INDEX]).get();
	shold = dynamic_pointer_cast<ctoot::control::FloatControl>(staticEnvControls->getControls()[HOLD_INDEX]).get();
	sdecay = dynamic_pointer_cast<ctoot::control::FloatControl>(staticEnvControls->getControls()[DECAY_INDEX]).get();

	if (!basic) {
		ampEnvControls = new mpc::ctootextensions::MpcEnvelopeControls(0, "AmpEnv", AMPENV_OFFSET);
		filterEnvControls = new mpc::ctootextensions::MpcEnvelopeControls(0, "StaticAmpEnv", AMPENV_OFFSET);
		ampEnv = new mpc::ctootextensions::MpcEnvelopeGenerator(ampEnvControls);
		filterEnv = new mpc::ctootextensions::MpcEnvelopeGenerator(filterEnvControls);
		svfControls = new ctoot::synth::modules::filter::StateVariableFilterControls(0, "Filter", SVF_OFFSET);
		svfControls->createControls();
		svf0 = new ctoot::synth::modules::filter::StateVariableFilter(svfControls);
		svf1 = new ctoot::synth::modules::filter::StateVariableFilter(svfControls);
		fattack = dynamic_pointer_cast<ctoot::control::FloatControl>(filterEnvControls->getControls()[ATTACK_INDEX]).get();
		fhold = dynamic_pointer_cast<ctoot::control::FloatControl>(filterEnvControls->getControls()[HOLD_INDEX]).get();
		fdecay = dynamic_pointer_cast<ctoot::control::FloatControl>(filterEnvControls->getControls()[DECAY_INDEX]).get();
		attack = dynamic_pointer_cast<ctoot::control::FloatControl>(ampEnvControls->getControls()[ATTACK_INDEX]).get();
		hold = dynamic_pointer_cast<ctoot::control::FloatControl>(ampEnvControls->getControls()[HOLD_INDEX]).get();
		decay = dynamic_pointer_cast<ctoot::control::FloatControl>(ampEnvControls->getControls()[DECAY_INDEX]).get();
		reso = dynamic_pointer_cast< ctoot::control::FloatControl>(svfControls->getControls()[RESO_INDEX]).get();
		mix = dynamic_pointer_cast< ctoot::control::FloatControl>(svfControls->getControls()[MIX_INDEX]).get();
		bandpass = dynamic_pointer_cast<ctoot::control::BooleanControl>(svfControls->getControls()[BANDPASS_INDEX]).get();
	}
}

const float Voice::TIME_RATIO{ 5.46f };
const float Voice::STATIC_ATTACK_LENGTH{ 10.92f };
const float Voice::STATIC_DECAY_LENGTH{ 109.2f };
const int Voice::MAX_ATTACK_LENGTH_MS;
const int Voice::MAX_DECAY_LENGTH_MS;
const int Voice::MAX_ATTACK_LENGTH_SAMPLES;
const int Voice::MAX_DECAY_LENGTH_SAMPLES;
const int Voice::ATTACK_INDEX;
const int Voice::HOLD_INDEX;
const int Voice::DECAY_INDEX;
const int Voice::RESO_INDEX;
const int Voice::MIX_INDEX;
const int Voice::BANDPASS_INDEX;
const int Voice::SVF_OFFSET;
const int Voice::AMPENV_OFFSET;

void Voice::init(
	int track,
	int velocity,
	int padNumber,
	weak_ptr<mpc::sampler::Sound> oscVars,
	mpc::sampler::NoteParameters* np,
	int varType,
	int varValue,
	int muteNote,
	int muteDrum,
	int frameOffset,
	bool enableEnvs)
{
    this->enableEnvs = enableEnvs;
    this->frameOffset = frameOffset;
    this->track = track;
	this->np = np;
	this->padNumber = padNumber;
	this->oscVars = oscVars;
	
	finished = false;
    readyToPlay = false;
	staticDecay = false;
	muteInfo->setNote(muteNote);
    muteInfo->setDrum(muteDrum);
    veloFactor = velocity / 127.0;
    veloToStart = 0;
    attackValue = 0;
    decayValue = 2;
    veloToAttack = 0;
    decayMode = 0;
    veloToLevel = 100;
	auto lOscVars = oscVars.lock();
    tune = lOscVars->getTune();

	if (np != nullptr) {
		tune += np->getTune();
		veloToStart = np->getVelocityToStart();
		attackValue = np->getAttack();
		decayValue = np->getDecay();
		veloToAttack = np->getVelocityToAttack();
		decayMode = np->getDecayMode();
		veloToLevel = np->getVeloToLevel();
	}
    switch (varType) {
    case 0:
        tune += (varValue - 64) * 2;
        break;
    case 1:
        decayValue = varValue;
        decayMode = 1;
        break;
    case 2:
        attackValue = varValue;
        break;
    }

	increment = pow(2.0, tune / 120.0);
	start = lOscVars->getStart() + (veloFactor * (veloToStart / 100.0) * lOscVars->getLastFrameIndex());
    end = lOscVars->getEnd();
	position = start;
	sampleData = lOscVars->getSampleData();
	playableSampleLength = lOscVars->isLoopEnabled() ? INT_MAX : (int) ((end - start) / increment);
    attackMs = (float)((attackValue / 100.0) * MAX_ATTACK_LENGTH_MS);
    attackMs += (float)((veloToAttack / 100.0) * MAX_ATTACK_LENGTH_MS * veloFactor);
    finalDecayValue = decayValue < 2 ? 2 : decayValue;
    decayMs = (float)((finalDecayValue / 100.0) * MAX_DECAY_LENGTH_MS);
    attackLengthSamples = (int)(attackMs * 44.1);
    decayLengthSamples = (int)(decayMs * 44.1);
	if (attackLengthSamples > MAX_ATTACK_LENGTH_SAMPLES) {
		attackLengthSamples = (int)(MAX_ATTACK_LENGTH_SAMPLES);
	}

	if (decayLengthSamples > MAX_DECAY_LENGTH_SAMPLES) {
		decayLengthSamples = MAX_DECAY_LENGTH_SAMPLES;
	}

    holdLengthSamples = playableSampleLength - attackLengthSamples - decayLengthSamples;
    staticEnvHoldSamples = (int)(playableSampleLength - (((STATIC_ATTACK_LENGTH + STATIC_DECAY_LENGTH) / TIME_RATIO) * 44.1));
    staticEnv->reset();
    sattack->setValue(STATIC_ATTACK_LENGTH);
    shold->setValue(staticEnvHoldSamples);
    sdecay->setValue(STATIC_DECAY_LENGTH);
    veloToLevelFactor = (float)((veloToLevel / 100.0));
    amplitude = (float)(((veloFactor * veloToLevelFactor) + 1.0f - veloToLevelFactor));
    amplitude *= (lOscVars->getSndLevel() / 100.0);
    if(!basic) {
        ampEnv->reset();
        attack->setValue(decayMode == 1 ? (float)(0) : attackMs * TIME_RATIO);
        hold->setValue(decayMode == 1 ? 0 : holdLengthSamples);
        decay->setValue(decayMs * TIME_RATIO);
        filtParam = np->getFilterFrequency();
        if(varType == 3) filtParam = varValue;

        initialFilterValue = (float)(((filtParam + (veloFactor * np->getVelocityToFilterFrequency()))));
        initialFilterValue = (float)((17.0 + (initialFilterValue * 0.75)));
        filterEnv->reset();
        fattack->setValue((float)((np->getFilterAttack() / 500.0) * MAX_ATTACK_LENGTH_SAMPLES));
        fhold->setValue(0);
        fdecay->setValue((float)((np->getFilterDecay() / 500.0) * MAX_DECAY_LENGTH_SAMPLES));
        reso->setValue((float)((1.0 / 16.0) + (np->getFilterResonance() / 26.0)));
        mix->setValue(0.0f);
        bandpass->setValue(false);
        svf0->update();
        svf1->update();
    }
    decayCounter = 0;
    readyToPlay = true;
}

vector<float> Voice::getFrame()
{
    if (!readyToPlay || finished) return EMPTY_FRAME;

    if (frameOffset > 0) {
        frameOffset--;
        return EMPTY_FRAME;
    }
    envAmplitude = basic ? 1.0f : ampEnv->getEnvelope(false);
    staticEnvAmp = enableEnvs ? staticEnv->getEnvelope(staticDecay) : 1.0f;
    envAmplitude *= staticEnvAmp;
	
    float filterEnvFactor = 0;
    float filterFreq = 0;
    if(!basic) {
        filterFreq = mpc::ctootextensions::MpcSoundPlayerChannel::midiFreq(initialFilterValue * 1.44f) * inverseNyquist;
        filterEnvFactor = (float)(filterEnv->getEnvelope(false) * (np->getFilterEnvelopeAmount() / 100.0));
        filterFreq += mpc::ctootextensions::MpcSoundPlayerChannel::midiFreq(144) * inverseNyquist * filterEnvFactor;
    }


	readFrame();

    if (oscVars.lock()->isMono()) {
	    tempFrame[0] *= envAmplitude * amplitude;
		if (!basic) {
			tempFrame[0] = svf0->filter(tempFrame[0], filterFreq);
		}
		tempFrame[1] = tempFrame[0];
	} else {
        tempFrame[0] *= envAmplitude * amplitude;
        tempFrame[1] *= envAmplitude * amplitude;
		if(!basic) {
            tempFrame[0] = svf0->filter(tempFrame[0], filterFreq);
            tempFrame[1] = svf1->filter(tempFrame[1], filterFreq);
        }
    }
    return tempFrame;
}

void Voice::readFrame() {
	if (oscVars.lock()->isLoopEnabled() && position > end - 1)
		position = start;

	if (position > end - 1 || (staticEnv != nullptr && staticEnv->isComplete()) || (ampEnv != nullptr && ampEnv->isComplete())) {
		tempFrame = EMPTY_FRAME;
		finished = true;
		return;
	}
	k = (int)(ceil(position));
	j = k - 1;
	if (j == -1) j = 0;

	frac = position - j;

	if (oscVars.lock()->isMono()) {
		tempFrame[0] = ((*sampleData)[j] * (1.0f - frac)) + ((*sampleData)[k] * frac);
	}
	else {
		auto rOffset = sampleData->size() / 2;
		tempFrame[0] = ((*sampleData)[j] * (1.0f - frac)) + ((*sampleData)[k] * frac);
		tempFrame[1] = ((*sampleData)[j + rOffset] * (1.0f - frac)) + ((*sampleData)[k + rOffset] * frac);
	}
	position += increment;
}

int Voice::getPadNumber()
{
    return padNumber;
}

void Voice::open()
{
}

int Voice::processAudio(ctoot::audio::core::AudioBuffer* buffer, int nFrames)
{
	buffer->makeSilence();
	if (finished) {
		return AUDIO_OK;
		// maybe should be AUDIO_SILENCE
	}
	processing = true;
	left = buffer->getChannel(0);
	right = buffer->getChannel(1);
	count = nFrames;
	for (int i = 0; i < count; i++) {
		frame = getFrame();
		(*left)[i] = frame[0];
		(*right)[i] = frame[1];

		if (decayCounter != 0) {
			if (decayCounter == 1) startDecay();
			decayCounter--;
		}
	}
	if (finished) {
		padNumber = -1;
		//if (parent != nullptr) {
		//	parent->kill(this);
		//}
	}
	processing = false;
	return AUDIO_OK;
}

bool Voice::isProcessing() {
	return processing;
}

bool Voice::isFinished() {
	return finished;
}

void Voice::take() {
	finished = false;
}

void Voice::close()
{
}

void Voice::finish() {
	finished = true;
}

void Voice::startDecay()
{
    staticDecay = true;
}

int Voice::getVoiceOverlap()
{
    return np->getVoiceOverlap();
}

int Voice::getStripNumber()
{
    return stripNumber;
}

bool Voice::isDecaying()
{
    return staticDecay;
}

mpc::ctootextensions::MuteInfo* Voice::getMuteInfo()
{
    return muteInfo;
}

void Voice::startDecay(int offset)
{
	if (offset > 0) {
		decayCounter = offset;
	}
	else {
		startDecay();
	}
}

Voice::~Voice() {
	delete muteInfo;
	delete staticEnvControls;
	delete staticEnv;

	if (!basic) {
		delete ampEnvControls;
		delete filterEnvControls;
		delete ampEnv;
		delete filterEnv;
		delete svfControls;
		delete svf0;
		delete svf1;
	}
}
