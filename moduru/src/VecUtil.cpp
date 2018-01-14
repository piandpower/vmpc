#include <VecUtil.hpp>

#include <file/ByteUtil.hpp>
#include <stdexcept>
using namespace moduru;
using namespace std;

vector<char> VecUtil::CopyOfRange(vector<char>* src, int offset, int endOffset) {
	return vector<char>(src->begin() + offset, src->begin() + endOffset);
}

vector<double> CopyOf(vector<double> src, int newLength) {
	while (src.size() < newLength) {
		src.push_back(0);
	}
	return vector<double>(src.begin(), src.begin() + newLength);
}

vector<short> VecUtil::BytesToShorts(vector<char> src) {
	if (src.size() == 0 || src.size() % 2 != 0) return vector<short>(0);
	int length = (int) (src.size()) / 2;
	auto res = vector<short>(length);
	int charCounter = 0;
	auto pair = vector<char>(2);
	for (int i = 0; i < length; i++) {
		pair[0] = src[charCounter++];
		pair[1] = src[charCounter++];
		res[i] = moduru::file::ByteUtil::bytes2short(pair);
	}
	return res;
}

bool VecUtil::Equals(vector<char> a, vector<char> b) {
	if (a.size() != b.size()) return false;
	for (int i = 0; i < a.size(); i++)
		if (a[i] != b[i]) return false;
	return true;
}

/*
template<typename T>
void VecUtil::VecCopy(vector<T>* src, int srcPos, vector<T>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<unsigned char>(src->size());
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}
*/
/*
void VecUtil::VecCopy(vector<unsigned char>* src, int srcPos, vector<unsigned char>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<unsigned char>(src->size());
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<char>* src, int srcPos, vector<char>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<char>(src->size());
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<double>* src, int srcPos, vector<double>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<double>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<int>* src, int srcPos, vector<int>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<int>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<float>* src, int srcPos, vector<float>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<float>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<char16_t>* src, int srcPos, vector<char16_t>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<char16_t>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<bool>* src, int srcPos, vector<bool>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<bool>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<int64_t>* src, int srcPos, vector<int64_t>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<int64_t>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}


void VecUtil::VecCopy(vector<int16_t>* src, int srcPos, vector<int16_t>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<int16_t>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<int8_t>* src, int srcPos, vector<int8_t>* dest, int destPos, int length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<int8_t>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}

void VecUtil::VecCopy(vector<moduru::lang::Object*>* src, int64_t srcPos, vector<moduru::lang::Object*>* dest, uint64_t destPos, uint64_t length) {
	if (destPos + length > dest->size()) throw std::invalid_argument("Destination out of bounds!\n");
	if (srcPos + length > src->size()) throw std::invalid_argument("Source out of bounds!\n");
	if (src == dest) dest = new vector<moduru::lang::Object*>(src->size());
	dest->resize(length);
	for (int i = 0; i < length; i++)
		(*dest)[i + destPos] = (*src)[i + srcPos];
}
*/

vector<int8_t> VecUtil::CopyOf(vector<int8_t> src, int newLength) {
	while (src.size() < newLength) {
		src.push_back(0);
	}
	return vector<int8_t>(src.begin(), src.begin() + newLength);
}

vector<int> VecUtil::CopyOf(vector<int> src, int newLength) {
	while (src.size() < newLength) {
		src.push_back(0);
	}
	return vector<int>(src.begin(), src.begin() + newLength);
}
