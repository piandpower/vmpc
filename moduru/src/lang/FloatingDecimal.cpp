#include <lang/FloatingDecimal.hpp>

//#include <lang/Appendable.hpp>
#include <lang/Character.hpp>
#include <lang/Double.hpp>
#include <lang/DoubleConsts.hpp>
#include <lang/Float.hpp>
#include <lang/FloatConsts.hpp>
//#include <lang/FloatingDecimal_1.hpp>
#include <lang/FDBigInt.hpp>
#include <lang/FpUtils.hpp>
#include <lang/Integer.hpp>
#include <lang/String.hpp>
//#include <lang/Math.hpp>
//#include <java/util/regex/Matcher.hpp>
//#include <java/util/regex/Pattern.hpp>

#include <math/Math.hpp>
#include <VecUtil.hpp>

#include <cmath>

using namespace moduru::lang;
using namespace std;

FloatingDecimal::FloatingDecimal(bool negSign, int32_t decExponent, vector<char16_t> digits, int32_t n, bool e) 
{
	isNegative = negSign;
	isExceptional = e;
	this->decExponent = decExponent;
	this->digits = digits;
	this->nDigits = n;
}

FloatingDecimal::FloatingDecimal(double d)
{
	auto dBits = Double::doubleToLongBits(d);
	int64_t fractBits;
	int32_t binExp;
	int32_t nSignificantBits;
	if ((dBits & signMask) != 0) {
		isNegative = true;
		dBits ^= signMask;
	}
	else {
		isNegative = false;
	}
	binExp = static_cast<int32_t>(((dBits & expMask) >> expShift));
	fractBits = dBits & fractMask;
	if (binExp == static_cast<int32_t>((expMask >> expShift))) {
		isExceptional = true;
		if (fractBits == 0LL) {
			digits = infinity;
		}
		else {
			digits = notANumber;
			isNegative = false;
		}
		nDigits = digits.size();
		return;
	}
	isExceptional = false;
	if (binExp == 0) {
		if (fractBits == 0LL) {
			decExponent = 0;
			digits = zero;
			nDigits = 1;
			return;
		}
		while ((fractBits & fractHOB) == 0LL) {
			fractBits <<= 1;
			binExp -= 1;
		}
		nSignificantBits = expShift + binExp + int32_t(1);
		binExp += 1;
	}
	else {
		fractBits |= fractHOB;
		nSignificantBits = expShift + int32_t(1);
	}
	binExp -= expBias;
	dtoa(binExp, fractBits, nSignificantBits);
}

FloatingDecimal::FloatingDecimal(float f) 
{
	auto fBits = Float::floatToIntBits(f);
	int32_t fractBits;
	int32_t binExp;
	int32_t nSignificantBits;
	if ((fBits & singleSignMask) != 0) {
		isNegative = true;
		fBits ^= singleSignMask;
	}
	else {
		isNegative = false;
	}
	binExp = static_cast< int32_t >(((fBits & singleExpMask) >> singleExpShift));
	fractBits = fBits & singleFractMask;
	if (binExp == static_cast< int32_t >((singleExpMask >> singleExpShift))) {
		isExceptional = true;
		if (fractBits == 0LL) {
			digits = infinity;
		}
		else {
			digits = notANumber;
			isNegative = false;
		}
		nDigits = digits.size();
		return;
	}
	isExceptional = false;
	if (binExp == 0) {
		if (fractBits == 0) {
			decExponent = 0;
			digits = zero;
			nDigits = 1;
			return;
		}
		while ((fractBits & singleFractHOB) == 0) {
			fractBits <<= 1;
			binExp -= 1;
		}
		nSignificantBits = singleExpShift + binExp + int32_t(1);
		binExp += 1;
	}
	else {
		fractBits |= singleFractHOB;
		nSignificantBits = singleExpShift + int32_t(1);
	}
	binExp -= singleExpBias;
	dtoa(binExp, (static_cast< int64_t >(fractBits)) << (expShift - singleExpShift), nSignificantBits);
}

constexpr int64_t FloatingDecimal::signMask;
constexpr int64_t FloatingDecimal::expMask;
constexpr int64_t FloatingDecimal::fractMask;
constexpr int32_t FloatingDecimal::expShift;
constexpr int32_t FloatingDecimal::expBias;
constexpr int64_t FloatingDecimal::fractHOB;
constexpr int64_t FloatingDecimal::expOne;
constexpr int32_t FloatingDecimal::maxSmallBinExp;
constexpr int32_t FloatingDecimal::minSmallBinExp;
constexpr int32_t FloatingDecimal::maxDecimalDigits;
constexpr int32_t FloatingDecimal::maxDecimalExponent;
constexpr int32_t FloatingDecimal::minDecimalExponent;
constexpr int32_t FloatingDecimal::bigDecimalExponent;
constexpr int64_t FloatingDecimal::highbyte;
constexpr int64_t FloatingDecimal::highbit;
constexpr int64_t FloatingDecimal::lowbytes;
constexpr int32_t FloatingDecimal::singleSignMask;
constexpr int32_t FloatingDecimal::singleExpMask;
constexpr int32_t FloatingDecimal::singleFractMask;
constexpr int32_t FloatingDecimal::singleExpShift;
constexpr int32_t FloatingDecimal::singleFractHOB;
constexpr int32_t FloatingDecimal::singleExpBias;
constexpr int32_t FloatingDecimal::singleMaxDecimalDigits;
constexpr int32_t FloatingDecimal::singleMaxDecimalExponent;
constexpr int32_t FloatingDecimal::singleMinDecimalExponent;
constexpr int32_t FloatingDecimal::intDecimalDigits;

int32_t FloatingDecimal::countBits(int64_t v)
{

    if(v == 0LL)
        return 0;

    while ((v & highbyte) == 0LL) {
        v <<= 8;
    }
    while (v > 0LL) {
        v <<= 1;
    }
    auto n = int32_t(0);
    while ((v & lowbytes) != 0LL) {
        v <<= 8;
        n += 8;
    }
    while (v != 0LL) {
        v <<= 1;
        n += 1;
    }
    return n;
}

vector<FDBigInt*> FloatingDecimal::b5p;

FDBigInt* FloatingDecimal::big5pow(int32_t p)
{

	/* assert(p >= 0 : p) */;
	if (&b5p == nullptr) {
		b5p = vector<FDBigInt*>(p + int32_t(1));
	}
	else if (b5p.size() <= p) {
		auto t = vector<FDBigInt*>(p + int32_t(1));
		//moduru::VecUtil::VecCopy(&b5p, 0, &t, 0, b5p.size());
		b5p = t;
	}
	if (b5p[p] != nullptr)
		return b5p[p];
	else if (p < small5pow.size())
		return b5p[p] = new FDBigInt(small5pow[p]);
	else if (p < long5pow.size())
		return b5p[p] = new FDBigInt(long5pow[p]);
	else {
		int32_t q, r;
		q = p >> int32_t(1);
		r = p - q;
		auto bigq = b5p[q];
		if (bigq == nullptr)
			bigq = big5pow(q);

		if (r < small5pow.size()) {
			return (b5p[p] = bigq->mult(small5pow[r]));
		}
		else {
			auto bigr = b5p[r];
			if (bigr == nullptr)
				bigr = big5pow(r);

			return (b5p[p] = bigq->mult(bigr));
		}
	}
}

FDBigInt* FloatingDecimal::multPow52(FDBigInt* v, int32_t p5, int32_t p2)
{

    if(p5 != 0) {
        if(p5 < small5pow.size()) {
            v = v->mult(small5pow[p5]);
        } else {
            v = v->mult(big5pow(p5));
        }
    }
    if(p2 != 0) {
        v->lshiftMe(p2);
    }
    return v;
}

FDBigInt* FloatingDecimal::constructPow52(int32_t p5, int32_t p2)
{

    auto v = new FDBigInt(big5pow(p5));
    if(p2 != 0) {
        v->lshiftMe(p2);
    }
    return v;
}

FDBigInt* FloatingDecimal::doubleToBigInt(double dval)
{
    auto lbits = Double::doubleToLongBits(dval) & ~signMask;
    auto binexp = static_cast< int32_t >((static_cast<int64_t>(static_cast<uint64_t>(lbits) >> expShift)));
    lbits &= fractMask;
    if(binexp > 0) {
        lbits |= fractHOB;
    } else {
        /* assert(lbits != 0LL : lbits) */ ;
        binexp += 1;
        while ((lbits & fractHOB) == 0LL) {
            lbits <<= 1;
            binexp -= 1;
        }
    }
    binexp -= expBias;
    auto nbits = countBits(lbits);
    auto lowOrderZeros = expShift + int32_t(1) - nbits;
    lbits = static_cast<uint64_t>(lbits) >> lowOrderZeros;
    bigIntExp = binexp + int32_t(1) - nbits;
    bigIntNBits = nbits;
    return new FDBigInt(lbits);
}

double FloatingDecimal::ulp(double dval, bool subtracting)
{

    auto lbits = Double::doubleToLongBits(dval) & ~signMask;
    auto binexp = static_cast< int32_t >((static_cast<int64_t>(static_cast<uint64_t>(lbits) >> expShift)));
    double ulpval;
    if(subtracting && (binexp >= expShift) && ((lbits & fractMask) == 0LL)) {
        binexp -= 1;
    }
    if(binexp > expShift) {
        ulpval = Double::longBitsToDouble((static_cast< int64_t >((binexp - expShift))) << expShift);
    } else if(binexp == 0) {
        ulpval = Double::MIN_VALUE;
    } else {
        ulpval = Double::longBitsToDouble(int64_t(1LL) << (binexp - int32_t(1)));
    }
    if(subtracting)
        ulpval = -ulpval;

    return ulpval;
}

float FloatingDecimal::stickyRound(double dval)
{
    auto lbits = Double::doubleToLongBits(dval);
    auto binexp = lbits & expMask;
    if(binexp == 0LL || binexp == expMask) {
        return static_cast< float >(dval);
    }
    lbits += static_cast< int64_t >(roundDir);
    return static_cast< float >(Double::longBitsToDouble(lbits));
}

void FloatingDecimal::developLongDigits(int32_t decExponent, int64_t lvalue, int64_t insignificant)
{
    vector<char16_t> digits;
    int32_t ndigits;
    int32_t digitno;
    int32_t c;
    int32_t i;
    for (i = 0; insignificant >= 10LL; i++) 
                insignificant /= 10LL;

    if(i != 0) {
        auto pow10 = long5pow[i] << i;
        auto residue = lvalue % pow10;
        lvalue /= pow10;
        decExponent += i;
        if(residue >= (pow10 >> int32_t(1))) {
            lvalue++;
        }
    }
    if(lvalue <= Integer::MAX_VALUE) {
        auto ivalue = static_cast< int32_t >(lvalue);
        ndigits = 10;
        //digits = dynamic_cast< vector<char16_t> >(perThreadBuffer->get()));
        digitno = ndigits - int32_t(1);
        c = ivalue % int32_t(10);
        ivalue /= 10;
        while (c == 0) {
            decExponent++;
            c = ivalue % int32_t(10);
            ivalue /= 10;
        }
        while (ivalue != 0) {
            digits[digitno--] = static_cast< char16_t >((c + u'0'));
            decExponent++;
            c = ivalue % int32_t(10);
            ivalue /= 10;
        }
        digits[digitno] = static_cast< char16_t >((c + u'0'));
    } else {
        ndigits = 20;
        //digits = dynamic_cast< vector<char16_t> >((dynamic_cast< Object* >(perThreadBuffer->get()));
        digitno = ndigits - int32_t(1);
        c = static_cast< int32_t >((lvalue % int64_t(10LL)));
        lvalue /= 10LL;
        while (c == 0) {
            decExponent++;
            c = static_cast< int32_t >((lvalue % int64_t(10LL)));
            lvalue /= 10LL;
        }
        while (lvalue != 0LL) {
            digits[digitno--] = static_cast< char16_t >((c + u'0'));
            decExponent++;
            c = static_cast< int32_t >((lvalue % int64_t(10LL)));
            lvalue /= 10;
        }
        digits[digitno] = static_cast< char16_t >((c + u'0'));
    }
    vector<char16_t> result;
    ndigits -= digitno;
    result = vector<char16_t>(ndigits);
    moduru::VecUtil::VecCopy(&digits, digitno, &result, 0, ndigits);
    this->digits = result;
    this->decExponent = decExponent + int32_t(1);
    this->nDigits = ndigits;
}

void FloatingDecimal::roundup()
{
    int32_t i;
    int32_t q = digits[i = (nDigits - int32_t(1))];
    if(q == u'9') {
        while (q == u'9' && i > 0) {
            digits[i] = u'0';
            q = digits[--i];
        }
        if(q == u'9') {
            decExponent += 1;
            digits[int32_t(0)] = u'1';
            return;
        }
    }
    digits[i] = static_cast< char16_t >((q + int32_t(1)));
}

void FloatingDecimal::dtoa(int32_t binExp, int64_t fractBits, int32_t nSignificantBits)
{
	int32_t nFractBits;
	int32_t nTinyBits;
	int32_t decExp;
	nFractBits = countBits(fractBits);
	nTinyBits = moduru::math::Math::max(int32_t(0), nFractBits - binExp - int32_t(1));
	if (binExp <= maxSmallBinExp && binExp >= minSmallBinExp) {
		if ((nTinyBits < long5pow.size()) && ((nFractBits + n5bits[nTinyBits]) < 64)) {
			int64_t halfULP;
			if (nTinyBits == 0) {
				if (binExp > nSignificantBits) {
					halfULP = int64_t(1LL) << (binExp - nSignificantBits - int32_t(1));
				}
				else {
					halfULP = 0LL;
				}
				if (binExp >= expShift) {
					fractBits <<= (binExp - expShift);
				}
				else {
					fractBits = static_cast<uint64_t>(fractBits) >> (expShift - binExp);
				}
				developLongDigits(0, fractBits, halfULP);
				return;
			}
		}
	}
	auto d2 = Double::longBitsToDouble(expOne | (fractBits & ~fractHOB));
	decExp = static_cast<int32_t>(floor((d2 - 1.5) * 0.289529654 + 0.176091259 + static_cast<double>(binExp) * 0.301029995663981));
	int32_t B2, B5;
	int32_t S2, S5;
	int32_t M2, M5;
	int32_t Bbits;
	int32_t tenSbits;
	FDBigInt* Sval;
	FDBigInt* Bval;
	FDBigInt* Mval;
	B5 = moduru::math::Math::max(int32_t(0), -decExp);
	B2 = B5 + nTinyBits + binExp;
	S5 = moduru::math::Math::max(int32_t(0), decExp);
	S2 = S5 + nTinyBits;
	M5 = B5;
	M2 = B2 - nSignificantBits;
	fractBits = static_cast<uint64_t>(fractBits) >> (expShift + int32_t(1) - nFractBits);
	B2 -= nFractBits - int32_t(1);
	auto common2factor = moduru::math::Math::min(B2, S2);
	B2 -= common2factor;
	S2 -= common2factor;
	M2 -= common2factor;
	if (nFractBits == 1)
		M2 -= 1;

	if (M2 < 0) {
		B2 -= M2;
		S2 -= M2;
		M2 = 0;
	}
	auto digits = this->digits = vector<char16_t>(int32_t(18));
	auto ndigit = int32_t(0);
	bool low, high;
	int64_t lowDigitDifference;
	int32_t q;
	Bbits = nFractBits + B2 + ((B5 < n5bits.size()) ? n5bits[B5] : (B5 * int32_t(3)));
	tenSbits = S2 + int32_t(1) + (((S5 + int32_t(1)) < n5bits.size()) ? n5bits[(S5 + int32_t(1))] : ((S5 + int32_t(1)) * int32_t(3)));
	if (Bbits < 64 && tenSbits < 64) {
		if (Bbits < 32 && tenSbits < 32) {
			auto b = (static_cast<int32_t>(fractBits) * small5pow[B5]) << B2;
			auto s = small5pow[S5] << S2;
			auto m = small5pow[M5] << M2;
			auto tens = s * int32_t(10);
			ndigit = 0;
			q = b / s;
			b = int32_t(10) * (b % s);
			m *= 10;
			low = (b < m);
			high = (b + m > tens);
			/* assert(q < 10 : q) */;
			if ((q == 0) && !high) {
				decExp--;
			}
			else {
				digits[ndigit++] = static_cast<char16_t>(('0' + q));
			}
			if (decExp <= -int32_t(3) || decExp >= 8) {
				high = low = false;
			}
			while (!low && !high) {
				q = b / s;
				b = int32_t(10) * (b % s);
				m *= 10;
				/* assert(q < 10 : q) */;
				if (m > 0LL) {
					low = (b < m);
					high = (b + m > tens);
				}
				else {
					low = true;
					high = true;
				}
				digits[ndigit++] = static_cast<char16_t>(('0' + q));
			}
			lowDigitDifference = (b << int32_t(1)) - tens;
		}
		else {
			auto b = (fractBits * long5pow[B5]) << B2;
			auto s = long5pow[S5] << S2;
			auto m = long5pow[M5] << M2;
			auto tens = s * int64_t(10LL);
			ndigit = 0;
			q = static_cast<int32_t>((b / s));
			b = int64_t(10LL) * (b % s);
			m *= 10LL;
			low = (b < m);
			high = (b + m > tens);
			/* assert(q < 10 : q) */;
			if ((q == 0) && !high) {
				decExp--;
			}
			else {
				digits[ndigit++] = static_cast<char16_t>(('0' + q));
			}
			if (decExp <= -int32_t(3) || decExp >= 8) {
				high = low = false;
			}
			while (!low && !high) {
				q = static_cast<int32_t>((b / s));
				b = int32_t(10) * (b % s);
				m *= 10;
				/* assert(q < 10 : q) */;
				if (m > 0LL) {
					low = (b < m);
					high = (b + m > tens);
				}
				else {
					low = true;
					high = true;
				}
				digits[ndigit++] = static_cast<char16_t>(('0' + q));
			}
			lowDigitDifference = (b << int32_t(1)) - tens;
		}
	}
	else {
		FDBigInt* tenSval;
		int32_t shiftBias;
		Bval = multPow52(new FDBigInt(fractBits), B5, B2);
		Sval = constructPow52(S5, S2);
		Mval = constructPow52(M5, M2);
		Bval->lshiftMe(shiftBias = Sval->normalizeMe());
		Mval->lshiftMe(shiftBias);
		tenSval = Sval->mult(int32_t(10));
		ndigit = 0;
		q = Bval->quoRemIteration(Sval);
		Mval = Mval->mult(int32_t(10));
		low = (Bval->cmp(Mval) < 0);
		high = (Bval->add(Mval)->cmp(tenSval) > 0);
		/* assert(q < 10 : q) */;
		if ((q == 0) && !high) {
			decExp--;
		}
		else {
			digits[ndigit++] = static_cast<char16_t>(('0' + q));
		}
		if (decExp <= -int32_t(3) || decExp >= 8) {
			high = low = false;
		}
		while (!low && !high) {
			q = Bval->quoRemIteration(Sval);
			Mval = Mval->mult(int32_t(10));
			/* assert(q < 10 : q) */;
			low = (Bval->cmp(Mval) < 0);
			high = (Bval->add(Mval)->cmp(tenSval) > 0);
			digits[ndigit++] = static_cast<char16_t>(('0' + q));
		}
		if (high && low) {
			Bval->lshiftMe(1);
			lowDigitDifference = Bval->cmp(tenSval);
		}
		else
			lowDigitDifference = 0LL;
	}
	this->decExponent = decExp + int32_t(1);
	this->digits = digits;
	this->nDigits = ndigit;
	if (high) {
		if (low) {
			if (lowDigitDifference == 0LL) {
				if ((digits[nDigits - int32_t(1)] & int32_t(1)) != 0)
					roundup();

			}
			else if (lowDigitDifference > 0) {
				roundup();
			}
		}
		else {
			roundup();
		}
	}
}

/*
moduru::lang::String* FloatingDecimal::toString()
{
    auto result = new StringBuffer(nDigits + int32_t(8));
    if(isNegative) {
        npc(result)->append('-');
    }
    if(isExceptional) {
        npc(result)->append(digits, int32_t(0), nDigits);
    } else {
        npc(result)->append("0.");
        npc(result)->append(digits, int32_t(0), nDigits);
        npc(result)->append('e');
        npc(result)->append(decExponent);
    }
    return new String(result);
}
*/

moduru::lang::String* FloatingDecimal::toJavaFormatString()
{
    //auto result = perThreadBuffer_->get();
    //auto i = getChars(result);
    //return new String(result, int32_t(0), i);
	return new String("FloatingDecimal not working yet");
}

int32_t FloatingDecimal::getChars(vector<char16_t>* result)
{
    /* assert(nDigits <= 19 : nDigits) */ ;
    auto i = int32_t(0);
    if(isNegative) {
        (*result)[int32_t(0)] = u'-';
        i = 1;
    }
    if(isExceptional) {
        moduru::VecUtil::VecCopy(&digits, 0, result, i, nDigits);
        i += nDigits;
    } else {
        if(decExponent > 0 && decExponent < 8) {
            auto charLength = moduru::math::Math::min(nDigits, decExponent);
            moduru::VecUtil::VecCopy(&digits, 0, result, i, charLength);
            i += charLength;
            if(charLength < decExponent) {
                charLength = decExponent - charLength;
                moduru::VecUtil::VecCopy(&zero, 0, result, i, charLength);
                i += charLength;
                (*result)[i++] = u'.';
                (*result)[i++] = u'0';
			}
			else {
				(*result)[i++] = u'.';
				if (charLength < nDigits) {
					auto t = nDigits - charLength;
					moduru::VecUtil::VecCopy(&digits, charLength, result, i, t);
					i += t;
				}
				else {
					(*result)[i++] = u'0';
				}
			}
        } else if(decExponent <= 0 && decExponent > -int32_t(3)) {
            (*result)[i++] = u'0';
            (*result)[i++] = u'.';
            if(decExponent != 0) {
                moduru::VecUtil::VecCopy(&zero, 0, result, i, -decExponent);
                i -= decExponent;
            }
            moduru::VecUtil::VecCopy(&digits, 0, result, i, nDigits);
            i += nDigits;
        } else {
            (*result)[i++] = digits[int32_t(0)];
            (*result)[i++] = u'.';
            if(nDigits > 1) {
                moduru::VecUtil::VecCopy(&digits, 1, result, i, nDigits - int32_t(1));
                i += nDigits - int32_t(1);
            } else {
                (*result)[i++] = u'0';
            }
            (*result)[i++] = u'E';
            int32_t e;
            if(decExponent <= 0) {
                (*result)[i++] = u'-';
                e = -decExponent + int32_t(1);
            } else {
                e = decExponent - int32_t(1);
            }
            if(e <= 9) {
                (*result)[i++] = static_cast< char16_t >((e + u'0'));
            } else if(e <= 99) {
                (*result)[i++] = static_cast< char16_t >((e / int32_t(10) + u'0'));
                (*result)[i++] = static_cast< char16_t >((e % int32_t(10) + u'0'));
            } else {
                (*result)[i++] = static_cast< char16_t >((e / int32_t(100) + u'0'));
                e %= 100;
                (*result)[i++] = static_cast< char16_t >((e / int32_t(10) + u'0'));
                (*result)[i++] = static_cast< char16_t >((e % int32_t(10) + u'0'));
            }
        }
    }
    return i;
}

//moduru::lang::ThreadLocal* FloatingDecimal::perThreadBuffer = new FloatingDecimal_1();

/*
void FloatingDecimal::appendTo(Appendable* buf)
{
    auto result = dynamic_cast< vector<char16_t> >((dynamic_cast< Object* >(npc(perThreadBuffer_)->get())));
    auto i = getChars(result);
    if(dynamic_cast< StringBuilder* >(buf) != nullptr)
        npc((dynamic_cast< StringBuilder* >(buf)))->append(result, int32_t(0), i);
    else if(dynamic_cast< StringBuffer* >(buf) != nullptr)
        npc((dynamic_cast< StringBuffer* >(buf)))->append(result, int32_t(0), i);
    // else assert false
}
*/

/*
FloatingDecimal* FloatingDecimal::readJavaFormatString(moduru::lang::String* in)
{

    auto isNegative = false;
    auto signSeen = false;
    int32_t decExp;
    char16_t c;
    try {
        in = npc(in)->trim();
        auto l = npc(in)->length();
        if(l == 0)
            throw new NumberFormatException("empty String");

        auto i = int32_t(0);
        switch (c = npc(in)->charAt(i)) {
        case u'-':
            isNegative = true;
        case u'+':
            i++;
            signSeen = true;
        }

        c = npc(in)->charAt(i);
        if(c == u'N' || c == u'I') {
            auto potentialNaN = false;
            vector<char16_t> targetChars = nullptr;
            if(c == u'N') {
                targetChars = notANumber_;
                potentialNaN = true;
            } else {
                targetChars = infinity_;
            }
            auto j = int32_t(0);
            while (i < l && j < npc(targetChars)->length) {
                if(npc(in)->charAt(i) == (*targetChars)[j]) {
                    i++;
                    j++;
                } else
                    goto parseNumber0_break;
            }
            if((j == npc(targetChars)->length) && (i == l)) {
                return (potentialNaN ? new FloatingDecimal(Double::NaN) : new FloatingDecimal(isNegative ? Double::NEGATIVE_INFINITY : Double::POSITIVE_INFINITY));
            } else {
                goto parseNumber0_break;
            }
        } else if(c == u'0') {
            if(l > i + int32_t(1)) {
                auto ch = npc(in)->charAt(i + int32_t(1));
                if(ch == u'x' || ch == u'X')
                    return parseHexString(in);

            }
        }
        auto digits = vector<char16_t>(l);
        auto nDigits = int32_t(0);
        auto decSeen = false;
        auto decPt = int32_t(0);
        auto nLeadZero = int32_t(0);
        auto nTrailZero = int32_t(0);
        while (i < l) {
            bool digitLoop0_continue = false;
            switch (c = npc(in)->charAt(i)) {
            case u'0':
                if(nDigits > 0) {
                    nTrailZero += 1;
                } else {
                    nLeadZero += 1;
                }
                break;
            case u'1':
            case u'2':
            case u'3':
            case u'4':
            case u'5':
            case u'6':
            case u'7':
            case u'8':
            case u'9':
                while (nTrailZero > 0) {
                    digits[nDigits++] = u'0';
                    nTrailZero -= 1;
                }
                digits[nDigits++] = c;
                break;
            case u'.':
                if(decSeen) {
                    throw new NumberFormatException("multiple points");
                }
                decPt = i;
                if(signSeen) {
                    decPt -= 1;
                }
                decSeen = true;
                break;
            default:
                goto digitLoop0_break;
            }

            i++;

        }

digitLoop0_break:;
        if(nDigits == 0) {
            digits = zero_;
            nDigits = 1;
            if(nLeadZero == 0) {
                goto parseNumber0_break;
            }
        }
        if(decSeen) {
            decExp = decPt - nLeadZero;
        } else {
            decExp = nDigits + nTrailZero;
        }
        if((i < l) && (((c = npc(in)->charAt(i)) == u'e') || (c == u'E'))) {
            auto expSign = int32_t(1);
            auto expVal = int32_t(0);
            auto reallyBig = Integer::MAX_VALUE / int32_t(10);
            auto expOverflow = false;
            switch (npc(in)->charAt(++i)) {
            case u'-':
                expSign = -int32_t(1);
            case u'+':
                i++;
            }

            auto expAt = i;
            while (i < l) {
                bool expLoop0_continue = false;
                if(expVal >= reallyBig) {
                    expOverflow = true;
                }
                switch (c = npc(in)->charAt(i++)) {
                case u'0':
                case u'1':
                case u'2':
                case u'3':
                case u'4':
                case u'5':
                case u'6':
                case u'7':
                case u'8':
                case u'9':
                    expVal = expVal * int32_t(10) + (static_cast< int32_t >(c) - static_cast< int32_t >('0'));
                    continue;
                default:
                    i--;
                    goto expLoop0_break;
                }


            }

expLoop0_break:;
            auto expLimit = bigDecimalExponent + nDigits + nTrailZero;
            if(expOverflow || (expVal > expLimit)) {
                decExp = expSign * expLimit;
            } else {
                decExp = decExp + expSign * expVal;
            }
            if(i == expAt)
                goto parseNumber0_break;

        }
        if(i < l && ((i != l - int32_t(1)) || (npc(in)->charAt(i) != u'f' && npc(in)->charAt(i) != u'F' && npc(in)->charAt(i) != u'd' && npc(in)->charAt(i) != u'D'))) {
            goto parseNumber0_break;
        }
        return new FloatingDecimal(isNegative, decExp, digits, nDigits, false);
    } catch (StringIndexOutOfBoundsException* e) {
    }

parseNumber0_break:;
    throw new NumberFormatException(::StringBuilder().append("For input string: \"")->append(in)
        ->append("\"")->toString());
}
*/

double FloatingDecimal::doubleValue()
{
	auto kDigits = moduru::math::Math::min(nDigits, maxDecimalDigits + int32_t(1));
	int64_t lValue;
	double dValue;
	double rValue, tValue;
	if (digits == infinity || digits == notANumber) {
		if (digits == notANumber)
			return Double::NaN;
		else
			return (isNegative ? Double::NEGATIVE_INFINITY : Double::POSITIVE_INFINITY);
	}
	else {
		if (mustSetRoundDir) {
			roundDir = 0;
		}
		auto iValue = static_cast<int32_t>(digits[int32_t(0)]) - static_cast<int32_t>('0');
		auto iDigits = moduru::math::Math::min(kDigits, intDecimalDigits);
		for (auto i = int32_t(1); i < iDigits; i++) {
			iValue = iValue * int32_t(10) + static_cast<int32_t>(digits[i]) - static_cast<int32_t>('0');
		}
		lValue = static_cast<int64_t>(iValue);
		for (auto i = iDigits; i < kDigits; i++) {
			lValue = lValue * int64_t(10LL) + static_cast<int64_t>((static_cast<int32_t>(digits[i]) - static_cast<int32_t>('0')));
		}
		dValue = static_cast<double>(lValue);
		auto exp = decExponent - kDigits;
		if (nDigits <= maxDecimalDigits) {
			if (exp == 0 || dValue == 0.0)
				return (isNegative) ? -dValue : dValue;
			else if (exp >= 0) {
				if (exp <= maxSmallTen) {
					rValue = dValue * small10pow[exp];
					if (mustSetRoundDir) {
						tValue = rValue / small10pow[exp];
						roundDir = (tValue == dValue) ? int32_t(0) : (tValue < dValue) ? int32_t(1) : -int32_t(1);
					}
					return (isNegative) ? -rValue : rValue;
				}
				auto slop = maxDecimalDigits - kDigits;
				if (exp <= maxSmallTen + slop) {
					dValue *= small10pow[slop];
					rValue = dValue * small10pow[exp - slop];
					if (mustSetRoundDir) {
						tValue = rValue / small10pow[exp - slop];
						roundDir = (tValue == dValue) ? int32_t(0) : (tValue < dValue) ? int32_t(1) : -int32_t(1);
					}
					return (isNegative) ? -rValue : rValue;
				}
			}
			else {
				if (exp >= -maxSmallTen) {
					rValue = dValue / small10pow[-exp];
					tValue = rValue * small10pow[-exp];
					if (mustSetRoundDir) {
						roundDir = (tValue == dValue) ? int32_t(0) : (tValue < dValue) ? int32_t(1) : -int32_t(1);
					}
					return (isNegative) ? -rValue : rValue;
				}
			}
		}
		if (exp > 0) {
			if (decExponent > maxDecimalExponent + int32_t(1)) {
				return (isNegative) ? Double::NEGATIVE_INFINITY : Double::POSITIVE_INFINITY;
			}
			if ((exp & int32_t(15)) != 0) {
				dValue *= small10pow[exp & int32_t(15)];
			}
			if ((exp >>= 4) != 0) {
				int32_t j;
				for (j = 0; exp > 1; j++, exp >>= 1) {
					if ((exp & int32_t(1)) != 0)
						dValue *= big10pow[j];

				}
				auto t = dValue * big10pow[j];
				if (Double::isInfinite(t)) {
					t = dValue / 2.0;
					t *= big10pow[j];
					if (Double::isInfinite(t)) {
						return (isNegative) ? Double::NEGATIVE_INFINITY : Double::POSITIVE_INFINITY;
					}
					t = Double::MAX_VALUE;
				}
				dValue = t;
			}
		}
		else if (exp < 0) {
			exp = -exp;
			if (decExponent < minDecimalExponent - int32_t(1)) {
				return (isNegative) ? -0.0 : 0.0;
			}
			if ((exp & int32_t(15)) != 0) {
				dValue /= small10pow[exp & int32_t(15)];
			}
			if ((exp >>= 4) != 0) {
				int32_t j;
				for (j = 0; exp > 1; j++, exp >>= 1) {
					if ((exp & int32_t(1)) != 0)
						dValue *= tiny10pow[j];

				}
				auto t = dValue * tiny10pow[j];
				if (t == 0.0) {
					t = dValue * 2.0;
					t *= tiny10pow[j];
					if (t == 0.0) {
						return (isNegative) ? -0.0 : 0.0;
					}
					t = Double::MIN_VALUE;
				}
				dValue = t;
			}
		}
		auto bigD0 = new FDBigInt(lValue, digits, kDigits, nDigits);
		exp = decExponent - nDigits;
		while (true) {
			bool correctionLoop0_continue = false;
			auto bigB = doubleToBigInt(dValue);
			int32_t B2, B5;
			int32_t D2, D5;
			int32_t Ulp2;
			if (exp >= 0) {
				B2 = B5 = 0;
				D2 = D5 = exp;
			}
			else {
				B2 = B5 = -exp;
				D2 = D5 = 0;
			}
			if (bigIntExp >= 0) {
				B2 += bigIntExp;
			}
			else {
				D2 -= bigIntExp;
			}
			Ulp2 = B2;
			int32_t hulpbias;
			if (bigIntExp + bigIntNBits <= -expBias + int32_t(1)) {
				hulpbias = bigIntExp + expBias + expShift;
			}
			else {
				hulpbias = expShift + int32_t(2) - bigIntNBits;
			}
			B2 += hulpbias;
			D2 += hulpbias;
			auto common2 = moduru::math::Math::min(B2, moduru::math::Math::min(D2, Ulp2));
			B2 -= common2;
			D2 -= common2;
			Ulp2 -= common2;
			bigB = multPow52(bigB, B5, B2);
			auto bigD = multPow52(new FDBigInt(bigD0), D5, D2);
			FDBigInt* diff;
			int32_t cmpResult;
			bool overvalue;
			if ((cmpResult = bigB->cmp(bigD)) > 0) {
				overvalue = true;
				diff = bigB->sub(bigD);
				if ((bigIntNBits == 1) && (bigIntExp > -expBias)) {
					Ulp2 -= 1;
					if (Ulp2 < 0) {
						Ulp2 = 0;
						diff->lshiftMe(1);
					}
				}
			}
			else if (cmpResult < 0) {
				overvalue = false;
				diff = bigD->sub(bigB);
			}
			else {
				goto correctionLoop0_break;
			}
			auto halfUlp = constructPow52(B5, Ulp2);
			if ((cmpResult = diff->cmp(halfUlp)) < 0) {
				if (mustSetRoundDir) {
					roundDir = overvalue ? -int32_t(1) : int32_t(1);
				}
				goto correctionLoop0_break;
			}
			else if (cmpResult == 0) {
				dValue += 0.5 * ulp(dValue, overvalue);
				if (mustSetRoundDir) {
					roundDir = overvalue ? -int32_t(1) : int32_t(1);
				}
				goto correctionLoop0_break;
			}
			else {
				dValue += ulp(dValue, overvalue);
				if (dValue == 0.0 || dValue == Double::POSITIVE_INFINITY)
					goto correctionLoop0_break;

				continue;
			}

		}

	correctionLoop0_break:;
		return (isNegative) ? -dValue : dValue;
	}
}

float FloatingDecimal::floatValue()
{
    auto kDigits = moduru::math::Math::min(nDigits, singleMaxDecimalDigits + int32_t(1));
    int32_t iValue;
    float fValue;
    if(digits == infinity || digits == notANumber) {
        if(digits == notANumber)
            return Float::NaN;
        else
            return (isNegative ? Float::NEGATIVE_INFINITY : Float::POSITIVE_INFINITY);
    } else {
        iValue = static_cast< int32_t >(digits[int32_t(0)]) - static_cast< int32_t >('0');
        for (auto i = int32_t(1); i < kDigits; i++) {
            iValue = iValue * int32_t(10) + static_cast< int32_t >(digits[i]) - static_cast< int32_t >('0');
        }
        fValue = static_cast< float >(iValue);
        auto exp = decExponent - kDigits;
        if(nDigits <= singleMaxDecimalDigits) {
            if(exp == 0 || fValue == 0.0f)
                return (isNegative) ? -fValue : fValue;
            else if(exp >= 0) {
                if(exp <= singleMaxSmallTen) {
                    fValue *= singleSmall10pow[exp];
                    return (isNegative) ? -fValue : fValue;
                }
                auto slop = singleMaxDecimalDigits - kDigits;
                if(exp <= singleMaxSmallTen + slop) {
                    fValue *= singleSmall10pow[slop];
                    fValue *= singleSmall10pow[exp - slop];
                    return (isNegative) ? -fValue : fValue;
                }
            } else {
                if(exp >= -singleMaxSmallTen) {
                    fValue /= singleSmall10pow[-exp];
                    return (isNegative) ? -fValue : fValue;
                }
            }
        } else if((decExponent >= nDigits) && (nDigits + decExponent <= maxDecimalDigits)) {
            auto lValue = static_cast< int64_t >(iValue);
            for (auto i = kDigits; i < nDigits; i++) {
                lValue = lValue * int64_t(10LL) + static_cast< int64_t >((static_cast< int32_t >(digits[i]) - static_cast< int32_t >('0')));
            }
            auto dValue = static_cast< double >(lValue);
            exp = decExponent - nDigits;
            dValue *= small10pow[exp];
            fValue = static_cast< float >(dValue);
            return (isNegative) ? -fValue : fValue;
        }
        if(decExponent > singleMaxDecimalExponent + int32_t(1)) {
            return (isNegative) ? Float::NEGATIVE_INFINITY : Float::POSITIVE_INFINITY;
        } else if(decExponent < singleMinDecimalExponent - int32_t(1)) {
            return (isNegative) ? -0.0f : 0.0f;
        }
        mustSetRoundDir = !fromHex;
        auto dValue = doubleValue();
        return stickyRound(dValue);
    }
}

vector<double> FloatingDecimal::small10pow = vector<double>({ 1.0 , 10.0 , 100.0 , 1000.0 , 10000.0 , 100000.0 , 1000000.0 , 1.0E7 , 1.0E8 , 1.0E9 , 1.0E10 , 1.0E11 , 1.0E12 , 1.0E13 , 1.0E14 , 1.0E15 , 1.0E16 , 1.0E17 , 1.0E18 , 1.0E19 , 1.0E20 , 1.0E21 , 1.0E22 });
vector<float> FloatingDecimal::singleSmall10pow = vector<float>({ 1.0f , 10.0f , 100.0f , 1000.0f , 10000.0f , 100000.0f , 1000000.0f , 1.0E7f , 1.0E8f , 1.0E9f , 1.0E10f });
vector<double> FloatingDecimal::big10pow = vector<double>({ 1.0E16, 1.0E32, 1.0E64, 1.0E128, 1.0E256 });
vector<double> FloatingDecimal::tiny10pow = vector<double>({ 1.0E-16, 1.0E-32, 1.0E-64, 1.0E-128, 1.0E-256 });
int32_t FloatingDecimal::maxSmallTen = (int32_t) (small10pow.size()) - 1;
int32_t FloatingDecimal::singleMaxSmallTen = (int32_t) (FloatingDecimal::singleSmall10pow.size()) - 1;
vector<int32_t> FloatingDecimal::small5pow = vector<int32_t>({ 1 , 5 , 5 * 5 , 5 * 5 * 5 , 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 });
vector<int64_t> FloatingDecimal::long5pow = vector<int64_t>({ int64_t(1LL) , int64_t(5LL) , int64_t(5LL) * 5 , int64_t(5LL) * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 , int64_t(5LL) * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 * 5 });
vector<int32_t> FloatingDecimal::n5bits = vector<int32_t>({ 0, 3, 5, 7, 10, 12, 14, 17, 19, 21, 24, 26, 28, 31, 33, 35, 38, 40, 42, 45, 47, 49, 52, 54, 56, 59, 61 });
vector<char16_t> FloatingDecimal::infinity = vector<char16_t>({ 'I' , 'n' , 'f' , 'i' , 'n' , 'i' , 't' , 'y' });
vector<char16_t> FloatingDecimal::notANumber = vector<char16_t>({ 'N' , 'a' , 'N' });
vector<char16_t> FloatingDecimal::zero = vector<char16_t>({ '0' , '0' , '0' , '0' , '0' , '0' , '0' , '0' });

//moduru::util::regex::Pattern* FloatingDecimal::hexFloatPattern_ = moduru::util::regex::Pattern::compile("([-+])?0[xX](((\\p{XDigit}+)\\.?)|((\\p{XDigit}*)\\.(\\p{XDigit}+)))[pP]([-+])?(\\p{Digit}+)[fFdD]?");

/*
FloatingDecimal* FloatingDecimal::parseHexString(string s)
{

    auto m = hexFloatPattern->matcher(s);
    auto validInput = m->matches();
    if(!validInput) {
        throw new NumberFormatException(::StringBuilder().append("For input string: \"")->append(s)
            ->append("\"")->toString());
    } else {
        auto group1 = npc(m)->group(int32_t(1));
        auto sign = ((group1 == nullptr) || npc(group1)->equals(static_cast< Object* >("+"))) ? 1.0 : -1.0;
        String* significandString = nullptr;
        auto signifLength = int32_t(0);
        auto exponentAdjust = int32_t(0);
{
            auto leftDigits = int32_t(0);
            auto rightDigits = int32_t(0);
            String* group4;
            if((group4 = npc(m)->group(int32_t(4))) != nullptr) {
                significandString = stripLeadingZeros(group4);
                leftDigits = npc(significandString)->length();
            } else {
                auto group6 = stripLeadingZeros(npc(m)->group(int32_t(6)));
                leftDigits = npc(group6)->length();
                auto group7 = npc(m)->group(int32_t(7));
                rightDigits = npc(group7)->length();
                significandString = ::StringBuilder().append(((group6 == nullptr) ? u"" : group6))->append(group7)->toString();
            }
            significandString = stripLeadingZeros(significandString);
            signifLength = npc(significandString)->length();
            if(leftDigits >= 1) {
                exponentAdjust = int32_t(4) * (leftDigits - int32_t(1));
            } else {
                exponentAdjust = -int32_t(4) * (rightDigits - signifLength + int32_t(1));
            }
            if(signifLength == 0) {
                return new FloatingDecimal(sign * 0.0);
            }
        }

        auto group8 = npc(m)->group(int32_t(8));
        auto positiveExponent = (group8 == nullptr) || npc(group8)->equals(static_cast< Object* >("+"));
        int64_t unsignedRawExponent;
        try {
            unsignedRawExponent = Integer::parseInt(npc(m)->group(int32_t(9)));
        } catch (NumberFormatException* e) {
            return new FloatingDecimal(sign * (positiveExponent ? Double::POSITIVE_INFINITY : 0.0));
        }
        auto rawExponent = (positiveExponent ? int64_t(1LL) : -int64_t(1LL)) * unsignedRawExponent;
        auto exponent = rawExponent + exponentAdjust;
        auto round = false;
        auto sticky = false;
        auto bitsCopied = int32_t(0);
        auto nextShift = int32_t(0);
        auto significand = int64_t(0LL);
        int64_t leadingDigit = getHexDigit(significandString, 0);
        if(leadingDigit == 1) {
            significand |= leadingDigit << int32_t(52);
            nextShift = int32_t(52) - int32_t(4);
        } else if(leadingDigit <= 3) {
            significand |= leadingDigit << int32_t(51);
            nextShift = int32_t(52) - int32_t(5);
            exponent += 1;
        } else if(leadingDigit <= 7) {
            significand |= leadingDigit << int32_t(50);
            nextShift = int32_t(52) - int32_t(6);
            exponent += 2;
        } else if(leadingDigit <= 15) {
            significand |= leadingDigit << int32_t(49);
            nextShift = int32_t(52) - int32_t(7);
            exponent += 3;
        } else {
            throw new AssertionError(static_cast< Object* >("Result from digit conversion too large!"));
        }
        auto i = int32_t(0);
        for (i = 1; i < signifLength && nextShift >= 0; i++) {
            int64_t currentDigit = getHexDigit(significandString, i);
            significand |= (currentDigit << nextShift);
            nextShift -= 4;
        }
        if(i < signifLength) {
            int64_t currentDigit = getHexDigit(significandString, i);
            switch (nextShift) {
            case -int32_t(1):
                significand |= ((currentDigit & int64_t(14LL)) >> int32_t(1));
                round = (currentDigit & int64_t(1LL)) != 0LL;
                break;
            case -int32_t(2):
                significand |= ((currentDigit & int64_t(12LL)) >> int32_t(2));
                round = (currentDigit & int64_t(2LL)) != 0LL;
                sticky = (currentDigit & int64_t(1LL)) != 0;
                break;
            case -int32_t(3):
                significand |= ((currentDigit & int64_t(8LL)) >> int32_t(3));
                round = (currentDigit & int64_t(4LL)) != 0LL;
                sticky = (currentDigit & int64_t(3LL)) != 0;
                break;
            case -int32_t(4):
                round = ((currentDigit & int64_t(8LL)) != 0);
                sticky = (currentDigit & int64_t(7LL)) != 0;
                break;
            default:
                throw new AssertionError(static_cast< Object* >("Unexpected shift distance remainder."));
            }

            i++;
            while (i < signifLength && !sticky) {
                currentDigit = getHexDigit(significandString, i);
                sticky = sticky || (currentDigit != 0);
                i++;
            }
        }
        if(exponent > DoubleConsts::MAX_EXPONENT) {
            return new FloatingDecimal(sign * Double::POSITIVE_INFINITY);
        } else {
            if(exponent <= DoubleConsts::MAX_EXPONENT && exponent >= DoubleConsts::MIN_EXPONENT) {
                significand = (((static_cast< int64_t >(exponent) + static_cast< int64_t >(DoubleConsts::EXP_BIAS)) << (DoubleConsts::SIGNIFICAND_WIDTH - int32_t(1))) & DoubleConsts::EXP_BIT_MASK) | (DoubleConsts::SIGNIF_BIT_MASK & significand);
            } else {
                if(exponent < (DoubleConsts::MIN_SUB_EXPONENT - int32_t(1))) {
                    return new FloatingDecimal(sign * 0.0);
                } else {
                    sticky = sticky || round;
                    round = false;
                    auto bitsDiscarded = int32_t(53) - (static_cast< int32_t >(exponent) - DoubleConsts::MIN_SUB_EXPONENT + int32_t(1));
                    round = (significand & (int64_t(1LL) << (bitsDiscarded - int32_t(1)))) != 0LL;
                    if(bitsDiscarded > 1) {
                        auto mask = ~((~int64_t(0LL)) << (bitsDiscarded - int32_t(1)));
                        sticky = sticky || ((significand & mask) != 0LL);
                    }
                    significand = significand >> bitsDiscarded;
                    significand = (((static_cast< int64_t >((DoubleConsts::MIN_EXPONENT - int32_t(1))) + static_cast< int64_t >(DoubleConsts::EXP_BIAS)) << (DoubleConsts::SIGNIFICAND_WIDTH - int32_t(1))) & DoubleConsts::EXP_BIT_MASK) | (DoubleConsts::SIGNIF_BIT_MASK & significand);
                }
            }
            auto incremented = false;
            auto leastZero = ((significand & int64_t(1LL)) == 0LL);
            if((leastZero && round && sticky) || ((!leastZero) && round)) {
                incremented = true;
                significand++;
            }
            auto fd = new FloatingDecimal(FpUtils::rawCopySign(Double::longBitsToDouble(significand), sign));
            if((exponent >= FloatConsts::MIN_SUB_EXPONENT - int32_t(1)) && (exponent <= FloatConsts::MAX_EXPONENT)) {
                if((significand & int64_t(268435455LL)) == 0LL) {
                    if(round || sticky) {
                        if(leastZero) {
                            if(round ^ sticky) {
                                npc(fd)->roundDir = 1;
                            }
                        } else {
                            if(round)
                                npc(fd)->roundDir = -int32_t(1);

                        }
                    }
                }
            }
            npc(fd)->fromHex = true;
            return fd;
        }
    }
}
*/

/*
moduru::lang::String* FloatingDecimal::stripLeadingZeros(String* s)
{

    return s->replaceFirst("^0+", u"");
}


int32_t FloatingDecimal::getHexDigit(String* s, int32_t position)
{

    auto value = Character::digit(npc(s)->charAt(position), int32_t(16));
    if(value <= -int32_t(1) || value >= 16) {
        throw new AssertionError(static_cast< Object* >(::StringBuilder().append("Unexpected failure of digit conversion of ")->append(npc(s)->charAt(position))->toString()));
    }
    return value;
}
*/
