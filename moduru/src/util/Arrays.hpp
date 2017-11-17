#pragma once
#include <lang/Object.hpp>

#include <vector>
#include <set>

namespace moduru {

	namespace lang {
		class StringBuilder;
	}

	namespace util {

		class Comparator;

		class Arrays
			: public virtual moduru::lang::Object
		{

		public:
			typedef moduru::lang::Object super;

		public:
			static void sort(std::vector<int64_t>* a);
			static void sort(std::vector<int64_t>* a, int32_t fromIndex, int32_t toIndex);
			static void sort(std::vector<int32_t>* a);
			static void sort(std::vector<int32_t>* a, int32_t fromIndex, int32_t toIndex);
			static void sort(std::vector<int16_t>* a);
			static void sort(std::vector<int16_t>* a, int32_t fromIndex, int32_t toIndex);
			static void sort(std::vector<char16_t>* a);
			static void sort(std::vector<char16_t>* a, int32_t fromIndex, int32_t toIndex);
			static void sort(std::vector<int8_t>* a);
			static void sort(std::vector<int8_t>* a, int32_t fromIndex, int32_t toIndex);
			static void sort(std::vector<double>* a);
			static void sort(std::vector<double>* a, int32_t fromIndex, int32_t toIndex);
			static void sort(std::vector<float>* a);
			static void sort(std::vector<float>* a, int32_t fromIndex, int32_t toIndex);

		private:
			static void sort2(std::vector<double>* a, int32_t fromIndex, int32_t toIndex);
			static void sort2(std::vector<float>* a, int32_t fromIndex, int32_t toIndex);
			static void sort1(std::vector<int64_t>* x, int32_t off, int32_t len);
			static void swap(std::vector<int64_t>* x, int32_t a, int32_t b);
			static void vecswap(std::vector<int64_t>* x, int32_t a, int32_t b, int32_t n);
			static int32_t med3(std::vector<int64_t>* x, int32_t a, int32_t b, int32_t c);
			static void sort1(std::vector<int32_t>* x, int32_t off, int32_t len);
			static void swap(std::vector<int32_t>* x, int32_t a, int32_t b);
			static void vecswap(std::vector<int32_t>* x, int32_t a, int32_t b, int32_t n);
			static int32_t med3(std::vector<int32_t>* x, int32_t a, int32_t b, int32_t c);
			static void sort1(std::vector<int16_t>* x, int32_t off, int32_t len);
			static void swap(std::vector<int16_t>* x, int32_t a, int32_t b);
			static void vecswap(std::vector<int16_t>* x, int32_t a, int32_t b, int32_t n);
			static int32_t med3(std::vector<int16_t>* x, int32_t a, int32_t b, int32_t c);
			static void sort1(std::vector<char16_t>* x, int32_t off, int32_t len);
			static void swap(std::vector<char16_t>* x, int32_t a, int32_t b);
			static void vecswap(std::vector<char16_t>* x, int32_t a, int32_t b, int32_t n);
			static int32_t med3(std::vector<char16_t>* x, int32_t a, int32_t b, int32_t c);
			static void sort1(std::vector<int8_t>* x, int32_t off, int32_t len);
			static void swap(std::vector<int8_t>* x, int32_t a, int32_t b);
			static void vecswap(std::vector<int8_t>* x, int32_t a, int32_t b, int32_t n);
			static int32_t med3(std::vector<int8_t>* x, int32_t a, int32_t b, int32_t c);
			static void sort1(std::vector<double>* x, int32_t off, int32_t len);
			static void swap(std::vector<double>* x, int32_t a, int32_t b);
			static void vecswap(std::vector<double>* x, int32_t a, int32_t b, int32_t n);
			static int32_t med3(std::vector<double>* x, int32_t a, int32_t b, int32_t c);
			static void sort1(std::vector<float>* x, int32_t off, int32_t len);
			static void swap(std::vector<float>* x, int32_t a, int32_t b);
			static void vecswap(std::vector<float>* x, int32_t a, int32_t b, int32_t n);
			static int32_t med3(std::vector<float>* x, int32_t a, int32_t b, int32_t c);

		public:
			static void sort(std::vector<moduru::lang::Object*>* a);
			static void sort(std::vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex);

		private:
			static constexpr int32_t INSERTIONSORT_THRESHOLD{ int32_t(7) };
			static void mergeSort(std::vector<moduru::lang::Object*>* src, std::vector<moduru::lang::Object*>* dest, int32_t low, int32_t high, int32_t off);
			static void swap(std::vector<moduru::lang::Object*>* x, int32_t a, int32_t b);

		public:
			static void sort(std::vector<moduru::lang::Object*>* a, Comparator* c);
			static void sort(std::vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, Comparator* c);

		private:
			static void mergeSort(std::vector<moduru::lang::Object*>* src, std::vector<moduru::lang::Object*>* dest, int32_t low, int32_t high, int32_t off, Comparator* c);
			static void rangeCheck(int32_t arrayLen, int32_t fromIndex, int32_t toIndex);

		public:
			static int32_t binarySearch(std::vector<int64_t>* a, int64_t key);
			static int32_t binarySearch(std::vector<int64_t>* a, int32_t fromIndex, int32_t toIndex, int64_t key);

		private:
			static int32_t binarySearch0(std::vector<int64_t>* a, int32_t fromIndex, int32_t toIndex, int64_t key);

		public:
			static int32_t binarySearch(std::vector<int32_t>* a, int32_t key);
			static int32_t binarySearch(std::vector<int32_t>* a, int32_t fromIndex, int32_t toIndex, int32_t key);

		private:
			static int32_t binarySearch0(std::vector<int32_t>* a, int32_t fromIndex, int32_t toIndex, int32_t key);

		public:
			static int32_t binarySearch(std::vector<int16_t>* a, int16_t key);
			static int32_t binarySearch(std::vector<int16_t>* a, int32_t fromIndex, int32_t toIndex, int16_t key);

		private:
			static int32_t binarySearch0(std::vector<int16_t>* a, int32_t fromIndex, int32_t toIndex, int16_t key);

		public:
			static int32_t binarySearch(std::vector<char16_t>* a, char16_t key);
			static int32_t binarySearch(std::vector<char16_t>* a, int32_t fromIndex, int32_t toIndex, char16_t key);

		private:
			static int32_t binarySearch0(std::vector<char16_t>* a, int32_t fromIndex, int32_t toIndex, char16_t key);

		public:
			static int32_t binarySearch(std::vector<int8_t>* a, int8_t key);
			static int32_t binarySearch(std::vector<int8_t>* a, int32_t fromIndex, int32_t toIndex, int8_t key);

		private:
			static int32_t binarySearch0(std::vector<int8_t>* a, int32_t fromIndex, int32_t toIndex, int8_t key);

		public:
			static int32_t binarySearch(std::vector<double>* a, double key);
			static int32_t binarySearch(std::vector<double>* a, int32_t fromIndex, int32_t toIndex, double key);

		private:
			static int32_t binarySearch0(std::vector<double>* a, int32_t fromIndex, int32_t toIndex, double key);

		public:
			static int32_t binarySearch(std::vector<float>* a, float key);
			static int32_t binarySearch(std::vector<float>* a, int32_t fromIndex, int32_t toIndex, float key);

		private:
			static int32_t binarySearch0(std::vector<float>* a, int32_t fromIndex, int32_t toIndex, float key);

		public:
			static int32_t binarySearch(std::vector<moduru::lang::Object*>* a, moduru::lang::Object* key);
			static int32_t binarySearch(std::vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* key);

		private:
			static int32_t binarySearch0(std::vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* key);

		public:
			static int32_t binarySearch(std::vector<moduru::lang::Object*>* a, moduru::lang::Object* key, Comparator* c);
			static int32_t binarySearch(std::vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* key, Comparator* c);

		private:
			static int32_t binarySearch0(std::vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* key, Comparator* c);

		public:
			static bool equals(std::vector<int64_t>* a, std::vector<int64_t>* a2);
			static bool equals(std::vector<int32_t>* a, std::vector<int32_t>* a2);
			static bool equals(std::vector<int16_t>* a, std::vector<int16_t>* a2);
			static bool equals(std::vector<char16_t>* a, std::vector<char16_t>* a2);
			static bool equals(std::vector<int8_t>* a, std::vector<int8_t>* a2);
			static bool equals(std::vector<bool>* a, std::vector<bool>* a2);
			static bool equals(std::vector<double>* a, std::vector<double>* a2);
			static bool equals(std::vector<float>* a, std::vector<float>* a2);
			static bool equals(std::vector<moduru::lang::Object*>* a, std::vector<moduru::lang::Object*>* a2);
			static void fill(std::vector<int64_t>* a, int64_t val);
			static void fill(std::vector<int64_t>* a, int32_t fromIndex, int32_t toIndex, int64_t val);
			static void fill(std::vector<int32_t>* a, int32_t val);
			static void fill(std::vector<int32_t>* a, int32_t fromIndex, int32_t toIndex, int32_t val);
			static void fill(std::vector<int16_t>* a, int16_t val);
			static void fill(std::vector<int16_t>* a, int32_t fromIndex, int32_t toIndex, int16_t val);
			static void fill(std::vector<char16_t>* a, char16_t val);
			static void fill(std::vector<char16_t>* a, int32_t fromIndex, int32_t toIndex, char16_t val);
			static void fill(std::vector<int8_t>* a, int8_t val);
			static void fill(std::vector<int8_t>* a, int32_t fromIndex, int32_t toIndex, int8_t val);
			static void fill(std::vector<bool>* a, bool val);
			static void fill(std::vector<bool>* a, int32_t fromIndex, int32_t toIndex, bool val);
			static void fill(std::vector<double>* a, double val);
			static void fill(std::vector<double>* a, int32_t fromIndex, int32_t toIndex, double val);
			static void fill(std::vector<float>* a, float val);
			static void fill(std::vector<float>* a, int32_t fromIndex, int32_t toIndex, float val);
			static void fill(std::vector<moduru::lang::Object*>* a, moduru::lang::Object* val);
			static void fill(std::vector<moduru::lang::Object*>* a, int32_t fromIndex, int32_t toIndex, moduru::lang::Object* val);
			static std::vector<moduru::lang::Object*>* copyOf(std::vector<moduru::lang::Object*>* original, int32_t newLength);
			//static std::vector<moduru::lang::Object*>* copyOf(std::vector<moduru::lang::Object*>* original, int32_t newLength, moduru::lang::Class* newType);
			static std::vector<int8_t>* copyOf(std::vector<int8_t>* original, int32_t newLength);
			static std::vector<int16_t>* copyOf(std::vector<int16_t>* original, int32_t newLength);
			static std::vector<int32_t>* copyOf(std::vector<int32_t>* original, int32_t newLength);
			static std::vector<int64_t>* copyOf(std::vector<int64_t>* original, int32_t newLength);
			static std::vector<char16_t>* copyOf(std::vector<char16_t>* original, int32_t newLength);
			static std::vector<float>* copyOf(std::vector<float>* original, int32_t newLength);
			static std::vector<double>* copyOf(std::vector<double>* original, int32_t newLength);
			static std::vector<bool>* copyOf(std::vector<bool>* original, int32_t newLength);
			static std::vector<moduru::lang::Object*>* copyOfRange(std::vector<moduru::lang::Object*>* original, int32_t from, int32_t to);
			//static std::vector<moduru::lang::Object*>* copyOfRange(std::vector<moduru::lang::Object*>* original, int32_t from, int32_t to, moduru::lang::Class* newType);
			static std::vector<int8_t>* copyOfRange(std::vector<int8_t>* original, int32_t from, int32_t to);
			static std::vector<int16_t>* copyOfRange(std::vector<int16_t>* original, int32_t from, int32_t to);
			static std::vector<int32_t>* copyOfRange(std::vector<int32_t>* original, int32_t from, int32_t to);
			static std::vector<int64_t>* copyOfRange(std::vector<int64_t>* original, int32_t from, int32_t to);
			static std::vector<char16_t>* copyOfRange(std::vector<char16_t>* original, int32_t from, int32_t to);
			static std::vector<float>* copyOfRange(std::vector<float>* original, int32_t from, int32_t to);
			static std::vector<double>* copyOfRange(std::vector<double>* original, int32_t from, int32_t to);
			static std::vector<bool>* copyOfRange(std::vector<bool>* original, int32_t from, int32_t to);
			//static List* asList(std::vector<moduru::lang::Object*>* a);
			static int32_t hashCode(std::vector<int64_t>* a);
			static int32_t hashCode(std::vector<int32_t>* a);
			static int32_t hashCode(std::vector<int16_t>* a);
			static int32_t hashCode(std::vector<char16_t>* a);
			static int32_t hashCode(std::vector<int8_t>* a);
			static int32_t hashCode(std::vector<bool>* a);
			static int32_t hashCode(std::vector<float>* a);
			static int32_t hashCode(std::vector<double>* a);
			static int32_t hashCode(std::vector<moduru::lang::Object*>* a);
			static int32_t deepHashCode(std::vector<moduru::lang::Object*>* a);
			static bool deepEquals(std::vector<moduru::lang::Object*>* a1, std::vector<moduru::lang::Object*>* a2);
			/*
			static moduru::lang::String* toString(std::vector<int64_t>* a);
			static moduru::lang::String* toString(std::vector<int32_t>* a);
			static moduru::lang::String* toString(std::vector<int16_t>* a);
			static moduru::lang::String* toString(std::vector<char16_t>* a);
			static moduru::lang::String* toString(std::vector<int8_t>* a);
			static moduru::lang::String* toString(std::vector<bool>* a);
			static moduru::lang::String* toString(std::vector<float>* a);
			static moduru::lang::String* toString(std::vector<double>* a);
			static moduru::lang::String* toString(std::vector<moduru::lang::Object*>* a);
			static moduru::lang::String* deepToString(std::vector<moduru::lang::Object*>* a);
			*/

		private:
			//static void deepToString(std::vector<moduru::lang::Object*>* a, moduru::lang::StringBuilder* buf, set<std::vector<moduru::lang::Object*>*> dejaVu);

			Arrays();

		private:
			//friend class Arrays_ArrayList;
		};

	}
}
