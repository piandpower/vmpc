#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace moduru {
	namespace lang {

		class FDBigInt;
		class String;

		class FloatingDecimal
		{

		public:
			bool isExceptional{};
			bool isNegative{};
			int32_t decExponent{};
			std::vector<char16_t> digits{};
			int32_t nDigits{};
			int32_t bigIntExp{};
			int32_t bigIntNBits{};
			bool mustSetRoundDir{ false };
			bool fromHex{ false };
			int32_t roundDir{ 0 };

		public:
			static constexpr int64_t signMask{ int64_t(-0x7fffffffffffffffLL - 1) };
			static constexpr int64_t expMask{ int64_t(9218868437227405312LL) };
			static constexpr int64_t fractMask{ int64_t(4503599627370495LL) };
			static constexpr int32_t expShift{ int32_t(52) };
			static constexpr int32_t expBias{ int32_t(1023) };
			static constexpr int64_t fractHOB{ int64_t(4503599627370496LL) };
			static constexpr int64_t expOne{ int64_t(4607182418800017408LL) };
			static constexpr int32_t maxSmallBinExp{ int32_t(62) };
			static constexpr int32_t minSmallBinExp{ int32_t(-21) };
			static constexpr int32_t maxDecimalDigits{ int32_t(15) };
			static constexpr int32_t maxDecimalExponent{ int32_t(308) };
			static constexpr int32_t minDecimalExponent{ int32_t(-324) };
			static constexpr int32_t bigDecimalExponent{ int32_t(324) };
			static constexpr int64_t highbyte{ int64_t(-72057594037927936LL) };
			static constexpr int64_t highbit{ int64_t(-0x7fffffffffffffffLL - 1) };
			static constexpr int64_t lowbytes{ int64_t(72057594037927935LL) };
			static constexpr int32_t singleSignMask{ int32_t(-0x7fffffff - 1) };
			static constexpr int32_t singleExpMask{ int32_t(2139095040) };
			static constexpr int32_t singleFractMask{ int32_t(8388607) };
			static constexpr int32_t singleExpShift{ int32_t(23) };
			static constexpr int32_t singleFractHOB{ int32_t(8388608) };
			static constexpr int32_t singleExpBias{ int32_t(127) };
			static constexpr int32_t singleMaxDecimalDigits{ int32_t(7) };
			static constexpr int32_t singleMaxDecimalExponent{ int32_t(38) };
			static constexpr int32_t singleMinDecimalExponent{ int32_t(-45) };
			static constexpr int32_t intDecimalDigits{ int32_t(9) };

		private:
			static int32_t countBits(int64_t v);
			static std::vector<FDBigInt*> b5p;
			static FDBigInt* big5pow(int32_t p);
			static FDBigInt* multPow52(FDBigInt* v, int32_t p5, int32_t p2);
			static FDBigInt* constructPow52(int32_t p5, int32_t p2);
			FDBigInt* doubleToBigInt(double dval);
			static double ulp(double dval, bool subtracting);

		public:
			virtual float stickyRound(double dval);

		private:
			void developLongDigits(int32_t decExponent, int64_t lvalue, int64_t insignificant);
			void roundup();

		private:
			void dtoa(int32_t binExp, int64_t fractBits, int32_t nSignificantBits);

		public:
			//std::string toString(); // override;
			virtual String* toJavaFormatString();

		private:
			int32_t getChars(std::vector<char16_t>* result);
			//static ThreadLocal* perThreadBuffer_;

		public:
			//virtual void appendTo(Appendable* buf);
			//static FloatingDecimal* readJavaFormatString(std::string in);
			virtual double doubleValue();
			virtual float floatValue();

		private:
			static std::vector<double> small10pow;
			static std::vector<float> singleSmall10pow;
			static std::vector<double> big10pow;
			static std::vector<double> tiny10pow;
			static int32_t maxSmallTen;
			static int32_t singleMaxSmallTen;
			static std::vector<int32_t> small5pow;
			static std::vector<int64_t> long5pow;
			static std::vector<int32_t> n5bits;
			static std::vector<char16_t> infinity;
			static std::vector<char16_t> notANumber;
			static std::vector<char16_t> zero;
			//static moduru::util::regex::Pattern* hexFloatPattern;

		public:
			//static FloatingDecimal* parseHexString(std::string s);
			//static std::string stripLeadingZeros(std::string s);
			//static int32_t getHexDigit(std::string s, int32_t position);

		private:
			FloatingDecimal(bool negSign, int32_t decExponent, std::vector<char16_t> digits, int32_t n, bool e);

		public:
			FloatingDecimal(double d);
			FloatingDecimal(float f);

			//friend class FloatingDecimal_1;
			//friend class FDBigInt;

		};
	}
}
