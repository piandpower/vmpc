#pragma once

#include <cstdint>
#include <vector>

#include <string>

namespace moduru {
	namespace lang {

		class FDBigInt
		{

		public:
			int32_t nWords{};
			std::vector<int32_t> data{};

		public:
			virtual void lshiftMe(int32_t c);
			virtual int32_t normalizeMe();
			virtual FDBigInt* mult(int32_t iv);
			virtual void multaddMe(int32_t iv, int32_t addend);
			virtual FDBigInt* mult(FDBigInt* other);
			virtual FDBigInt* add(FDBigInt* other);
			virtual FDBigInt* sub(FDBigInt* other);

		private:
			static bool dataInRangeIsZero(int32_t i, int32_t m, FDBigInt* other);

		public:
			virtual int32_t cmp(FDBigInt* other);
			virtual int32_t quoRemIteration(FDBigInt* S) /* throws(IllegalArgumentException) */;
			virtual int64_t longValue();
			std::string toString();

			FDBigInt(int32_t v);
			FDBigInt(int64_t v);
			FDBigInt(FDBigInt* other);

		private:
			FDBigInt(std::vector<int32_t> d, int32_t n);

		public:
			FDBigInt(int64_t seed, std::vector<char16_t> digit, int32_t nd0, int32_t nd);

		private:
			//friend class FloatingDecimal;
			//friend class FloatingDecimal_1;
		};

	}
}
