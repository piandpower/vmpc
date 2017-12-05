#include "Wave.hpp"

#include <Util.hpp>

#include <math/Math.hpp>

#include <gui/Bressenham.hpp>

#include <cmath>

using namespace mpc::lcdgui;
using namespace moduru::gui;
using namespace std;

Wave::Wave()
{
	initRect();
}

void Wave::setFine(bool fine) {
	this->fine = fine;
	if (fine) {
		width = 109;
	}
	else {
		width = 245;
	}
	initRect();
}

void Wave::zoomPlus() {
	if (zoomFactor == 7) return;
	zoomFactor++;
	initSamplesPerPixel();
	SetDirty();
}

void Wave::zoomMinus() {
	if (zoomFactor == 1) return;
	zoomFactor--;
	initSamplesPerPixel();
	SetDirty();
}

void Wave::initSamplesPerPixel() {
	if (fine) {
		samplesPerPixel = 1;
		for (int i = 1; i < zoomFactor; i++) {
			samplesPerPixel *= 2;
		}
	}
	else {
		samplesPerPixel = (float)frames / (float)width;
	}
}

void Wave::setCenterSamplePos(unsigned int centerSamplePos) {
	this->centerSamplePos = centerSamplePos;
	SetDirty();
}

void Wave::initRect() {
	//int xoff = Constants::LCD_RECT()->L;
	//int yoff = Constants::LCD_RECT()->T;
	if (fine) {
		//mRECT = IRECT(xoff + 46, yoff + 32, xoff + 46 + 218, yoff + 32 + 74);
	}
	else {
		//mRECT = IRECT(xoff + 2, yoff + 42, xoff + 2 + 490, yoff + 42 + 54);
	}
}

void Wave::setSampleData(vector<float>* sampleData, bool mono, unsigned int view) {
	this->sampleData = sampleData;
	if (sampleData == nullptr) return;
	this->mono = mono;
	this->view = view;
	frames = mono ? sampleData->size() : (sampleData->size() / 2);
	initSamplesPerPixel();
	SetDirty();
}

void Wave::setSelection(unsigned int start, unsigned int end) {
	selectionStart = start;
	selectionEnd = end;
	if (!mono && view == 1) {
		selectionStart += frames;
		selectionEnd += frames;
	}
	SetDirty();
}

void Wave::makeLine(std::vector<std::vector<std::vector<int> > >* lines, std::vector<bool>* colors, unsigned int x) {
	int offset = 0;
	float peakPos = 0;
	float peakNeg = 0;
	int centerSamplePixel = 0;
	if (fine) {
		//offset += centerSamplePos;
		centerSamplePixel = centerSamplePos / samplesPerPixel;
	}
	int samplePos = (int)(floor((float) (x - (fine ? (54 - centerSamplePixel) : 0)) * samplesPerPixel));
	offset += samplePos;
	
	if (!mono && view == 1) offset += frames;

	if (offset < 0 || offset >= sampleData->size()) return;
	if (!mono && view == 0 && offset > frames) return;
	if (!mono && view == 1 && offset < frames) return;

	float sample;
	for (int i = 0; i < (floor)(samplesPerPixel); i++) {
		sample = sampleData->at(offset++);
		if (sample > 0) {
			peakPos = moduru::math::Math::maxf(peakPos, sample);
		}
		else if (sample < 0) {
			peakNeg = moduru::math::Math::minf(peakNeg, sample);
		}
	}

	lines->clear();
	const float invisible = 1214.0 / 32768.0;
	const float ratio = 1.0f / (1.0f - invisible);

	const unsigned int posLineLength = (unsigned int) (floor(13.0 * ((peakPos - invisible) * ratio)));
	const unsigned int negLineLength = (unsigned int)(floor(13.0 * ((abs(peakNeg) - invisible) * ratio)));

	if (posLineLength != 13) {
		lines->push_back(Bressenham::Line(x, 0, x, 13 - (posLineLength + 1)));
	}

	if (peakPos > invisible) {
		lines->push_back(Bressenham::Line(x, (13 - posLineLength) - 1, x, 13 - 1));
	}

	if (abs(peakNeg) > invisible) {
		lines->push_back(Bressenham::Line(x, 13, x, 13 + negLineLength));
	}

	if (negLineLength != 13) {
		lines->push_back(Bressenham::Line(x, 13 + (negLineLength + 1), x, 26));
	}

	colors->clear();

	if (samplePos >= selectionStart && samplePos < selectionEnd && !fine) {
		if (posLineLength != 13) {
			colors->push_back(true);
		}
		if (peakPos > invisible) {
			colors->push_back(false);
		}
		if (abs(peakNeg) > invisible) {
			colors->push_back(false);
		}
		if (negLineLength != 13) {
			colors->push_back(true);
		}
	}
	else {
		if (posLineLength != 13) {
			colors->push_back(false);
		}
		if (peakPos > invisible) {
			colors->push_back(true);
		}
		if (abs(peakNeg) > invisible) {
			colors->push_back(true);
		}
		if (negLineLength != 13) {
			colors->push_back(false);
		}
	}
}

void Wave::Draw(std::vector<std::vector<bool> >* pixels) {
/*
if (sampleData == nullptr) return false;
	vector<vector<vector<int>>> lines;
	vector<IColor*> colors;
	vector<int> offsetxy{ mRECT.L, mRECT.T };
	for (int i = 0; i < width; i++) {
		if (i == 55 && fine) continue;
		makeLine(&lines, &colors, i);
		Util::drawScaled(g, lines, 2, colors, offsetxy);
	}
	*/
	dirty = false;
}

Wave::~Wave() {
}
