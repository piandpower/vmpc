#pragma once

#include <vector>

#include <cstdint>

#include <lang/Object.hpp>

namespace moduru {

	class VecUtil {

	public:
		static std::vector<char> CopyOfRange(std::vector<char>* src, int offset, int length);
		static std::vector<double> CopyOf(std::vector<double> src, int newLength);

		static std::vector<short> BytesToShorts(std::vector<char> src);
		static bool Equals(std::vector<char> a, std::vector<char> b);

		static void VecCopy(std::vector<unsigned char>* src, int srcPos, std::vector<unsigned char>* dest, int destPos, int length);
		static void VecCopy(std::vector<char>* src, int srcPos, std::vector<char>* dest, int destPos, int length);
		static void VecCopy(std::vector<char16_t>* src, int srcPos, std::vector<char16_t>* dest, int destPos, int length);
		static void VecCopy(std::vector<int>* src, int srcPos, std::vector<int>* dest, int destPos, int length);
		static void VecCopy(std::vector<int8_t>* src, int srcPos, std::vector<int8_t>* dest, int destPos, int length);
		static void VecCopy(std::vector<int16_t>* src, int srcPos, std::vector<int16_t>* dest, int destPos, int length);
		static void VecCopy(std::vector<int64_t>* src, int srcPos, std::vector<int64_t>* dest, int destPos, int length);
		static void VecCopy(std::vector<float>* src, int srcPos, std::vector<float>* dest, int destPos, int length);
		static void VecCopy(std::vector<double>* src, int srcPos, std::vector<double>* dest, int destPos, int length);
		static void VecCopy(std::vector<bool>* src, int srcPos, std::vector<bool>* dest, int destPos, int length);

		static void VecCopy(std::vector<moduru::lang::Object*>* src, int64_t srcPos, std::vector<moduru::lang::Object*>* dest, uint64_t destPos, uint64_t length);
		static std::vector<int8_t> CopyOf(std::vector<int8_t> src, int newLength);
		static std::vector<int> CopyOf(std::vector<int> src, int newLength);

	};

}
