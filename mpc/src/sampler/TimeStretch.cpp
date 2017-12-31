#include "TimeStretch.hpp"

using namespace mpc::sampler;
using namespace std;

TimeStretch::TimeStretch(vector<float> sampleData, float ratio, int sampleRate) 
{
	oldBlockSize = int(1300);
	overlap = 0.1f;
	this->sampleRate = sampleRate;
	auto numberOfBlocks = static_cast< int >(ceil(sampleData.size() / oldBlockSize));
	if (ratio > 1) {
		overlap = 0.5f;
	}
	auto newBlockSizeCompress = static_cast< int >(oldBlockSize * (ratio + (ratio * overlap)));
	auto newBlockSizeExpand = static_cast< int >(oldBlockSize * ratio);
	auto fadeLengthCompress = static_cast< int >(oldBlockSize * ratio * overlap);
	fadeLengthExpand = static_cast< int >(oldBlockSize * overlap);
	int newLength = 0;
	if (ratio > 1) {
		newLength = static_cast< int >(numberOfBlocks * newBlockSizeExpand);
	}
	if (ratio < 1) {
		newLength = static_cast< int >(numberOfBlocks * (newBlockSizeCompress - fadeLengthCompress));
		newLength += fadeLengthCompress;
	}
	newSampleData = vector<float>(newLength);
	auto originalSegments = vector<vector<float>>();
	auto sampleCounter = int(0);
	auto block = vector<float>(oldBlockSize);
	for (auto f : sampleData) {
		block[sampleCounter++] = f;
		if (sampleCounter == oldBlockSize) {
			sampleCounter = 0;
			originalSegments.push_back(block);
			block = vector<float>(oldBlockSize);
		}
	}
	if (ratio < 1) {
		for (auto& fa : originalSegments) {
			auto ampCoEf = 0.0f;
			auto increment = static_cast<float>(1.0 / fadeLengthCompress);
			for (int i = 0; i < fadeLengthCompress; i++) {
				fa[i] *= ampCoEf;
				ampCoEf += increment;
			}
			increment -= (2.0 * increment);
			for (auto i = newBlockSizeCompress - fadeLengthCompress; i < newBlockSizeCompress; i++) {
				if (fa.size() > i) {
					fa[i] *= ampCoEf;
				}
				ampCoEf += increment;
			}
		}
		sampleCounter = 0;
		for (int j = 0; j < fadeLengthCompress; j++) {
			newSampleData[sampleCounter++] = originalSegments[0][j];
		}
		for (int i = 0; i < numberOfBlocks; i++) {
			auto fa = originalSegments[i];
			vector<float> nextFa;
			if (i != numberOfBlocks - 1) {
				nextFa = originalSegments[i + 1];
			}
			else {
				nextFa = vector<float>();
			}
			for (auto j = fadeLengthCompress; j < newBlockSizeCompress - fadeLengthCompress; j++) {
				newSampleData[sampleCounter++] = fa[j];
			}
			int nextFaCounter = 0;
			for (auto j = newBlockSizeCompress - fadeLengthCompress; j < newBlockSizeCompress; j++) {
				if (nextFa.size() != 0) {
					if (fa.size() > j && nextFa.size() > nextFaCounter) {
						newSampleData[sampleCounter++] = fa[j] + nextFa[nextFaCounter++];
					}
				}
				else {
					if (fa.size() > j) {
						newSampleData[sampleCounter++] = fa[j];
					}
				}
			}
		}
		processedData = newSampleData;
	}
	if (ratio > 1) {
		specialFadeLength = 0;
		int repetitions = floor(newBlockSizeExpand / (oldBlockSize - fadeLengthExpand));
		auto trimmedSegmentLength = newBlockSizeExpand + fadeLengthExpand - (repetitions * (oldBlockSize - fadeLengthExpand));
		auto totalNrOfRepetitions = repetitions;
		if (trimmedSegmentLength != 0)
			totalNrOfRepetitions++;

		vector<vector<float>> repeatedSegments;
		int segmentCounter = 0;
		for (auto& fa : originalSegments) {
			for (int i = 0; i < repetitions; i++) {
				auto faCopy = vector<float>(fa.size());
				sampleCounter = 0;
				for (auto f : fa) {
					faCopy[sampleCounter++] = f;
				}
				faCopy = fade(0.0f, 1.0f, 0, fadeLengthExpand, faCopy);
				faCopy = fade(1.0f, 0.0f, oldBlockSize - fadeLengthExpand, fadeLengthExpand, faCopy);
				repeatedSegments.push_back(faCopy);
				segmentCounter++;
			}
			sampleCounter = 0;
			auto faCopy = vector<float>(trimmedSegmentLength);
			for (auto f : fa) {
				faCopy[sampleCounter++] = f;
				if (sampleCounter == trimmedSegmentLength) {
					break;
				}
			}
			if (trimmedSegmentLength != 0) {
				auto trimmedSegmentStartIndex = repetitions * (oldBlockSize - fadeLengthExpand);
				specialFadeLength = fadeLengthExpand;
				specialFadeLength = newBlockSizeExpand - trimmedSegmentStartIndex;
				auto fadeToAmplitude = specialFadeLength / static_cast<float>(fadeLengthExpand);
				faCopy = fade(0, fadeToAmplitude, 0, specialFadeLength, faCopy);
				faCopy = fade(fadeToAmplitude, fadeToAmplitude, specialFadeLength, trimmedSegmentLength - (int(2) * specialFadeLength), faCopy);
				faCopy = fade(fadeToAmplitude, 0, trimmedSegmentLength - specialFadeLength, specialFadeLength, faCopy);
				repeatedSegments.push_back(faCopy);
				segmentCounter++;
			}
		}
		vector<vector<float>> consolidatedRepeatedSegments;
		for (int j = 0; j < numberOfBlocks; j++) {
			auto consolidated = vector<float>(newBlockSizeExpand + fadeLengthExpand);
			sampleCounter = 0;
			for (int k = 0; k < repetitions; k++) {
				vector<float> repeatedOriginalSegment = repeatedSegments[(j * totalNrOfRepetitions) + k];
				vector<float> nextSegment = repeatedSegments[(j * totalNrOfRepetitions) + k + 1];
				if (k == 0) {
					for (int i = 0; i < fadeLengthExpand; i++) {
						consolidated[sampleCounter++] = repeatedOriginalSegment[i];
					}
				}
				for (auto i = fadeLengthExpand; i < oldBlockSize - fadeLengthExpand; i++) {
					consolidated[sampleCounter++] = repeatedOriginalSegment[i];
				}
				auto nextSegmentCounter = int(0);
				for (auto i = oldBlockSize - fadeLengthExpand; i < oldBlockSize; i++) {
					consolidated[sampleCounter++] = repeatedOriginalSegment[i] + nextSegment[nextSegmentCounter++];
				}
			}
			if (trimmedSegmentLength != 0) {
				vector<float> lastSegment = repeatedSegments[(j * totalNrOfRepetitions) + (totalNrOfRepetitions - 1)];
				for (int i = 0; i < specialFadeLength; i++) {
					consolidated[sampleCounter++] = lastSegment[i + fadeLengthExpand];
				}
			}
			consolidatedRepeatedSegments.push_back(consolidated);
		}
		sampleCounter = 0;
		for (int i = 0; i < fadeLengthExpand; i++) {
			newSampleData[sampleCounter++] = consolidatedRepeatedSegments[0][i];
		}
		for (int i = 0; i < numberOfBlocks; i++) {
			vector<float> currentBlock = consolidatedRepeatedSegments[i];
			vector<float> nextBlock;
			if (consolidatedRepeatedSegments.size() > i + 1) {
				nextBlock = consolidatedRepeatedSegments[i + 1];
			}
			auto currentBlockCounter = fadeLengthExpand;
			for (int j = fadeLengthExpand; j < newBlockSizeExpand; j++) {
				newSampleData[sampleCounter++] = currentBlock[currentBlockCounter++];
			}
			for (int j = 0; j < fadeLengthExpand; j++) {
				if (nextBlock.size() != 0) {
					newSampleData[sampleCounter++] = currentBlock[currentBlockCounter++] + nextBlock[j];
				}
				else {
					if (sampleCounter < newSampleData.size()) {
						newSampleData[sampleCounter++] = currentBlock[currentBlockCounter++];
					}
				}
			}
		}
		processedData = newSampleData;
	}
}

vector<float> TimeStretch::getProcessedData()
{
    return processedData;
}

vector<float> TimeStretch::fade(float from, float to, int startIndex, int length, vector<float> fa)
{
	auto temp = vector<float>(fa.size());
    for (int i = 0; i < fa.size(); i++) {
        temp[i] = fa[i];
    }
    auto hannOffset = int(0);
    auto hannLength = fadeLengthExpand * int(2);
    auto ampCoEf = from;
	if (from == to) {
		for (auto i = startIndex; i < startIndex + length; i++) {
			fa[i] *= ampCoEf;
		}
		return fa;
	}
    auto descending = false;
    if(from > to) {
        descending = true;
        if(from < 1) {
            hannOffset = static_cast< int >((from * fadeLengthExpand));
        }
    }
    for (int i = 0; i < length; i++) {
        if(descending) {
            ampCoEf = value(hannLength, i + fadeLengthExpand + hannOffset);
        } else {
            ampCoEf = value(hannLength, i);
        }
        temp[startIndex + i] *= ampCoEf;
    }
    return temp;
}

float TimeStretch::value(int length, int index)
{
    return 0.5f * (1.0f - static_cast< float >(cos(TWO_PI * index / (length - 1.0f))));
}
