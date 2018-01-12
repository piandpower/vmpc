#include <lang/FDBigInt.hpp>

#include <lang/Integer.hpp>
//#include <lang/Math.hpp>

#include <math/Math.hpp>

#include <VecUtil.hpp>

#ifdef __linux__
#include <stdexcept>
#endif

using namespace moduru::lang;
using namespace std;

FDBigInt::FDBigInt(int32_t v)
{
	nWords = 1;
	data = vector<int32_t>(1);
	data[0] = v;
}

FDBigInt::FDBigInt(int64_t v)
{
	data = vector<int32_t>(2);
	data[0] = static_cast< int32_t >(v);
	data[1] = static_cast< int32_t >((static_cast<int64_t>(static_cast<uint64_t>(v) >> 32)));
	nWords = (data[1] == 0) ? 1 : 2;
}

FDBigInt::FDBigInt(FDBigInt* other)
{
	data = vector<int32_t>(nWords = other->nWords);
	moduru::VecUtil::VecCopy(&other->data, 0, &data, 0, nWords);
}

FDBigInt::FDBigInt(vector<int32_t> d, int32_t n)
{
	data = d;
	nWords = n;
}

FDBigInt::FDBigInt(int64_t seed, vector<char16_t> digit, int32_t nd0, int32_t nd)
{
	auto n = (nd + int32_t(8)) / int32_t(9);
	if (n < 2)
		n = 2;

	data = vector<int32_t>(n);
	data[0] = static_cast< int32_t >(seed);
	data[1] = static_cast< int32_t >((static_cast<int64_t>(static_cast<uint64_t>(seed) >> 32)));
	nWords = (data[1] == 0) ? 1 : 2;
	auto i = nd0;
	auto limit = nd - int32_t(5);
	int32_t v;
	while (i < limit) {
		auto ilim = i + int32_t(5);
		v = static_cast< int32_t >(digit[i++]) - static_cast< int32_t >(u'0');
		while (i < ilim) {
			v = int32_t(10) * v + static_cast< int32_t >(digit[i++]) - static_cast< int32_t >(u'0');
		}
		multaddMe(100000, v);
	}
	auto factor = 1;
	v = 0;
	while (i < nd) {
		v = int32_t(10) * v + static_cast< int32_t >(digit[i++]) - static_cast< int32_t >(u'0');
		factor *= 10;
	}
	if (factor != 1) {
		multaddMe(factor, v);
	}
}

void FDBigInt::lshiftMe(int32_t c)
{
	if (c <= 0) {
		if (c == 0)
			return;
		else
			throw std::invalid_argument("negative shift count");
	}
	auto wordcount = c >> int32_t(5);
	auto bitcount = c & int32_t(31);
	auto anticount = 32 - bitcount;
	auto t = data;
	auto s = data;
	if (nWords + wordcount + 1 > t.size()) {
		t = vector<int32_t>(nWords + wordcount + 1);
	}
	auto target = nWords + wordcount;
	auto src = nWords - 1;
	if (bitcount == 0) {
		moduru::VecUtil::VecCopy(&s, 0, &t, wordcount, nWords);
		target = wordcount - 1;
	}
	else {
		t[target--] = static_cast<int32_t>(static_cast<uint32_t>(s[src]) >> anticount);
		while (src >= 1) {
			t[target--] = (s[src] << bitcount) | (static_cast<int32_t>(static_cast<uint32_t>(s[--src]) >> anticount));
		}
		t[target--] = s[src] << bitcount;
	}
	while (target >= 0) {
		t[target--] = 0;
	}
	data = t;
	nWords += wordcount + 1;
	while (nWords > 1 && data[nWords - 1] == 0)
		nWords--;

}

int32_t FDBigInt::normalizeMe()
{
	int32_t src;
	auto wordcount = 0;
	auto bitcount = 0;
	auto v = 0;
	for (src = nWords - 1; src >= 0 && (v = data[src]) == 0; src--) {
		wordcount += 1;
	}
	if (src < 0) {
		throw std::invalid_argument("zero value");
	}
	nWords -= wordcount;
	if ((v & int32_t(-268435456)) != 0) {
		for (bitcount = 32; (v & int32_t(-268435456)) != 0; bitcount--)
			v = static_cast<uint32_t>(v) >> 1;

	}
	else {
		while (v <= 1048575) {
			v <<= 8;
			bitcount += 8;
		}
		while (v <= 134217727) {
			v <<= 1;
			bitcount += 1;
		}
	}
	if (bitcount != 0)
		lshiftMe(bitcount);

	return bitcount;
}

FDBigInt* FDBigInt::mult(int32_t iv)
{
	int64_t v = iv;
	vector<int32_t> r;
	int64_t p;
	r = vector<int32_t>((v * (static_cast<int64_t>(data[nWords - 1]) & int64_t(4294967295LL)) > 268435455LL) ? nWords + 1 : nWords);
	p = 0LL;
	for (auto i = 0; i < nWords; i++) {
		p += v * (static_cast<int64_t>(data[i]) & int64_t(4294967295LL));
		r[i] = static_cast<int32_t>(p);
		p = static_cast<uint64_t>(p) >> 32;
	}
	if (p == 0LL) {
		return new FDBigInt(r, nWords);
	}
	else {
		r[nWords] = static_cast<int32_t>(p);
		return new FDBigInt(r, nWords + 1);
	}
}

void FDBigInt::multaddMe(int32_t iv, int32_t addend)
{
    int64_t v = iv;
    int64_t p;
    p = v * (static_cast< int64_t >(data[0]) & int64_t(4294967295LL)) + (static_cast< int64_t >(addend) & int64_t(4294967295LL));
    data[0] = static_cast< int32_t >(p);
    p = static_cast<uint64_t>(p) >> 32;
    for (auto i = 1; i < nWords; i++) {
        p += v * (static_cast< int64_t >(data[i]) & int64_t(4294967295LL));
        data[i] = static_cast< int32_t >(p);
        p = static_cast<uint64_t>(p) >> 32;
    }
    if(p != 0LL) {
        data[nWords] = static_cast< int32_t >(p);
        nWords++;
    }
}

FDBigInt* FDBigInt::mult(FDBigInt* other)
{
	auto r = vector<int32_t>(nWords + other->nWords);
	int32_t i;
	for (i = 0; i < this->nWords; i++) {
		auto v = static_cast<int64_t>(this->data[i]) & int64_t(4294967295LL);
		auto p = int64_t(0LL);
		int32_t j;
		for (j = 0; j < other->nWords; j++) {
			p += (static_cast<int64_t>(r[i + j]) & int64_t(4294967295LL)) + v * (static_cast<int64_t>((other->data)[j]) & int64_t(4294967295LL));
			r[i + j] = static_cast<int32_t>(p);
			p = static_cast<uint64_t>(p) >> 32;
		}
		r[i + j] = static_cast<int32_t>(p);
	}
	for (i = (int)(r.size()) - 1; i > 0; i--)
		if (r[i] != 0)
			break;

	return new FDBigInt(r, i + 1);
}

FDBigInt* FDBigInt::add(FDBigInt* other)
{
	int32_t i;
	vector<int32_t> a;
	vector<int32_t> b;
	int32_t n, m;
	auto c = int64_t(0LL);
	if (this->nWords >= other->nWords) {
		a = this->data;
		n = this->nWords;
		b = other->data;
		m = other->nWords;
	}
	else {
		a = other->data;
		n = other->nWords;
		b = this->data;
		m = this->nWords;
	}
	auto r = vector<int32_t>(n);
	for (i = 0; i < n; i++) {
		c += static_cast<int64_t>(a[i]) & int64_t(4294967295LL);
		if (i < m) {
			c += static_cast<int64_t>(b[i]) & int64_t(4294967295LL);
		}
		r[i] = static_cast<int32_t>(c);
		c >>= 32;
	}
	if (c != 0LL) {
		auto s = vector<int32_t>(r.size() + 1);
		moduru::VecUtil::VecCopy(&r, 0, &s, 0, r.size());
		s[i++] = static_cast<int32_t>(c);
		return new FDBigInt(s, i);
	}
	return new FDBigInt(r, i);
}

FDBigInt* FDBigInt::sub(FDBigInt* other)
{
	auto r = vector<int32_t>(this->nWords);
	int32_t i;
	auto n = this->nWords;
	auto m = other->nWords;
	auto nzeros = 0;
	auto c = int64_t(0LL);
	for (i = 0; i < n; i++) {
		c += static_cast<int64_t>(this->data[i]) & int64_t(4294967295LL);
		if (i < m) {
			c -= static_cast<int64_t>(other->data[i]) & int64_t(4294967295LL);
		}
		if ((r[i] = static_cast<int32_t>(c)) == 0)
			nzeros++;
		else
			nzeros = 0;
		c >>= 32;
	}
	/* assert(c == 0LL : c) */;
	/* assert(dataInRangeIsZero(i, m, other)) */;
	return new FDBigInt(r, n - nzeros);
}

bool FDBigInt::dataInRangeIsZero(int32_t i, int32_t m, FDBigInt* other)
{
	while (i < m)
		if (other->data[i++] != 0)
			return false;
	return true;
}

int32_t FDBigInt::cmp(FDBigInt* other)
{
	int32_t i;
	if (this->nWords > other->nWords) {
		auto j = other->nWords - 1;
		for (i = this->nWords - 1; i > j; i--)
			if (this->data[i] != 0)
				return 1;


	}
	else if (this->nWords < other->nWords) {
		auto j = this->nWords - 1;
		for (i = other->nWords - 1; i > j; i--)
			if (other->data[i] != 0)
				return -1;


	}
	else {
		i = this->nWords - 1;
	}
	for (; i > 0; i--)
		if (this->data[i] != other->data[i])
			break;


	auto a = this->data[i];
	auto b = other->data[i];
	if (a < 0) {
		if (b < 0) {
			return a - b;
		}
		else {
			return 1;
		}
	}
	else {
		if (b < 0) {
			return -1;
		}
		else {
			return a - b;
		}
	}
}

int32_t FDBigInt::quoRemIteration(FDBigInt* S)
{
	if (nWords != S->nWords) {
		throw std::invalid_argument("disparate values");
	}
	auto n = nWords - 1;
	auto q = (static_cast<int64_t>(data[n]) & int64_t(4294967295LL)) / static_cast<int64_t>(S->data[n]);
	auto diff = int64_t(0LL);
	for (auto i = 0; i <= n; i++) {
		diff += (static_cast<int64_t>(data[i]) & int64_t(4294967295LL)) - q * (static_cast<int64_t>(S->data[i]) & int64_t(4294967295LL));
		data[i] = static_cast<int32_t>(diff);
		diff >>= 32;
	}
	if (diff != 0LL) {
		auto sum = int64_t(0LL);
		while (sum == 0LL) {
			sum = 0LL;
			for (auto i = 0; i <= n; i++) {
				sum += (static_cast<int64_t>(data[i]) & int64_t(4294967295LL)) + (static_cast<int64_t>(S->data[i]) & int64_t(4294967295LL));
				data[i] = static_cast<int32_t>(sum);
				sum >>= 32;
			}
			/* assert(sum == 0 || sum == 1 : sum) */;
			q -= 1;
		}
	}
	auto p = int64_t(0LL);
	for (auto i = 0; i <= n; i++) {
		p += int32_t(10) * (static_cast<int64_t>(data[i]) & int64_t(4294967295LL));
		data[i] = static_cast<int32_t>(p);
		p >>= 32;
	}
	/* assert(p == 0LL : p) */;
	return static_cast<int32_t>(q);
}

int64_t FDBigInt::longValue()
{
	/* assert(this->nWords > 0 : this->nWords) */;
	if (this->nWords == 1)
		return (static_cast<int64_t>(data[0]) & int64_t(4294967295LL));

	/* assert(dataInRangeIsZero(2, this->nWords, this)) */;
	/* assert(data[1] >= 0) */;
	return (static_cast<int64_t>((data[1])) << 32) | (static_cast<int64_t>(data[0]) & int64_t(4294967295LL));
}

string FDBigInt::toString()
{
	string r = "";
	r.resize(30);
	r += "[";
	auto i = moduru::math::Math::min(nWords - 1, (int)(data.size()) - 1);
	if (nWords > data.size()) {
		r += "(" + to_string(data.size()) + "<" + to_string(nWords) + "!)";
	}
	for (; i > 0; i--) {
		r += Integer::toHexString(data[i]);
		r.push_back(' ');
	}
	r += Integer::toHexString(data[0]);
	r.push_back(']');
	return r;
}
