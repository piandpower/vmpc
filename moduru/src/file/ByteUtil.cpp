#include <file/ByteUtil.hpp>

#include <sstream>

using namespace moduru::file;
using namespace std;

unsigned short ByteUtil::bytes2ushort(vector<char> ca) {
	unsigned char pair[2];
	pair[0] = ca[0];
	pair[1] = ca[1];
	return (pair[1] << 8) | pair[0];
}

short ByteUtil::bytes2short(vector<char> ca) {
	return (ca[1] << 8) | (ca[0] & 0xff);
}

/*
short ByteUtil::bytes2short(vector<char> ca) {
	unsigned char pair[2];
	pair[0] = ca[0];
	pair[1] = ca[1];
	return (pair[1] << 8) | (pair[0] & 0xff);
}
*/

unsigned int ByteUtil::bytes2uint(vector<char> quartet) {
	char ca[4];
	for (int i = 0; i < 4; i++)
		ca[i] = quartet[i];
	return  *(unsigned int *)ca;
}

vector<char> ByteUtil::uint2bytes(unsigned int n) {
	auto res = vector<char>(4);
	res[3] = (n >> 24) & 0xFF;
	res[2] = (n >> 16) & 0xFF;
	res[1] = (n >> 8) & 0xFF;
	res[0] = n & 0xFF;
	return res;
}

vector<char> ByteUtil::ushort2bytes(unsigned short n) {
	//little endian
	auto res = vector<char>(2);
	res[1] = (n >> 8);
	res[0] = n & 0xFF;
	return res;
}

vector<char> ByteUtil::short2bytes(short n) {
	//little endian
	auto res = vector<char>(2);
	res[1] = (n >> 8) & 0xFF;
	res[0] = n & 0xff;
	return res;
}

vector<char> ByteUtil::stitchByteArrays(vector<vector<char> > byteArrays)
{
	unsigned int totalSize = 0;
	for (auto& ba : byteArrays) {
		totalSize += (int) ba.size();
	}
	auto result = vector<char>(totalSize);
	int counter = 0;
	for (auto& ba : byteArrays) {
		for (auto b : ba)
			result[counter++] = b;
	}
	return result;
}


char ByteUtil::hexToByte(string charPair)
{
	unsigned short byte = 0;
	std::istringstream iss(charPair.c_str());
	iss >> std::hex >> byte;
	return byte % 0x100;
}

vector<char> ByteUtil::hexToBytes(string charPairs)
{
	vector<char> res;
	for (int i = 0; i < charPairs.size(); i += 2) {
		string current;
		current.push_back(charPairs[i]);
		current.push_back(charPairs[i + 1]);
		res.push_back(hexToByte(current));
	}
	return res;
}
