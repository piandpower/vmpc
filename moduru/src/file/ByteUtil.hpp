#pragma once
#include <string>
#include <vector>

namespace moduru {
	namespace file {

		class ByteUtil {

		public:
			static unsigned short bytes2ushort(std::vector<char> pair);
			static short bytes2short(std::vector<char> pair);
			static unsigned int bytes2uint(std::vector<char> quartet);

			static std::vector<char> uint2bytes(unsigned int n); // returns quartet
			static std::vector<char> ushort2bytes(unsigned short n); // returns pair
			static std::vector<char> short2bytes(short n); // returns pair

			static std::vector<char> stitchByteArrays(std::vector<std::vector<char> > byteArrays);

			static char hexToByte(std::string charPair);
			static std::vector<char> hexToBytes(std::string charPairs);
		};

	}
}
